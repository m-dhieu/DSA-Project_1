#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

/*
 * Pharmacy Inventory System using a Binary Search Tree.
 * The medicine code is the BST key. Smaller keys go left and larger keys go
 * right. Duplicate keys update the existing quantity and do not create a node.
 * The program validates file records, supports repeated searches, prints an
 * in-order inventory listing, and releases all allocated memory.
 */

MedicineNode *createNode(const char *code, const char *name, int quantity, double price)
{
    MedicineNode *node = malloc(sizeof(*node));
    if (node == NULL) return NULL;
    snprintf(node->medicineCode, sizeof(node->medicineCode), "%s", code);
    snprintf(node->medicineName, sizeof(node->medicineName), "%s", name);
    node->quantity = quantity;
    node->unitPrice = price;
    node->left = NULL;
    node->right = NULL;
    return node;
}

MedicineNode *insertMedicine(MedicineNode *root, const char *code, const char *name, int quantity, 
		double price, int *wasDuplicate, int *memoryError)
{
    int comparison;
    MedicineNode *newNode;
    if (root == NULL)
    {
        newNode = createNode(code, name, quantity, price);
        if (newNode == NULL)
        {
            *memoryError = 1;
            return NULL;
        }
        *wasDuplicate = 0;
        return newNode;
    }
    comparison = strcmp(code, root->medicineCode);
    if (comparison < 0)
    {
        root->left = insertMedicine(root->left, code, name, quantity, price, wasDuplicate, memoryError);
    }
    else if (comparison > 0)
    {
        root->right = insertMedicine(root->right, code, name, quantity, price, wasDuplicate, memoryError);
    }
    else
    {
        root->quantity = quantity;
        *wasDuplicate = 1;
    }
    return root;
}

MedicineNode *searchMedicine(MedicineNode *root, const char *code)
{
    int comparison;
    if (root == NULL) return NULL;
    comparison = strcmp(code, root->medicineCode);
    if (comparison == 0) return root;
    if (comparison < 0) return searchMedicine(root->left, code);
    return searchMedicine(root->right, code);
}

void displayMedicine(const MedicineNode *medicine)
{
    if (medicine == NULL) return;
    printf("\nMedicine Code : %s\n", medicine->medicineCode);
    printf("Medicine Name : %s\n", medicine->medicineName);
    printf("Quantity      : %d\n", medicine->quantity);
    printf("Unit Price    : %.2f\n", medicine->unitPrice);
}

void inorderTraversal(const MedicineNode *root)
{
    if (root == NULL) return;
    inorderTraversal(root->left);
    printf("%-8s | %-35s | %8d | %10.2f\n", root->medicineCode, root->medicineName, root->quantity, root->unitPrice);
    inorderTraversal(root->right);
}

void freeTree(MedicineNode *root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int isValidCode(const char *code)
{
    size_t i;
    if (code == NULL || code[0] != 'M' || strlen(code) < 2 || strlen(code) >= CODE_SIZE) return 0;
    for (i = 1; code[i] != '\0'; i++)
        if (!isdigit((unsigned char)code[i])) return 0;
    return 1;
}

int isValidQuantity(const char *text, int *quantity)
{
    char *endPointer;
    long value;
    if (text == NULL || text[0] == '\0' || quantity == NULL) return 0;
    errno = 0;
    endPointer = NULL;
    value = strtol(text, &endPointer, 10);
    if (errno == ERANGE || endPointer == text || *endPointer != '\0' || value < 0 || value > INT_MAX) return 0;
    *quantity = (int)value;
    return 1;
}

int isValidPrice(const char *text, double *price)
{
    char *endPointer;
    double value;
    size_t i;
    int decimalPoints = 0;
    int digitFound = 0;
    if (text == NULL || text[0] == '\0' || price == NULL) return 0;
    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.')
        {
            decimalPoints++;
            if (decimalPoints > 1) return 0;
        }
        else if (isdigit((unsigned char)text[i])) digitFound = 1;
        else return 0;
    }
    if (!digitFound) return 0;
    errno = 0;
    endPointer = NULL;
    value = strtod(text, &endPointer);
    if (errno == ERANGE || endPointer == text || *endPointer != '\0' || value < 0) return 0;
    *price = value;
    return 1;
}

int isBlankLine(const char *line)
{
    size_t i;
    for (i = 0; line[i] != '\0'; i++)
        if (!isspace((unsigned char)line[i])) return 0;
    return 1;
}

static void discardRemainingLine(FILE *file)
{
    int character;
    while ((character = fgetc(file)) != '\n' && character != EOF) { }
}

MedicineNode *loadInventory(const char *filename, int *validRecords, int *duplicateRecords, int *uniqueMedicines, LoadStatus *status)
{
    FILE *file;
    MedicineNode *root = NULL;
    char line[LINE_SIZE];
    int lineNumber = 0;
    *validRecords = 0;
    *duplicateRecords = 0;
    *uniqueMedicines = 0;
    *status = LOAD_SUCCESS;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        *status = LOAD_FILE_ERROR;
        return NULL;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *code, *name, *quantityText, *priceText, *extraField;
        int quantity, wasDuplicate, memoryError;
        double price;
        MedicineNode *newRoot;
        lineNumber++;
        if (strchr(line, '\n') == NULL && !feof(file))
        {
            discardRemainingLine(file);
            printf("Warning: Overlong record on line %d skipped.\n", lineNumber);
            continue;
        }
        line[strcspn(line, "\r\n")] = '\0';
        if (isBlankLine(line))
        {
            printf("Warning: Blank line %d skipped.\n", lineNumber);
            continue;
        }
        code = strtok(line, "|");
        name = strtok(NULL, "|");
        quantityText = strtok(NULL, "|");
        priceText = strtok(NULL, "|");
        extraField = strtok(NULL, "|");
        if (code == NULL || name == NULL || quantityText == NULL || priceText == NULL || extraField != NULL)
        {
            printf("Warning: Malformed record on line %d skipped.\n", lineNumber);
            continue;
        }
        if (!isValidCode(code) || name[0] == '\0' || strlen(name) >= NAME_SIZE ||
            !isValidQuantity(quantityText, &quantity) || !isValidPrice(priceText, &price))
        {
            printf("Warning: Invalid record on line %d skipped.\n", lineNumber);
            continue;
        }
        memoryError = 0;
	newRoot = insertMedicine(root, code, name, quantity, price, &wasDuplicate, &memoryError);
	if (memoryError){
	    fclose(file);
	    freeTree(root);
            *status = LOAD_MEMORY_ERROR;
            return NULL;}
        root = newRoot;
        (*validRecords)++;
        if (wasDuplicate) (*duplicateRecords)++;
        else (*uniqueMedicines)++;
    }
    fclose(file);
    if (root == NULL) *status = LOAD_NO_VALID_RECORDS;
    return root;
}

static int readSearchCode(char *code, size_t codeSize)
{
    char line[100];
    printf("\nEnter medicine code to search, or type quit: ");
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    if (strchr(line, '\n') == NULL && !feof(stdin))
    {
        discardRemainingLine(stdin);
        printf("Input is too long.\n");
        return -1;
    }
    line[strcspn(line, "\r\n")] = '\0';
    if (strcmp(line, "quit") == 0 || strcmp(line, "QUIT") == 0) return 0;
    if (line[0] == '\0' || strlen(line) >= codeSize)
    {
        printf("Invalid search code.\n");
        return -1;
    }
    snprintf(code, codeSize, "%s", line);
    return 1;
}

int main(void)
{
    MedicineNode *root, *medicine;
    LoadStatus status;
    char searchCode[CODE_SIZE];
    int validRecords, duplicateRecords, uniqueMedicines, inputResult;
    root = loadInventory("inventory.txt", &validRecords, &duplicateRecords, &uniqueMedicines, &status);
    if (status == LOAD_FILE_ERROR)
    {
        fprintf(stderr, "Error: Could not open inventory.txt.\n");
        return 1;
    }
    if (status == LOAD_MEMORY_ERROR)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }
    if (status == LOAD_NO_VALID_RECORDS)
    {
        printf("No valid medicine records found.\n");
        return 0;
    }
    printf("\n PHARMACY INVENTORY SYSTEM \n");
    printf("\nValid records processed : %d\n", validRecords);
    printf("Duplicate records      : %d\n", duplicateRecords);
    printf("Unique medicines       : %d\n", uniqueMedicines);
    printf("\n--- COMPLETE INVENTORY ---\n");
    printf("%-8s | %-35s | %8s | %10s\n", "Code", "Medicine Name", "Quantity", "Unit Price");
    inorderTraversal(root);
    while (1)
    {
        inputResult = readSearchCode(searchCode, sizeof(searchCode));
        if (inputResult == 0) break;
        if (inputResult < 0) continue;
        medicine = searchMedicine(root, searchCode);
        if (medicine == NULL)
            printf("\nMedicine with code '%s' not found.\n", searchCode);
        else
        {
            printf("\nMedicine found:");
            displayMedicine(medicine);
        }
    }
    freeTree(root);
    printf("\nAll allocated memory is released.\n");
    printf("Program terminated :)\n");
    return 0;
}


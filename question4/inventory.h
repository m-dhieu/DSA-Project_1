#ifndef INVENTORY_H
#define INVENTORY_H
#define CODE_SIZE 20
#define NAME_SIZE 100
#define LINE_SIZE 256

typedef struct MedicineNode
{
    char medicineCode[CODE_SIZE];
    char medicineName[NAME_SIZE];
    int quantity;
    double unitPrice;
    struct MedicineNode *left;
    struct MedicineNode *right;
} MedicineNode;

typedef enum
{
    LOAD_SUCCESS,
    LOAD_FILE_ERROR,
    LOAD_NO_VALID_RECORDS,
    LOAD_MEMORY_ERROR
} LoadStatus;

MedicineNode *createNode(const char *code, const char *name, int quantity, double price);
MedicineNode *insertMedicine(MedicineNode *root, const char *code, const char *name, int quantity, double price, int *wasDuplicate, int *memoryError);
MedicineNode *searchMedicine(MedicineNode *root, const char *code);
void displayMedicine(const MedicineNode *medicine);
void inorderTraversal(const MedicineNode *root);
void freeTree(MedicineNode *root);
int isValidCode(const char *code);
int isValidQuantity(const char *text, int *quantity);
int isValidPrice(const char *text, double *price);
int isBlankLine(const char *line);
MedicineNode *loadInventory(const char *filename, int *validRecords, int *duplicateRecords, int *uniqueMedicines, LoadStatus *status);

#endif


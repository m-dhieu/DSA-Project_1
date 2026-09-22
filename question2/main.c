#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

typedef struct Node {
    char identifier[MAX_LENGTH];
    char sampleType[MAX_LENGTH];
    int priority;
    struct Node *next;
    struct Node *prev;
} Node;

/* new sample node */
Node *createNode(const char identifier[], const char sampleType[], int priority) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    /* copy strings safely within max len */
    strncpy(newNode->identifier, identifier, MAX_LENGTH - 1);
    newNode->identifier[MAX_LENGTH - 1] = '\0';
    strncpy(newNode->sampleType, sampleType, MAX_LENGTH - 1);
    newNode->sampleType[MAX_LENGTH - 1] = '\0';
    newNode->priority = priority;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/* insert new sample at end of queue */
int insertAtEnd(Node **head, Node **tail,
                 const char identifier[], const char sampleType[], int priority) {
    Node *newNode = createNode(identifier, sampleType, priority);

    if (newNode == NULL) {
        return 0;
    }  
    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        newNode->prev = *tail;
        (*tail)->next = newNode;
        *tail = newNode;
    }
    return 1;
}

/* current sample */
void displayCurrent(const Node *current) {
    if (current == NULL) {
        printf("\nQueue is empty.\n");
        return;
    }
    printf("\nCurrent Sample\n");
    printf("Identifier: %s\n", current->identifier);
    printf("Sample Type: %s\n", current->sampleType);
    printf("Priority: %d\n", current->priority);
}

/* next sample */
Node *moveNext(Node *current) {
    if (current == NULL) {
        printf("\nQueue is empty.\n");
        return NULL;
    }
    if (current->next == NULL) {
        printf("\nAlready at the last sample.\n");
        return current;
    }
    return current->next;
}

/* previous sample */
Node *movePrevious(Node *current) {
    if (current == NULL) {
        printf("\nQueue is empty.\n");
        return NULL;
    }
    if (current->prev == NULL) {
        printf("\nAlready at the first sample.\n");
        return current;
    }
    return current->prev;
}

/* beginning to end */
void traverseForward(const Node *head) {
    const Node *current = head;
    if (current == NULL) {
        printf("\nQueue is empty.\n");
        return;
    }
    printf("\nSamples in Forward Direction\n");
    while (current != NULL) {
        printf("\nIdentifier: %s\n", current->identifier);
        printf("Sample Type: %s\n", current->sampleType);
        printf("Priority: %d\n", current->priority);
        current = current->next;
    }
    printf("\nReached end of queue.\n");
}

/* end to beginning */
void traverseBackward(const Node *tail) {
    const Node *current = tail;
    if (current == NULL) {
        printf("\nQueue is empty.\n");
        return;
    }
    printf("\nSamples in Backward Direction\n");
    while (current != NULL) {
        printf("\nIdentifier: %s\n", current->identifier);
        printf("Sample Type: %s\n", current->sampleType);
        printf("Priority: %d\n", current->priority);
        current = current->prev;
    }
    printf("\nReached beginning of queue.\n");
}

/* free nodes & nullify head/tail */
void freeList(Node **head, Node **tail) {
    Node *current = *head;
    Node *nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    *head = NULL;
    *tail = NULL;
}

/* read sample from user (allows spaces in strings) */
void readSample(char identifier[], char sampleType[], int *priority) {
    printf("Patient/Sample Identifier: ");
    if (fgets(identifier, MAX_LENGTH, stdin) == NULL) {
        identifier[0] = '\0';
    } else {
        identifier[strcspn(identifier, "\n")] = '\0';
    }
    printf("Sample Type: ");
    if (fgets(sampleType, MAX_LENGTH, stdin) == NULL) {
        sampleType[0] = '\0';
    } else {
        sampleType[strcspn(sampleType, "\n")] = '\0';
    }
    do {
        printf("Priority (1 = Urgent, 2 = Normal, 3 = Routine): ");
        if (scanf("%d", priority) != 1) {
            *priority = 0; // Force invalid value to trigger loop repeat
            while (getchar() != '\n'); // clear invalid non-integers
        }
        if (*priority < 1 || *priority > 3) {
            printf("Invalid priority. Please enter 1, 2, or 3.\n");
        }
    } while (*priority < 1 || *priority > 3);
    while (getchar() != '\n'); /*consume trailing newline*/
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;
    Node *current = NULL;
    int numberOfSamples;
    int priority;
    int choice;
    char identifier[MAX_LENGTH];
    char sampleType[MAX_LENGTH];
    printf("Enter number of initial samples: ");
    if (scanf("%d", &numberOfSamples) != 1 || numberOfSamples < 0) {
        printf("Invalid number. Exiting...\n");
        return 1;
    }
    while (getchar() != '\n');

    /* read & insert initial samples */
    for (int i = 0; i < numberOfSamples; i++) {
        printf("\nEnter sample details %d\n", i + 1);
        readSample(identifier, sampleType, &priority);
        insertAtEnd(&head, &tail, identifier, sampleType, priority);
	if (!insertAtEnd(&head, &tail, identifier, sampleType, priority)) {
            printf("Failed to add sample. Exiting.\n");
            freeList(&head, &tail);
            return 1;
        }
    }
   
    current = head;

    do {
        printf("\n LABORATOTY SAMPLE QUEUE \n");
        printf("1. Move to next sample\n");
        printf("2. Move to previous sample\n");
        printf("3. Display current sample\n");
        printf("4. Add new sample to end\n");
        printf("5. Review all samples forward\n");
        printf("6. Review all samples backward\n");
        printf("7. Exit\n");
        printf("...\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            choice = -1; 
            while (getchar() != '\n'); 
        }

        switch (choice) {
            case 1:
                current = moveNext(current);
                displayCurrent(current);
                break;

            case 2:
                current = movePrevious(current);
                displayCurrent(current);
                break;

            case 3:
                displayCurrent(current);
                break;

            case 4:
                printf("\nEnter new sample details\n");
                while (getchar() != '\n');
                readSample(identifier, sampleType, &priority);
                if (insertAtEnd(&head, &tail, identifier, sampleType, priority)){
                    if (current == NULL) {
                        current = head;
                    }
                    printf("\nNew sample added successfully.\n");
                } else {
        	    printf("\nFailed to add new sample.\n");
    		}
		break;

            case 5:
                traverseForward(head);
                break;

            case 6:
                traverseBackward(tail);
                break;

            case 7:
                printf("\nExiting the laboratory sample queue.\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 7);
    /* free allocated memory */
    freeList(&head, &tail);
    current = NULL;
    printf("Memory successfully released.\n");

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

static Node *createNode(int id) {
    Node *node;
    node = malloc(sizeof *node);
    if (node == NULL) {
        return NULL; /* caller handles failure */
    }
    node->id = id;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *buildTreeFromArray(const int values[], int size, int *success) {
    Node **nodes;
    Node *root;
    int i;
    int leftIdx;
    int rightIdx;

    if (success == NULL) return NULL;
    *success = 0;

    if (values == NULL || size <= 0) return NULL;

    /* array of node pointers for level-order linking */
    nodes = malloc(sizeof *nodes * (size_t)size);
    if (nodes == NULL) {
        return NULL;
    }

    /* create nodes first */
    for (i = 0; i < size; i++) {
        nodes[i] = createNode(values[i]);
        if (nodes[i] == NULL) {
            /* free already allocated nodes on failure */
            int j;
            for (j = 0; j < i; j++) {
                freeTree(nodes[j]);
            }
            free(nodes);
            return NULL;
        }
    }

    /* wire left/right children (index arithmetic) */
    for (i = 0; i < size; i++) {
        leftIdx = 2 * i + 1;
        rightIdx = 2 * i + 2;
        if (leftIdx < size) {
            nodes[i]->left = nodes[leftIdx];
        }
        if (rightIdx < size) {
            nodes[i]->right = nodes[rightIdx];
        }
    }

    root = nodes[0];
    free(nodes); /* free pointer array, not the nodes */
    *success = 1;
    return root;
}

Node *insertNode(Node *root, int id) {
    Node **queue;
    int capacity;
    int head;
    int tail;
    Node *node;
    Node *newNode;

    if (root == NULL) {
        /* new node becomes root in empty tree */
        return createNode(id);
    }

    /* BFS with dynamic array as a queue */
    capacity = 16;
    queue = malloc(sizeof *queue * (size_t)capacity);
    if (queue == NULL) {
        return NULL;
    }

    head = 0;
    tail = 0;
    queue[tail++] = root;
    while (head < tail) {
        node = queue[head++];
        if (node->left == NULL) {
            newNode = createNode(id);
            if (newNode == NULL) {
                free(queue);
                return NULL;
            }
            node->left = newNode;
            free(queue);
            return root;
        } else {
            queue[tail++] = node->left;
            if (tail > capacity) {
                free(queue);
                return root;
            }
        }
        if (node->right == NULL) {
            newNode = createNode(id);
            if (newNode == NULL) {
                free(queue);
                return NULL;
            }
            node->right = newNode;
            free(queue);
            return root;
        } else {
            queue[tail++] = node->right;
            if (tail > capacity) {
                free(queue);
                return root;
            }
        }
    }
    free(queue);
    return root;
}

/* search by id */
Node *searchNode(Node *root, int id) {
    Node *found;
    if (root == NULL) return NULL;
    if (root->id == id) return root;
    found = searchNode(root->left, id);
    if (found != NULL) return found;
    return searchNode(root->right, id);
}

/* track parent */ 
static Node *searchWithParentRecursive(Node *root, int id, Node *parent, Node **parentOut) {
    Node *found;
    if (root == NULL) return NULL;
    if (root->id == id) {
        *parentOut = parent;
        return root;
    }
    found = searchWithParentRecursive(root->left, id, root, parentOut);
    if (found != NULL) return found;
    return searchWithParentRecursive(root->right, id, root, parentOut);
}

/* search for node and also return its parent (if any) */
Node *searchWithParent(Node *root, int id, Node **parentOut) {
    if (parentOut == NULL) return NULL;
    *parentOut = NULL;
    return searchWithParentRecursive(root, id, NULL, parentOut);
}

void displayRoot(Node *root) {
    if (root == NULL) {
        printf("Tree is empty; no root exists.\n");
        return;
    }
    printf("Root identifier: %d\n", root->id);
}

void displayLeaves(Node *root) {
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL) {
        printf("%d ", root->id);
        return;
    }
    displayLeaves(root->left);
    displayLeaves(root->right);
}

/* present parent information of specified ID */
void displayParent(Node *root, int id) {
    Node *parent;
    Node *node;
    parent = NULL;
    node = searchWithParent(root, id, &parent);
    if (node == NULL) {
        printf("Identifier %d does not exist in tree.\n", id);
    } else if (parent == NULL) {
        printf("Root node %d has no parent.\n", id);
    } else {
        printf("Parent of node %d: %d\n", id, parent->id);
    }
}

void displaySibling(Node *root, int id) {
    Node *parent;
    Node *node;
    Node *sibling;
    parent = NULL;
    node = searchWithParent(root, id, &parent);
    if (node == NULL) {
        printf("Identifier %d does not exist in tree.\n", id);
        return;
    }
    if (parent == NULL) {
        printf("Root node %d has no sibling.\n", id);
        return;
    }
    sibling = (parent->left == node) ? parent->right : parent->left;
    if (sibling == NULL) {
        printf("Node %d has no sibling.\n", id);
    } else {
        printf("Sibling of node %d: %d\n", id, sibling->id);
    }
}

void displayGrandchildren(Node *root, int id) {
    Node *node;
    Node *children[2];
    int found;
    int i;
    node = searchNode(root, id);
    if (node == NULL) {
        printf("Identifier %d does not exist in tree.\n", id);
        return;
    }
    children[0] = node->left;
    children[1] = node->right;
    found = 0;
    for (i = 0; i < 2; i++) {
        if (children[i] != NULL) {
            if (children[i]->left != NULL) {
                if (!found) printf("Grandchildren: ");
                printf("%d ", children[i]->left->id);
                found = 1;
            }
            if (children[i]->right != NULL) {
                if (!found) printf("Grandchildren: ");
                printf("%d ", children[i]->right->id);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("Node %d has no grandchildren.\n", id);
    } else {
        printf("\n");
    }
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    return ch;
}

/* menu */
void displayMenu(void) {
    printf("\n E-SPORTS TOURNAMENT TREE SYSTEM \n");
    printf("1. Display root identifier\n");
    printf("2. Display all leaf participants\n");
    printf("3. Display parent of an identifier\n");
    printf("4. Display sibling of an identifier\n");
    printf("5. Display grandchildren of an identifier\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    int values[] = {
        42, 17, 68, 9, 23, 55, 81, 4, 13, 20, 31,
        49, 61, 75, 90, 2, 7, 11, 15, 19, 21, 27,
        35, 45, 52, 58, 64, 72, 78, 85, 95, 1, 3,
        5, 6, 8, 10, 12, 14, 16, 18, 22, 24, 26, 29,
        33, 37, 41, 44, 47, 50, 54, 57, 60, 63, 66,
        70, 74, 77, 80, 83, 87, 92, 97, 25, 28, 30,
        34, 39
    };
    int size;
    int success;
    Node *root;
    int choice;
    int id;
    int scanResult;
    size = (int)(sizeof values / sizeof values[0]);
    success = 0;
    root = buildTreeFromArray(values, size, &success);
    if (!success || root == NULL) {
        printf("Failed to construct tournament tree.\n");
        return 0;
    }
    choice = 0;
    id = 0;
    do {
        displayMenu();
        scanResult = scanf("%d", &choice);
        if (scanResult == EOF) {
            printf("\nStream broken. Exiting system...\n");
            break;
        } else if (scanResult != 1) {
            printf("Invalid choice. Please enter an integer (1-6).\n");
            if (clearInputBuffer() == EOF) break;
            continue;
        }
        switch (choice) {
            case 1:
                displayRoot(root);
                break;
            case 2:
                printf("Leaf participants: ");
                displayLeaves(root);
                printf("\n");
                break;
            case 3:
                printf("Enter participant or match identifier: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid identifier. Please enter an integer.\n");
                    if (clearInputBuffer() == EOF) choice = 6;
                } else {
                    displayParent(root, id);
                }
                break;
            case 4:
                printf("Enter participant or match identifier: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid identifier. Please enter an integer.\n");
                    if (clearInputBuffer() == EOF) choice = 6;
                } else {
                    displaySibling(root, id);
                }
                break;
            case 5:
                printf("Enter participant or match identifier: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid identifier. Please enter an integer.\n");
                    if (clearInputBuffer() == EOF) choice = 6;
                } else {
                    displayGrandchildren(root, id);
                }
                break;
            case 6:
                printf("Exiting tournament tree system...\n");
                break;
            default:
                printf("Invalid choice. Please select 1-6.\n");
                break;
        }
    } while (choice != 6);
    freeTree(root);
    return 0;
}


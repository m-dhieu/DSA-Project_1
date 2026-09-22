#ifndef BINARY_TREE_H
#define BINARY_TREE_H

/**
 * Binary tree node for the tournament participant tree.
 *
 * The 68 given identifiers are stored directly in these nodes.
 * The tree is constructed in level order from the array:
 *   For index i (0-based):
 *     left child  -> 2*i + 1 (if < size)
 *     right child -> 2*i + 2 (if < size)
 *
 * This yields a complete (or nearly complete) binary tree.
 */
typedef struct Node {
    int id;                  /* participant/match identifier */
    struct Node *left;       /* left child pointer */
    struct Node *right;      /* right child pointer */
} Node;

/* tree construction */
Node *buildTreeFromArray(const int values[], int size, int *success);

/* insertion */
Node *insertNode(Node *root, int id);

/* search utilities */
Node *searchNode(Node *root, int id);
Node *searchWithParent(Node *root, int id, Node **parentOut);

/* menu operations */
void displayRoot(Node *root);
void displayLeaves(Node *root);
void displayParent(Node *root, int id);
void displaySibling(Node *root, int id);
void displayGrandchildren(Node *root, int id);

/* memory management & UI helpers */
void freeTree(Node *root);
void displayMenu(void);
int clearInputBuffer(void);

#endif


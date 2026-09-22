# Laboratory Sample Queue

This program models a hospital laboratory sample processing queue using a doubly linked list. Each node represents a blood sample awaiting processing and stores: patient/sample identifier (string), sample type (string), and priority level (integer: 1 = urgent, 2 = normal, 3 = routine).

The program provides a menu-driven interface that allows a laboratory technician to: move to the next/previous sample in the queue, display current sample, add a new sample to the end of the queue while the program is running, and review all samples in forward and backward directions.

The implementation uses dynamic memory allocation (`malloc`/`free`), maintains both `head` and `tail` pointers, and correctly handles edge cases such as an empty queue, a single-sample queue, and navigation at the beginning or end of the list.

---

## Data Structure Design

A doubly linked list is used to represent the processing queue.

```c
typedef struct Node {
    char identifier[MAX_LENGTH];
    char sampleType[MAX_LENGTH];
    int priority;
    struct Node *next;
    struct Node *prev;
} Node;
```

- `identifier`: Patient/sample identifier (string, max 99 characters + null terminator).
- `sampleType`: Type of sample (string, max 99 characters + null terminator).
- `priority`: Priority level (1 = urgent, 2 = normal, 3 = routine).
- `next`: Pointer to the next sample in the queue.
- `prev`: Pointer to the previous sample in the queue.

The list is managed using:

- `Node *head`: Points to the first sample in the queue.
- `Node *tail`: Points to the last sample in the queue.
- `Node *current`: Points to the sample currently being inspected by the technician.

Maintaining both `head` and `tail` allows constant-time insertion at the end of the list.

## Core Functions

### Node Creation

```c
Node *createNode(const char identifier[], const char sampleType[], int priority);
```

- Allocates memory for a new node using `malloc`.
- Copies `identifier` and `sampleType` safely using `strncpy` and ensures null-termination.
- Initializes `priority`, `next`, and `prev`.
- Returns `NULL` if memory allocation fails.

### Insertion at the End

```c
void insertAtEnd(Node **head, Node **tail,
                 const char identifier[], const char sampleType[], int priority);
```

- Creates a new node using `createNode`.
- If the list is empty (`*head == NULL`):
  - Sets `*head = newNode` and `*tail = newNode`.
- If the list is non-empty:
  - Links the new node after the current tail:
    ```c
    newNode->prev = *tail;
    (*tail)->next = newNode;
    *tail = newNode;
    ```
- Runs in O(1) time because the `tail` pointer is maintained.

### Navigation and Display

```c
void displayCurrent(const Node *current);
Node *moveNext(Node *current);
Node *movePrevious(Node *current);
```

- `displayCurrent`:
  - Prints the current sample’s identifier, type, and priority.
  - Handles `current == NULL` (empty queue) gracefully.
- `moveNext`:
  - Moves to `current->next` if it exists.
  - If already at the last sample, prints a message and remains at the current node.
- `movePrevious`:
  - Moves to `current->prev` if it exists.
  - If already at the first sample, prints a message and remains at the current node.

All functions handle the empty-queue case without crashing.

### Full Queue Review

```c
void traverseForward(const Node *head);
void traverseBackward(const Node *tail);
```

- `traverseForward`:
  - Starts at `head` and iterates using `next` until `NULL`.
  - Prints each sample’s details.
- `traverseBackward`:
  - Starts at `tail` and iterates using `prev` until `NULL`.
  - Prints each sample’s details.

Both functions:

- Handle an empty list by printing a message and returning.
- Stop automatically when the end or beginning of the queue is reached.

### User Input

```c
void readSample(char identifier[], char sampleType[], int *priority);
```

- Uses `fgets` to read `identifier` and `sampleType`, allowing spaces in the input.
- Removes trailing newlines using `strcspn`.
- Repeatedly prompts for `priority` until a valid integer in the range 1–3 is entered.
- Clears the input buffer when invalid data is entered to avoid infinite loops or corrupted reads.

### Memory Deallocation

```c
void freeList(Node **head, Node **tail);
```

- Traverses the list from `head` to `NULL`.
- Frees each node using `free`.
- Sets `*head = NULL` and `*tail = NULL` after freeing.

This ensures:

- No memory leaks (every `malloc` is matched by a `free`).
- No dangling pointers (head and tail are nullified after cleanup).

---

## Program Flow

1. Prompt the user for the number of initial samples.
   - Validate that the input is a non-negative integer.
   - Clear the input buffer before reading strings.
2. For each initial sample:
   - Call `readSample` to get identifier, sample type, and priority.
   - Call `insertAtEnd` to add the sample to the doubly linked list.
3. Set `current = head` to start inspection from the first sample.
4. Enter a menu loop:
   - Choose option.
   - Validate menu input; handle invalid entries gracefully.
   - Update `current`, `head`, and `tail` as needed.
5. On exit:
   - Call `freeList(&head, &tail)` to release all dynamically allocated memory.
   - Print a confirmation message and terminate.

---

## Handling of Edge Cases

The program correctly handles:

- **Empty queue**:
  - Navigation and display functions print a message and do not crash.
- **Single-sample queue**:
  - “Next” and “Previous” operations detect first/last status and inform the user.
- **At first sample**:
  - “Previous” operation reports that the technician is already at the first sample.
- **At last sample**:
  - “Next” operation reports that the technician is already at the last sample.
- **Adding a new sample while running**:
  - Works for both empty and non-empty queues.
  - If the queue was empty, the new sample becomes the current sample.
- **Invalid user input**:
  - Non-integer or out-of-range priority values are rejected with a clear message.
  - Invalid menu choices are handled without crashing.

---

## Time Complexity Analysis

Let `n` be the number of samples in the queue.

### Insertion at the End

- The implementation maintains a `tail` pointer.
- To add a new sample:
  - Allocate a new node.
  - Update a constant number of pointers (`prev`, `next`, `tail`).
  - No traversal of existing nodes is required.

**Time complexity:**
- With tail pointer: **O(1)**
- Without tail pointer: **O(n)** (would require traversing from `head` to find the last node)

**Justification:**  
Because the program keeps a direct pointer to the last node, inserting at the end requires only a fixed number of operations regardless of `n`. If `tail` were not maintained, the program would need to visit up to `n` nodes to find the insertion point, resulting in linear time.

### Traversal of All Samples

- Forward traversal visits each node once via `next`.
- Backward traversal visits each node once via `prev`.

**Time complexity:**
- Forward traversal: **O(n)**
- Backward traversal: **O(n)**

**Justification:**  
To review all samples, the program must access each of the `n` nodes exactly once. The number of steps grows linearly with the number of samples.

### Summary Table

| Operation                     | Time Complexity |
|-------------------------------|-----------------|
| Add sample to end (with tail) | O(1)            |
| Add sample to end (no tail)   | O(n)            |
| Forward traversal             | O(n)            |
| Backward traversal            | O(n)            |

---

## Memory Management

- All nodes are allocated dynamically with `malloc`.
- Every allocated node is freed exactly once in `freeList`.
- After freeing:
  - `head` and `tail` are set to `NULL`.
  - No further dereferencing of these pointers occurs.
- No memory leaks or dangling pointers are introduced during program execution.

---

## Compilation and Execution

To compile and run the program:

```bash
gcc -std=c11 -Wall -Wextra -o lab_queue main.c
./lab_queue
```

- `-Wall -Wextra` enable comprehensive warnings.
- The program then prompts for the number of initial samples and runs the menu-driven interface.


# Data Structures and Algorithms Tasks

These tasks present four C programming solutions developed for practical data-processing scenarios. They focus on the design, implementation decisions, algorithm analysis, and evaluation of each solution.

---

## Question 1: E-Commerce Order Priority System

### System Overview

The program reads orders from `orders.txt`, stores them in a dynamically allocated array, sorts them using Quick Sort, and writes the prioritized records to `priority_orders.txt`. If `priority_orders.txt` already exists, it is replaced when the program runs.

Each input line follows the format:
```
OrderID CustomerName ProductCategory OrderValue
```

### Priority Rules

The records are sorted according to the following rules:

1. Order value in descending order.
2. Customer name in ascending lexicographical order when order values are equal.
3. Order ID in ascending lexicographical order when both order value and customer name are equal.

For the supplied example, the expected contents of `priority_orders.txt` are:
```
ORD1031 David Furniture 1250000
ORD1007 Jean Furniture 1250000
ORD1023 Alice Electronics 450000
ORD1045 Eric Electronics 450000
ORD1012 Grace Clothing 85000
```

The terminal displays:
```
Number of orders processed: 5
Total value of all orders: 3485000 RWF
```

### Data Structure and Implementation

Each record is represented using an `Order` structure containing: order ID, customer name, product category, and order value.

The records are stored in a dynamically allocated array. The initial capacity is 10 records. When the array becomes full, its capacity is doubled using `realloc()`.

The implementation uses separate functions for: comparing order records, swapping records, partitioning the array, recursively sorting the array, reading records from the file, writing records to the output file, and calculating the total order value.

### File Processing

The program: opens `orders.txt`, reads each line, validates each record, stores valid records in the dynamic array, calculates the number of records and total value, sorts the records, creates or replaces `priority_orders.txt`, and writes the sorted records.

File-opening errors and memory-allocation failures are handled.

### Quick Sort Complexity Analysis

Let n be the number of orders.

Partitioning a subarray of size n requires a linear scan:
```
P(n) = Theta(n)
```

#### Best Case

The pivot divides the array into two approximately equal parts:
```
T(n) = 2T(n/2) + cn
```

Therefore:
```
T(n) = Theta(nlog n)
```

and:
```
O(nlog n)
```

#### Average Case

For reasonably distributed pivot positions, the expected running time is:
```
Theta(\log n)
```

Therefore:
```
O(nlog n)
```

#### Worst Case

The pivot produces partitions of sizes `n-1` and `0`:
```
T(n) = T(n-1) + cn
```

Expanding the recurrence:
```
T(n) = c(n + (n-1) + (n-2) + ... + 1)
```

Therefore:
```
T(n) = Theta(n^2)
```

and:
```
O(n^2)
```

The implementation chooses the last element as the pivot. Consequently, sorted, reverse-sorted, or specially arranged data can cause the worst-case behavior.

#### Auxiliary Space

The recursive Quick Sort stack requires:

- Balanced or average recursion: `O(log n)`.
- Worst-case recursion: `O(n)`.

Therefore, auxiliary recursion space is:

- Average case: `O(log n)`.
- Worst case: `O(n)`.

The dynamic array itself requires `O(n)` storage. This is required data storage and is separate from Quick Sort's auxiliary recursion space.

Reading, calculating the total, and writing the records each require `O(n)` time.

Total processing requires:

- Average case: `O(nlog n)`.
- Worst case: `O(n^2)`.

### Quick Sort vs Insertion Sort

Insertion Sort has:

- Best-case time: `(O(n)`.
- Average-case time: `O(n^2)`.
- Worst-case time: `O(n^2)`.

Quick Sort has:

- Best-case time: `O(nlog n)`.
- Average-case time: `O(nlog n)`.
- Worst-case time: `O(n^2)`.

Insertion Sort can be suitable for very small or nearly sorted datasets. However, the company expects the number of orders to increase substantially. Quick Sort is generally more scalable for a large, unsorted dataset because its average running time is \(O(n\log n)\).

The records initially exist in a file, but this implementation loads them into memory before sorting. If the dataset becomes too large to fit in memory, an external sorting algorithm such as external merge sort would be more appropriate.

### Boundary Cases

The program is designed to handle: empty input files, single order, duplicate order values or values and customer names, invalid records, file-opening errors, memory-allocation failures, and existing output files.

---

## Question 2: Laboratory Sample Queue

### System Overview

The program models a sample-processing queue using a doubly linked list. Each node represents a sample awaiting processing.

### Data Structure Design

Each node contains: identifier, `sampleType`, priority, and `next` or `prev` pointer.

The list maintains:

- `head`: pointer to the first sample.
- `tail`: pointer to the last sample.
- `current`: pointer to the sample currently being inspected.

The `next` and `prev` links allow navigation in both directions. Maintaining a tail pointer makes end insertion efficient.

### Insertion at the End

For an empty list:

```
head = newNode
tail = newNode
current = newNode
```

For a non-empty list:

```
newNode->prev = tail
tail->next = newNode
tail = newNode
```

This maintains both directions of the doubly linked list correctly.

### Navigation

Moving forward follows the `next` pointer.

Moving backward follows the `prev` pointer.

The program handles: an empty queue, already being at the first/last sample, and a single-sample queue.

The program does not move outside the valid range of nodes.

### Complexity Analysis

Let n be the number of samples.

#### Adding with a Tail Pointer

Only a fixed number of pointer updates are required:
```
O(1)
```

#### Adding without a Tail Pointer

The program would need to traverse from the head to the final node:
```
O(n)
```

#### Navigation

Moving to the next or previous node takes:
```
O(1)
```

#### Traversal

Forward and backward reviews visit every node once:
```
O(n)
```

### Memory Management

All nodes are allocated using `malloc()`.

The `freeList()` function traverses from the head, frees each node exactly once, and sets list pointers to `NULL`. This prevents memory leaks and dangling pointers.

### Boundary Cases

The implementation handles: empty queues, single-node queues, first and last node, adding a sample to an empty queue or while the system is running, invalid priority values, and invalid menu choices.

---

## Question 3: Binary Tournament Tree

### Construction Rule

The program constructs a complete binary tree from the 68 participant identifiers in the order provided.

The identifier values are not compared when determining placement. Instead, identifiers are inserted level by level from left to right using array index mapping.

For a node at index i, using zero-based indexing:

- Left child: `2i + 1`.
- Right child: `2i + 2`.

These children exist if the resulting indices are within the array bounds.

This yields a complete or nearly complete binary tree.

### Node Structure

Each node stores: Participant or match identifier, and right/left child pointer.

Parent relationships are not stored explicitly. Instead, parent and sibling queries are answered by searching from the root while tracking the parent during the search.

### Operations

The program supports: displaying the root identifier/leaf participants/grandchildren, finding the selected identifier's parent/sibling, searching for an identifier, inserting a new participant into the tree, handling nonexistent identifiers, and exiting and freeing all allocated memory.

### Construction Complexity

Each identifier is allocated once and linked once using index arithmetic. Building the initial tree therefore takes:
```
O(n)
```

### Runtime Insertion Complexity

Insertion is implemented using a level-order, or BFS, traversal from the root to find the first node that is missing a left or right child.

In the worst case, this traversal may visit all n existing nodes. Therefore, insertion takes:
```
O(n)
```

### Search and Relationship Queries

Because the tree is not ordered by identifier, searching for a particular identifier may visit all nodes:
```
O(n)
```

Parent, sibling, and grandchild queries each include this search cost. Once the node is found, the actual relationship lookup is \(O(1)\). Therefore, each complete relationship query is `O(n)`.

### Leaf Traversal Complexity

Displaying all leaves requires visiting the tree:
```
O(n)
```

### Memory Management

Every tree node is allocated using `malloc()`.

The `freeTree()` function recursively releases every node before program termination. This prevents memory leaks and dangling pointers.

### Boundary Cases

The program handles: the root having no parent or sibling, a leaf having no children, a node having no sibling or grandchildren, a nonexistent identifier, invalid menu input, invalid participant identifiers, memory-allocation failure, and graceful termination.

---

## Question 4: Pharmacy Inventory BST

### System Overview

The program loads medicine records from `inventory.txt` into a Binary Search Tree. It allows pharmacy staff to validate inventory records, search for medicines by code, display the complete inventory in sorted order, and perform multiple searches without rebuilding the tree.

The file format is:
```
MedicineCode|MedicineName|Quantity|UnitPrice
```

Example:
```
M1001|Paracetamol 500mg|250|50
```

The medicine code is used as the BST key.

### BST Construction

Each node stores: Medicine code/name, Quantity, Unit price, and Left/right child pointer.

The BST ordering property is:

- Smaller medicine codes are stored in the left subtree.
- Larger medicine codes are stored in the right subtree.

Medicine codes are compared lexicographically using `strcmp()`.

### Duplicate Medicine Codes

If a duplicate medicine code is encountered, the program updates the existing node rather than creating a second node.

This implementation uses replacement semantics:
```
existing quantity = latest valid quantity
```

This means a duplicate record is interpreted as a correction or replacement record rather than an additional stock delivery.

### File Validation

The program is designed to handle: blank lines, missing fields, extra fields, malformed records, invalid medicine codes, empty medicine names, negative or invalid quantities, negative or invalid prices, out-of-range numeric values, overlong records, empty files, files containing no valid records, file-opening errors, and memory-allocation errors.

Invalid records are rejected with warnings while valid records continue to be processed.

### Search and Traversal

The user can perform multiple searches without rebuilding the tree.

For a search:

1. The requested code is compared with the current node.
2. The complete record is displayed if the code matches.
3. The left subtree is searched if the requested code is smaller.
4. The right subtree is searched if the requested code is larger.
5. A not-found message is displayed when the search reaches `NULL`.

An in-order traversal displays the inventory in ascending medicine-code order.

### Binary Tree Versus Binary Search Tree

A binary tree is a hierarchical data structure in which each node has at most two children. A binary tree does not require any ordering of the values stored in its nodes.

A binary search tree is a binary tree with an ordering property. For each node:

- All keys in the left subtree are smaller than the node's key.
- All keys in the right subtree are larger than the node's key.

This ordering allows a search to eliminate one subtree at each comparison.

### BST Search Complexity

Let n be the number of unique medicine nodes and h be the height of the BST.

#### Best Case

The requested medicine is at the root:
```
O(1)
```

#### Average Case

If the tree is reasonably balanced, its height is:
```
O(log n)
```

Therefore, search complexity is:
```
O(log n)
```

An ordinary BST does not guarantee this bound.

#### Worst Case

If the tree is completely skewed, its height is:
```
O(n)
```

Therefore, search complexity is:
```
O(n)
```

### Effect of the Supplied File Order

The supplied records are ordered from `M1001` to `M1040`.

If the records are inserted directly into an ordinary BST in that order, every new medicine code is larger than the previous code. This produces a right-skewed tree.

For the supplied file:

- Construction: `O(n^2)`.
- Search: `O(n)`.
- In-order traversal: `O(n)`.

### Linear File Search Versus BST Search

A direct linear file search takes: `O(n)` per query.

For k queries: `O(kn)`.

For a reasonably balanced BST:

- Construction: `O(nlog n)` average.
- Each search: `O(log n)`.
- k searches: `O(klog n)`.
- Total: `O(nlog n + klog n)`.

For a skewed BST:
```
O(n^2 + kn)
```

A BST becomes more beneficial when many searches are performed on the same inventory because the tree is built once and reused. A self-balancing BST would provide more reliable logarithmic performance for sorted input.

### Memory Management

Every medicine node is allocated using `malloc()`.

The `freeTree()` function recursively releases every node before program termination. This prevents memory leaks and dangling pointers.

---

## Overall Complexity Summary

| Operation                            | Best Case        | Average Case | Worst Case  |
|--------------------------------------|-----------------:|-------------:|------------:|
| Quick Sort                           | O(nlog n)        | O(nlog n)    | O(n^2)      |
| Quick Sort recursion space           | O(log n)         | O(log n)     | O(n)        |
| Add sample with tail pointer         | O(1)             | O(1)         | O(1)        |
| Laboratory queue traversal           | O(n)             | O(n)         | O(n)        |
| Initial tournament tree construction | O(n)             | O(n)         | O(n)        |
| Runtime complete-tree insertion      | O(n)             | O(n)         | O(n)        |
| Tournament identifier search         | O(1), root found | O(n)         | O(n)        |
| BST search                           | O(1)             | O(log n)     | O(n)        |
| BST in-order traversal               | O(n)             | O(n)         | O(n)        |
| Linear file search                   | O(1)             | O(n)         | O(n)        |

---

## Summary

The four programs demonstrate how suitable data structures and algorithms can be applied to practical information-processing systems.

Quick Sort is used to prioritize large collections of e-commerce orders. The doubly linked list supports bidirectional laboratory queue navigation and constant-time insertion at the end when a tail pointer is maintained. The complete binary tournament tree supports participant relationship queries and \(O(n)\) insertion using a level-order, or BFS, traversal. The pharmacy BST supports repeated medicine searches without repeatedly scanning the inventory file, although its performance depends on the shape of the tree.

The implementations use modular functions, dynamic memory allocation, input validation, boundary-case handling, and explicit memory deallocation.

---

## AI Assistance Used

AI tools were used to support:

- Debugging and troubleshooting implementation issues.
- Improving documentation structure and clarity.

---

## Resources

The following resources were used for learning and reference during the development of these projects:

- [Jenny's Quick Sort Lectures (YouTube)](https://www.youtube.com/watch?v=trhZ4YJXo28)
- [Abdul Bari DSA (YouTube)](https://www.youtube.com/playlist?list=PLsr8vTgyLdy_YndxNcI4WkH5Vorj5qvrv)
- [GeeksforGeeks Learn DSA in C](https://www.geeksforgeeks.org/c/learn-dsa-in-c/)
- [Opencourseware C Memory Management](https://ocw.mit.edu/courses/6-s096-introduction-to-c-and-c-january-iap-2013/pages/lectures-and-assignments/c-memory-management/)

---

## License

This project is under the MIT License.

---

## Author

Monica Dhieu

---

*Monday, September 21, 2026*

# E-Commerce Order Priority System

A program that reads e-commerce orders from `orders.txt`, sorts them using Quick Sort, and writes the prioritized records to `priority_orders.txt`. `priority_orders.txt` is generated automatically when the program runs. If it already exists, it is replaced with the newly sorted output.

## Input Format

Each line in `orders.txt` contains four whitespace-separated fields:

```
OrderID CustomerName ProductCategory OrderValue
```

Example:

```
ORD1023 Alice Electronics 450000
ORD1007 Jean Furniture 1250000
ORD1045 Eric Electronics 450000
ORD1012 Grace Clothing 85000
ORD1031 David Furniture 1250000
```

This implementation assumes that the three text fields do not contain spaces.

## Priority Rules

Orders are sorted by:

1. Order value, descending.
2. Customer name, ascending lexicographical order, when values are equal.
3. OrderID, ascending lexicographical order, when both value and customer name are equal.

## Compilation and Execution

Using GCC on Linux or macOS:

```bash
gcc -Wall -Wextra -std=c11 main.c -o order_priority ./order_priority
```

Using MinGW on Windows:

```bash
gcc -Wall -Wextra -std=c11 main.c -o order_priority.exe order_priority.exe
```

The program must be run from the directory containing `orders.txt`.

## Expected Output

![Expected Output](output.jpg)

## Implementation Details

The program stores each order in an `Order` structure and stores all records in a dynamically allocated array. The array starts with a capacity of 10 records and doubles with `realloc()` when it becomes full.

Quick Sort is implemented using the following functions:

- `compareOrders()` applies the three priority rules.
- `swap()` exchanges two order records.
- `partition()` places the pivot in its correct position.
- `quickSort()` recursively sorts the array.
- `readOrders()` reads and validates input records.
- `writeOrders()` creates the sorted output file.
- `calculateTotal()` calculates the total order value.

The program does not use the C library sorting function `qsort()`.

## Complexity Analysis

Let `n` be the number of orders. Partitioning a subarray of size `n` takes `Theta(n)` time.

### Best case

The pivot divides the array into two approximately equal parts:

```
T(n) = 2T(n/2) + cn
```

This resolves to:

```
Theta(n log n) = O(n log n)
```

### Average case

The pivot is expected to produce reasonably balanced partitions. 

The recurrence can be represented as:

```
T(n) = T(k) + T(n-k-1) + cn
```

where k represents the size of one partition. Averaging over possible pivot positions gives a recurrence whose solution is:

```
T(n) = Theta(n log n)
```

Therefore, the average-case running time is:

```
O(n log n)
```

### Worst case

The pivot produces partitions of sizes `n - 1` and `0`:

```
T(n) = T(n - 1) + cn
```

Therefore:

```
Theta(n^2) = O(n^2)
```

Because this implementation chooses the last element as the pivot, sorted, reverse-sorted, or specially arranged input can produce the worst case.

### Auxiliary space

The recursive auxiliary space is:

- Balanced or average recursion: `O(log n)`.
- Worst-case recursion: `O(n)`.

The dynamically allocated array containing the input records requires `O(n)` storage. This is the required data storage and is separate from Quick Sort's recursive auxiliary space.

Reading, calculating the total, and writing the records each take `O(n)` time, assuming each record has bounded-size fields, like the implementation does (char[20], char[50], etc.). Therefore, the overall processing time is `O(n log n)` on average and `O(n^2)` in the worst case.

## Quick Sort Compared with Insertion Sort

Insertion Sort has `O(n)` best-case time when the data is already sorted, but it has `O(n^2)` average and worst-case time. It can be suitable for very small or nearly sorted datasets.

Quick Sort has `O(n log n)` best-case and average-case time, making it generally more scalable for a large, unsorted collection of orders. Its worst-case time is also `O(n^2)` when the pivot repeatedly creates unbalanced partitions.

The input begins in a file, but this program first reads all records into memory and then sorts the in-memory array. If the dataset eventually becomes too large to fit in memory, an external sorting algorithm such as external merge sort would be more appropriate.

## Boundary Cases

The program handles an empty input file, one order, duplicate values, duplicate values and customer names, invalid records, file errors, and memory allocation failures.


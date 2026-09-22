#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define LINE_SIZE 256

typedef struct
{
    char orderID[20];
    char customerName[50];
    char productCategory[50];
    long long orderValue;
} Order;

int compareOrders(const Order *a, const Order *b)
{
    if (a->orderValue > b->orderValue)
    {
        return -1;
    }

    if (a->orderValue < b->orderValue)
    {
        return 1;
    }

    int result = strcmp(a->customerName, b->customerName);

    if (result != 0)
    {
        return result;
    }

    return strcmp(a->orderID, b->orderID);
}

void swap(Order *a, Order *b)
{
    Order temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Order orders[], int low, int high)
{
    Order pivot = orders[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (compareOrders(&orders[j], &pivot) <= 0)
        {
            i++;
            swap(&orders[i], &orders[j]);
        }
    }

    swap(&orders[i + 1], &orders[high]);
    return i + 1;
}

void quickSort(Order orders[], int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(orders, low, high);
        quickSort(orders, low, pivotIndex - 1);
        quickSort(orders, pivotIndex + 1, high);
    }
}

int readOrders(Order **orders)
{
    FILE *file = fopen("orders.txt", "r");

    if (file == NULL)
    {
        printf("Error: Couldn't open orders.txt\n");
        return -1;
    }

    int capacity = INITIAL_CAPACITY;
    int count = 0;

    *orders = malloc((size_t)capacity * sizeof(Order));

    if (*orders == NULL)
    {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return -1;
    }

    char line[LINE_SIZE];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        lineNumber++;
        Order order;

        int result = sscanf(
            line,
            "%19s %49s %49s %lld",
            order.orderID,
            order.customerName,
            order.productCategory,
            &order.orderValue
        );

        if (result == EOF)
        {
            continue;
        }

        if (result != 4)
        {
            printf("Error: Invalid order format on line %d.\n", lineNumber);
            free(*orders);
            fclose(file);
            return -1;
        }

        char extra;
        int extraResult = sscanf(
            line,
            "%19s %49s %49s %lld %c",
            order.orderID,
            order.customerName,
            order.productCategory,
            &order.orderValue,
            &extra
        );

        if (extraResult != 4)
        {
            printf("Error: Invalid order format on line %d.\n", lineNumber);
            free(*orders);
            fclose(file);
            return -1;
        }

        if (count == capacity)
        {
            capacity *= 2;
            Order *temp = realloc(*orders, (size_t)capacity * sizeof(Order));

            if (temp == NULL)
            {
                printf("Error: Memory allocation failed\n");
                free(*orders);
                fclose(file);
                return -1;
            }

            *orders = temp;
        }

        (*orders)[count] = order;
        count++;
    }

    fclose(file);
    return count;
}

int writeOrders(const Order orders[], int count)
{
    FILE *file = fopen("priority_orders.txt", "w");

    if (file == NULL)
    {
        printf("Error: Could not create priority_orders.txt\n");
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        if (fprintf(
                file,
                "%s %s %s %lld\n",
                orders[i].orderID,
                orders[i].customerName,
                orders[i].productCategory,
                orders[i].orderValue
            ) < 0)
        {
            printf("Error: Failed to write to priority_orders.txt\n");
            fclose(file);
            return 0;
        }
    }

    if (fclose(file) != 0)
    {
        printf("Error: Failed to close priority_orders.txt\n");
        return 0;
    }

    return 1;
}

long long calculateTotal(const Order orders[], int count)
{
    long long total = 0;

    for (int i = 0; i < count; i++)
    {
        total += orders[i].orderValue;
    }

    return total;
}

int main(void)
{
    Order *orders = NULL;
    int count = readOrders(&orders);

    if (count == -1)
    {
        return 1;
    }

    if (count > 1)
    {
        quickSort(orders, 0, count - 1);
    }

    if (!writeOrders(orders, count))
    {
        free(orders);
        return 1;
    }

    printf("Number of orders processed: %d\n", count);
    printf("Total value of all orders: %lld RWF\n", calculateTotal(orders, count));

    free(orders);
    return 0;
}


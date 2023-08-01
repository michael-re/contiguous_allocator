#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "array.h"

#undef max
#undef min

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct array array;

array *new_array(size_t n, int fill)
{
    if (!n) return NULL;

    array *arr = (array *)calloc(1, sizeof(*arr));
    if (!arr)
    {
        fprintf(stderr, "Error: failed allocating memory for array\n");
        return NULL;
    }

    arr->data = (int *)calloc(n + 1, sizeof(*arr->data));
    if (!arr->data)
    {
        fprintf(stderr, "Error: failed allocating memory for array elements.\n");
        free(arr);
        return NULL;
    }

    arr->size = n;
    set_values(&arr, fill, 0, n);
    return arr;
}

array *copy_array(const array *source, size_t n)
{
    if (!source || !source->size || !n) return NULL;

    array *copy = new_array(n, 0);
    for (size_t i = 0; copy && i < copy->size && i < source->size; i++)
    {
        copy->data[i] = source->data[i];
    }
    return copy;
}

void free_array(array *source)
{
    if (!source) return;
    free(source->data);
    free(source);
    source = NULL;
}

int64_t array_contains(const array *source, int value)
{
    if (!source || source->size == 0) return -1;
    for (int64_t i = 0; i < (int64_t) source->size; i++)
    {
        if (source->data[i] == value) return i;
    }
    return -1;
}

int compare_array(const array *a, const array *b)
{
    if ((!a && b) || (a && !b)) return 0;
    if (!a && !b)               return 1;
    if (a->size != b->size)     return 0;
    if (!a->size)               return 1;

    for (size_t i = 0; i < a->size; i++)
    {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

size_t contiguous_count(const array *source, int value, size_t i)
{
    int count = 0;
    if (!source) return count;
    while (i < source->size)
    {
        if (source->data[i++] == value) count++;
        else                            break;
    }
    return count;
}

int find_largest(const array *source)
{
    int largest = INT_MIN;

    if (!source) return largest;
    for (size_t i = 0; i < source->size; i++)
    {
        largest = max(largest, source->data[i]);
    }
    return largest;
}

int find_smallest(const array *source)
{
    int smallest = INT_MAX;

    if (!source) return smallest;
    for (size_t i = 0; i < source->size; i++)
    {
        smallest = min(smallest, source->data[i]);
    }
    return smallest;
}

void set_values(array **source, int value, size_t start, size_t end)
{
    while (source && *source && start < end)
    {
        (*source)->data[start++] = value;
    }
}

#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include <ctype.h>
#include <stdint.h>

struct array
{
    int    *data;
    size_t  size;
};

/**
 * @brief Create a new array of size <n> and set all values to <fill>. If this
 *        function fails to create a new array, it will return a null pointer.
 *        Failure can arise from the following scenarios:
 *          * n is 0 - no point in allocating memory.
 *          * Failed to allocate memory for array struct.
 *          * Failed to allocate memory for data in array struct.
 *
 * @param n       - The number of elements array struct will hold.
 * @param fill    - Default value used to fill array.
 * @return array* - Pointer to an array of size <n>.
 */
struct array *new_array(size_t n, int fill);

/**
 * @brief Create a new array of size <n> and copy the first <n> elements from
 *        source into the new array. If <n> is bigger than source, after all
 *        values have been copied from source, the rest will be filled with 0.
 *        If this function fails to create a copy of the given array, it will
 *        return a null pointer. Failure can arise from the following scenarios:
 *          * n is 0.
 *          * Source is null.
 *          * Source is empty.
 *          * Failed to allocate memory for copy array.
 *          * Failed to allocate memory for copy array elements.
 *
 * @param source  - Array struct being copied.
 * @param n       - The first number of elements to copy from source array.
 * @return array* - Pointer to an array with a copy of <n> elements from source.
 */
struct array *copy_array(const struct array *source, size_t n);

/**
 * @brief Free all allocated memory for array struct and set the pointer to
 *        null.
 */
void free_array(struct array *source);

/**
 * @brief Searches source array for the given value and return the index of the
 *        first matching value. If no element in the array matches the value,
 *       -1 is returned.
 *
 * @param source - Array struct containing elements to search.
 * @param value  - Value to search for in the array.
 * @return int   - The index of the element in the source array.
 */
int64_t array_contains(const struct array *source, int value);

/**
 * @brief Compare the two given array struct and return if they both hold the
 *        same elements.
 *
 * @param a    - First array to compare.
 * @param b    - Second array to compare.
 * @return int - 1 if they hold the same elements, 0 otherwise.
 */
int compare_array(const struct array *a, const struct array *b);

/**
 * @brief Counts the number of elements that appear consecutively that match the
 *        given value starting from the specified index.
 *
 * @param source - Array struct to search.
 * @param value  - Value to count.
 * @param i      - Starting index to start count from.
 * @return int   - The number of elements that appears consecutively from the
 *                 starting index that match the given value.
 */
size_t contiguous_count(const struct array *source, int value, size_t i);

/**
 * @brief Find the largest element in the source array. If the array is empty,
 *        this will return INT_MIN.
 *
 * @param source - Array struct to search.
 * @return int   - The largest value found in the array.
 */
int find_largest(const struct array *source);

/**
 * @brief Find the smallest element in the source array. If the array is empty,
 *        this will return INT_MAX.
 *
 * @param source - Array struct to search.
 * @return int   - The smallest value found in the array.
 */
int find_smallest(const struct array *source);

/**
 * @brief Set the values of the source array in the range from start to end to
 *        the provided value.
 *
 * @param source - Array struct containing elements to set values.
 * @param value  - New value to update array struct with.
 * @param start  - Index of the first element to set.
 * @param end    - Index of the last element to set.
 */
void set_values(struct array **source, int value, size_t start, size_t end);

#endif // !ARRAY_H

#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#define CLI_WIDTH    80
#define UNALLOCATED '.'
#define FIRST_FIT   'F'
#define BEST_FIT    'B'
#define WORST_FIT   'W'

typedef struct array memory;

/**
 * @brief Create a new memory pool of size <n>.
 *
 * @param size       - The number of bytes held by the memory pool.
 * @return memory_t* - Pointer to the memory struct of size n with all values
 *                     set to UNALLOCATED.
 */
memory *new_memory_pool(size_t size);

/**
 * @brief Frees all allocated memory for memory pool.
 */
void free_memory_pool(memory *pool);

/**
 * @brief Allocate N bytes for a process using one of the 3 allocation
 *        algorithms: FIRST_FIT, BEST_FIT, WORST_FIT.
 *              * best fit:  Allocate the smallest hole that is big enough.
 *              * first fit: Allocate the first hole that is big enough.
 *              * worst fit: Allocate the largest hole.
 *
 * @param pool     - Memory pool to make allocation in.
 * @param process  - Name of processes requesting memory allocation.
 * @param size     - The number of bytes being request for allocation.
 * @param type     - The type of allocation algorithm to use.
 */
void allocate(memory *pool, char process, size_t size, char type);

/**
 * @brief Frees all allocation for a given process in the given memory pool.
 *
 * @param pool    - Memory pool holding allocated memory for process to
 *                  deallocate.
 * @param process - Name of process holding allocated memory to free.
 */
void deallocate(memory *pool, char process);

/**
 * @brief Compacts the allocated memory into one contiguous block by sliding all
 *        allocations to lower addresses. After the memory pool has been
 *        compacted, all the free space lies to the right as one contiguous
 *        block. (This somewhat resembles the operation of a mark-sweep garbage
 *        collector in C#).
 *
 * @param pool - Memory pool to compact.
 */
void compact_memory(memory *pool);

/**
 * @brief Prints the state of the memory pool by showing all allocated and
 *        unallocated blocks:
 *          * e.g - AAAAA......BBBBBBBBBB..EEEEEE.XX.Z.........
 *
 *        Each byte of memory is printed as either unallocated memory, showed as
 *        '.' character, or as an allocated byte of memory belonging to a
 *        process. For the example above, AAAAA represents all allocated blocks
 *        for process A, BBBBBBBBBB represents all allocated blocks for process
 *        B and so on.
 *
 * @note  After printing all memory blocks, this will also prints the list of
 *        memory address for each process by calling show_stat function.
 *
 * @param pool - Memory pool to print.
 */
void show_allocation(const memory *pool);

/**
 * @brief Prints the state of the memory pool as a list of address:
 *        e.g - addresses [0:199] - process A.
 *
 * @param pool - Memory pool to print list of addresses.
 */
void show_stat(const memory *pool);

/**
 * @brief Create a list containing the starting address of unallocated
 *        memory that is at least size n and returns it as an array
 *        struct.
 *
 * @param pool     - Memory pool to search for unallocated space.
 * @param size     - The minimum size of unallocated memory to look for.
 * @return memory* - An array pointer containing a list of address that point
 *                   to unallocated memory that is at list size big.
 */
memory *unallocated_pools(const memory *pool, size_t size);

/**
 * @brief Find the first unallocated memory block in the given memory pool that
 *        is at least size big and returns its starting address.
 *
 * @param pool - Memory pool to search for unallocated memory.
 * @param size - The minimum size of memory block to search for.
 * @return int - Starting address of the first unallocated memory block that is
 *               at least size big.
 */
int first_fit(const memory *pool, size_t size);

/**
 * @brief Find unallocated hole using best or worst fit algorithm in the memory
 *        pool that is at least n bytes big and returns its starting address.
 *        Keep in mind, this function only checks if alloc type matches best
 *        fit. Any other alloc type will result in worst fit being used.
 *
 * @param pool - Memory pool to search for unallocated memory.
 * @param size - The minimum size of unallocated memory to search for,
 * @param type - The allocating algorithm to use to find unallocated memory. By
 *               default, best fit is used, anything else will result in worst
 *               fit being used to find the memory block.
 * @return int - The starting address of the unallocated memory found using the
 *               specified allocation algorithm.
 */
int best_or_worst_fit(const memory *pool, size_t size, char type);

/**
 * @brief Simple utility function to read a line up to <n> characters from the
 *        given stream.
 *
 * @param n      - The maximum number of characters to read from stream.
 * @param stream - Stream to read line from.
 * @return char* - Pointer to buffer containing input read from stdin.
 */
char *read_line(size_t n, FILE *stream);

/**
 * @brief Reads a sequence of commands from a file, line by line, and executes
 *        them one by one. If it encounters exit command, it will immediately
 *        stop processing the script and return. The return value of this
 *        function indicates the current state of the program after executing
 *        the script: 1 for run and 0 for stop.
 *
 * @param script - File to read.
 * @param pool   - Memory pool to operate on.
 * @return int   - 0 if exit was executed and 1 if not.
 */
int read_script(const char *script, memory *pool);

/**
 * @brief Parses command from the given line. If it is valid, the command is
 *        executed and returns the current state of the program: run or stop.
 *
 * @param line - String that represent line to process.
 * @param pool - Memory pool to operate on.
 * @return int - 1 for run and 0 for stop.
 */
int process_line(char *line, memory *pool);

#endif // !MEMORY_H

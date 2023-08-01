#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "array.h"

memory *new_memory_pool(size_t size)
{
    memory *pool = new_array(size, UNALLOCATED);
    if (!pool)
    {
        fprintf(stderr, "Error creating memory pool. Terminating program.\n");
        exit(EXIT_FAILURE);
    }
    return pool;
}

void free_memory_pool(memory *pool)
{
    free_array(pool);
}

void allocate(memory *pool, char process, size_t size, char type)
{
    if (!size || !pool || size > pool->size) return;
    const int address = (type == FIRST_FIT) ? first_fit(pool, size) :
                                              best_or_worst_fit(pool, size, type);

    // only allocate memory if a valid address is returned
    if (address >= 0 && address < pool->size)
        set_values(&pool, process, address, (size_t)address + size);
}

void deallocate(memory *pool, char process)
{
    for (size_t i = 0; pool && i < pool->size; i++)
    {
        if (pool->data[i] == process)
            pool->data[i] = UNALLOCATED;
    }
}

void compact_memory(memory *pool)
{
    if (!pool) return;

    memory *copy = NULL;
    while (!compare_array(pool, copy))
    {
        free_array(copy);
        copy = copy_array(pool, pool->size);

        for (size_t i = 1; i < pool->size; i++)
        {
            if (pool->data[i - 1] == UNALLOCATED)
            {
                pool->data[i - 1] = pool->data[i];
                pool->data[i]     = UNALLOCATED;
            }
        }
    }
    free_array(copy);
}

memory *unallocated_pools(const memory *pool, size_t size)
{
    memory *temp  = new_array(pool->size, 0);
    size_t  count = 0;

    for (size_t i = 0; pool && i < pool->size; i++)
    {
        const size_t hole = contiguous_count(pool, UNALLOCATED, i);
        if (hole >= size)
        {
            temp->data[count++] = (int) i;
            i += hole - 1;
        }
    }

    // trim to size
    memory *unallocated = copy_array(temp, count);
    free_array(temp);
    return unallocated;
}

void show_allocation(const memory *pool)
{
    for (size_t i = 0, j = 0; pool && i < pool->size; i++)
    {
        printf("%c", (char) pool->data[i]);
        if (!(++j % (CLI_WIDTH))) printf("\n");
    }
    printf("\n");
}

void show_stat(const memory *pool)
{
    memory *copy = copy_array(pool, pool->size + 1);
    for (size_t i = 0, start = 0; i < copy->size; i++)
    {
        if (copy->data[start] != copy->data[i])
        {
            const char process = copy->data[start];
            if (process == UNALLOCATED)
            {
                printf("\taddresses [%zd:%d] - unallocated\n", start, (int)(i - 1));
            }
            else
            {
                printf("\taddresses [%zd:%d] - process %c\n", start, (int)(i - 1), process);
            }
            start = i;
        }
    }
    free_array(copy);
    printf("\n");
}

int first_fit(const memory *pool, size_t size)
{
    memory *holes  = unallocated_pools(pool, size);
    const int hole = holes ? holes->data[0] : -1;
    free_array(holes);
    return hole;
}

int best_or_worst_fit(const memory *pool, size_t size, char type)
{
    memory *holes  = unallocated_pools(pool, size);
    if (!holes)  return -1;

    memory *h_size = copy_array(holes, holes->size);
    if (!h_size) return -1;

    for (size_t i = 0; i < h_size->size; i++)
        h_size->data[i] = (int) contiguous_count(pool, UNALLOCATED, holes->data[i]);

    const int value = (type == BEST_FIT) ? find_smallest(h_size) : find_largest(h_size);
    const int index = (int) array_contains(h_size, value);
    const int hole  = (index != -1) ? holes->data[index] : -1;

    free_array(holes);
    free_array(h_size);
    return hole;
}

char *read_line(size_t n, FILE *stream)
{
    if (!stream || !n) return NULL;

    char *string = calloc(n + 1, sizeof(char));
    if (!string) return string;

    int current = fgetc(stream);
    if (current == EOF) return string;

    size_t read_count = 0;
    while ((current != EOF) && (read_count < n))
    {
        if (current == '\n') break;
        string[read_count++] = current;
        current = fgetc(stream);
    }

    if (!strlen(string))
    {
        free(string);
        string = NULL;
    }
    return string;
}

int read_script(const char *file_name, memory *pool)
{
    FILE *script = fopen(file_name, "r");
    if (script == NULL) return 1; // file does not exist

    char  *line = read_line(CLI_WIDTH, script);
    int    run  = 1;

    while (run && line)    // read each line in file and execute command
    {
        run = process_line(line, pool);
        free(line);
        line = read_line(CLI_WIDTH, script);
    }

    free(line);
    fclose(script);
    return run;
}

int process_line(char *line, memory *pool)
{
    char *command = strtok(line, " ");
    char *process = NULL;
    char *script  = NULL;
    char *alloc   = NULL;
    char *size    = NULL;

    switch (command[0])
    {
        case 'A':
            if (!(process = strtok(NULL, " "))) break; // invalid process name
            if (!(size    = strtok(NULL, " "))) break; // invalid allocation size
            if (!(alloc   = strtok(NULL, " "))) break; // invalid allocation type
            allocate(pool, *process, strtoll(size, NULL, 10), *alloc);
            break;
        case 'F':
            if (!(process = strtok(NULL, " "))) break; // valid process name
            deallocate(pool, *process);
            break;
        case 'S':
            if (!strcmp(command, "STAT")) show_stat(pool);
            else                          show_allocation(pool);
            break;
        case 'R':
            if (!(script = strtok(NULL, " "))) break; // valid script file
            return read_script(script, pool);
            break;
        case 'C':
            if (!strcmp(command, "CLEAR")) printf("\33c\e[3J");
            else                           compact_memory(pool);
            break;
        case 'E':
            return 0;
            break;
        default:
            break;
    }
    return 1;
}

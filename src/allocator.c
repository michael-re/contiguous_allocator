#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: invalid args supplied.\n\n");
        printf("\tusage: ./allocator.out <MEMORY POOL SIZE>\n\n");
        return EXIT_FAILURE;
    }

    const int size = atoi(argv[1]);
    if (!size) return EXIT_FAILURE;

    memory *pool = new_memory_pool(size);
    char   *line = NULL;
    int     run  = 1;

    while (run)
    {
        printf("allocator> ");
        line = read_line(CLI_WIDTH, stdin);
        run  = process_line(line, pool);

        free(line);
        line = NULL;
    }

    free_memory_pool(pool);
    return EXIT_SUCCESS;
}

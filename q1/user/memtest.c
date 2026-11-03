#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    int size = 3 * 4096;

    printf("Initial memory statistics:\n");
    memstats();

    printf("\nAllocating memory...\n");

    char *p = sbrk(size);

    if(p == (char*)-1) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // Touch each page to ensure it is allocated.
    for(int i = 0; i < size; i += 4096) {
        p[i] = 'A';
    }

    printf("\nAfter allocation:\n");
    memstats();

    printf("\nReleasing memory...\n");

    if(sbrk(-size) == (char*)-1) {
        printf("Memory release failed\n");
        exit(1);
    }

    printf("\nAfter deallocation:\n");
    memstats();

    exit(0);
}

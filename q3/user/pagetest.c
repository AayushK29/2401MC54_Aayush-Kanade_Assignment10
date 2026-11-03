#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    printf("Page Protection Test\n");

    char *p = sbrk(4096);

    if(p == (char*)-1) {
        printf("Allocation failed\n");
        exit(1);
    }

    // Initialize the allocated page.
    p[0] = 'A';

    uint64 va = (uint64)p;

    printf("\nBefore changing permissions:\n");

    pageinfo(va);

    printf("\nChanging page to read-only:\n");

    if(setreadonly(va) < 0) {
        printf("Failed to change permissions\n");
        exit(1);
    }

    printf("\nAfter changing permissions:\n");

    pageinfo(va);

    printf("\nAttempting to write to protected page...\n");

    int pid = fork();

    if(pid < 0) {
        printf("Fork failed\n");
        exit(1);
    }

    if(pid == 0) {

        // This write should cause a page fault.
        p[0] = 'B';

        printf("Write unexpectedly succeeded\n");

        exit(0);
    }

    wait(0);

    printf("\nParent process continues successfully.\n");

    exit(0);
}

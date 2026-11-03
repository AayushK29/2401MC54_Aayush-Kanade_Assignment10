#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    printf("Virtual to Physical Address Translation Test\n");

    char *p = sbrk(3 * 4096);

    if(p == (char*)-1) {
        printf("Allocation failed\n");
        exit(1);
    }

    // Touch all three pages.
    p[0] = 'A';
    p[4096] = 'B';
    p[8192] = 'C';

    printf("\nTesting valid virtual addresses:\n");

    for(int i = 0; i < 3; i++) {

        uint64 va = (uint64)(p + i * 4096);

        uint64 pa = vtop(va);

        printf("Virtual address: %d\n", (int)va);
printf("Physical address: %d\n", (int)pa);
printf("\n");
    }

    printf("Testing invalid virtual address:\n");

    uint64 invalid = 0x40000000;

    uint64 result = vtop(invalid);

    if(result == (uint64)-1) {
        printf("Address is invalid or unmapped\n");
    }
    else {
        printf("Physical address: %d\n",(int)result);
    }

    sbrk(-3 * 4096);

    exit(0);
}

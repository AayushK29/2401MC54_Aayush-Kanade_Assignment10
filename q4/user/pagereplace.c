
#include "kernel/types.h"
#include "user/user.h"

#define MAX 20

int pages[MAX] = {
    7, 0, 1, 2, 0, 3, 0, 4, 2, 3,
    0, 3, 2, 1, 2, 0, 1, 7, 0, 1
};

// Print current frame contents.
void
printframes(int frames[], int n)
{
    printf("Frames: ");

    for(int i = 0; i < n; i++) {
        if(frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
}

// FIFO Algorithm
void
fifo(int n)
{
    int frames[5];
    int next = 0;
    int hits = 0, faults = 0;

    for(int i = 0; i < n; i++)
        frames[i] = -1;

    printf("\n========== FIFO (%d Frames) ==========\n", n);

    for(int i = 0; i < MAX; i++) {

        int found = 0;

        for(int j = 0; j < n; j++) {
            if(frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(found) {
            hits++;
        }
        else {
            faults++;

            frames[next] = pages[i];

            next = (next + 1) % n;
        }

        printf("Page %d: ", pages[i]);
        printframes(frames, n);

        if(found)
            printf(" HIT\n");
        else
            printf(" FAULT\n");
    }

    printf("Total Hits: %d\n", hits);
    printf("Total Faults: %d\n", faults);
}

// LRU Algorithm
void
lru(int n)
{
    int frames[5];
    int last[5];

    int hits = 0, faults = 0;

    for(int i = 0; i < n; i++) {
        frames[i] = -1;
        last[i] = -1;
    }

    printf("\n========== LRU (%d Frames) ==========\n", n);

    for(int i = 0; i < MAX; i++) {

        int found = -1;

        for(int j = 0; j < n; j++) {
            if(frames[j] == pages[i]) {
                found = j;
                break;
            }
        }

        if(found != -1) {
            hits++;
            last[found] = i;
        }
        else {
            faults++;

            int replace = -1;

            // First use an empty frame.
            for(int j = 0; j < n; j++) {
                if(frames[j] == -1) {
                    replace = j;
                    break;
                }
            }

            // Otherwise find the least recently used page.
            if(replace == -1) {
                replace = 0;

                for(int j = 1; j < n; j++) {
                    if(last[j] < last[replace])
                        replace = j;
                }
            }

            frames[replace] = pages[i];
            last[replace] = i;
        }

        printf("Page %d: ", pages[i]);
        printframes(frames, n);

        if(found != -1)
            printf(" HIT\n");
        else
            printf(" FAULT\n");
    }

    printf("Total Hits: %d\n", hits);
    printf("Total Faults: %d\n", faults);
}

// Optimal Algorithm
void
optimal(int n)
{
    int frames[5];

    int hits = 0, faults = 0;

    for(int i = 0; i < n; i++)
        frames[i] = -1;

    printf("\n========== OPTIMAL (%d Frames) ==========\n", n);

    for(int i = 0; i < MAX; i++) {

        int found = 0;

        for(int j = 0; j < n; j++) {
            if(frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(found) {
            hits++;
        }
        else {
            faults++;

            int replace = -1;

            // Check for an empty frame.
            for(int j = 0; j < n; j++) {
                if(frames[j] == -1) {
                    replace = j;
                    break;
                }
            }

            if(replace == -1) {

                int farthest = -1;

                for(int j = 0; j < n; j++) {

                    int nextuse = MAX;

                    for(int k = i + 1; k < MAX; k++) {
                        if(frames[j] == pages[k]) {
                            nextuse = k;
                            break;
                        }
                    }

                    if(nextuse > farthest) {
                        farthest = nextuse;
                        replace = j;
                    }
                }
            }

            frames[replace] = pages[i];
        }

        printf("Page %d: ", pages[i]);
        printframes(frames, n);

        if(found)
            printf(" HIT\n");
        else
            printf(" FAULT\n");
    }

    printf("Total Hits: %d\n", hits);
    printf("Total Faults: %d\n", faults);
}

// Print percentage with two decimal places.
void
percentage(int value)
{
    int result = value * 10000 / MAX;

    printf("%d.%d%%", result / 100, result % 100);
}

int
main(void)
{
    int framecounts[] = {3, 4, 5};

    printf("PAGE REPLACEMENT ALGORITHMS\n");

    for(int i = 0; i < 3; i++) {

        int n = framecounts[i];

        fifo(n);
        lru(n);
        optimal(n);
    }

    printf("\n========== FINAL COMPARISON ==========\n");

    printf("Frames Algorithm Hits Faults HitRatio FaultRatio\n");

    for(int i = 0; i < 3; i++) {

        int n = framecounts[i];

        // FIFO
        int fh = 0, ff = 0;
        int f[5], next = 0;

        for(int j = 0; j < n; j++)
            f[j] = -1;

        for(int j = 0; j < MAX; j++) {

            int found = 0;

            for(int k = 0; k < n; k++) {
                if(f[k] == pages[j]) {
                    found = 1;
                    break;
                }
            }

            if(found)
                fh++;
            else {
                ff++;
                f[next] = pages[j];
                next = (next + 1) % n;
            }
        }

        printf("%d FIFO     %d %d ", n, fh, ff);
        percentage(fh);
        printf(" ");
        percentage(ff);
        printf("\n");

        // LRU
        int lh = 0, lf = 0;
        int l[5], last[5];

        for(int j = 0; j < n; j++) {
            l[j] = -1;
            last[j] = -1;
        }

        for(int j = 0; j < MAX; j++) {

            int found = -1;

            for(int k = 0; k < n; k++) {
                if(l[k] == pages[j]) {
                    found = k;
                    break;
                }
            }

            if(found != -1) {
                lh++;
                last[found] = j;
            }
            else {
                lf++;

                int replace = -1;

                for(int k = 0; k < n; k++) {
                    if(l[k] == -1) {
                        replace = k;
                        break;
                    }
                }

                if(replace == -1) {
                    replace = 0;

                    for(int k = 1; k < n; k++) {
                        if(last[k] < last[replace])
                            replace = k;
                    }
                }

                l[replace] = pages[j];
                last[replace] = j;
            }
        }

        printf("%d LRU      %d %d ", n, lh, lf);
        percentage(lh);
        printf(" ");
        percentage(lf);
        printf("\n");

        // Optimal
        int oh = 0, of = 0;
        int o[5];

        for(int j = 0; j < n; j++)
            o[j] = -1;

        for(int j = 0; j < MAX; j++) {

            int found = 0;

            for(int k = 0; k < n; k++) {
                if(o[k] == pages[j]) {
                    found = 1;
                    break;
                }
            }

            if(found) {
                oh++;
            }
            else {
                of++;

                int replace = -1;

                for(int k = 0; k < n; k++) {
                    if(o[k] == -1) {
                        replace = k;
                        break;
                    }
                }

                if(replace == -1) {

                    int farthest = -1;

                    for(int k = 0; k < n; k++) {

                        int nextuse = MAX;

                        for(int m = j + 1; m < MAX; m++) {
                            if(o[k] == pages[m]) {
                                nextuse = m;
                                break;
                            }
                        }

                        if(nextuse > farthest) {
                            farthest = nextuse;
                            replace = k;
                        }
                    }
                }

                o[replace] = pages[j];
            }
        }

        printf("%d OPTIMAL  %d %d ", n, oh, of);
        percentage(oh);
        printf(" ");
        percentage(of);
        printf("\n");
    }

    exit(0);
}

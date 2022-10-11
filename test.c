#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
    struct timespec tspec;
    long notnanotime;
    long nanotime;
    int x = 0;

    clock_gettime(CLOCK_REALTIME, &tspec);
    notnanotime = tspec.tv_sec;
    nanotime = tspec.tv_nsec;

    while (x == 0)
    {
        clock_gettime(CLOCK_REALTIME, &tspec);

        if (notnanotime != tspec.tv_sec)
        {
            printf("%ld\n", nanotime);
            return 0;
        }

        notnanotime = tspec.tv_sec;
        nanotime = tspec.tv_nsec;
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int flag;

void handler(int arg)
{
    flag = 1;
}

int main(int argc, char *argv[])
{
    int choice, delay;
    struct tm time_i, *time_ptr;
    time_t time_current, given_time, seconds;

    if (argc < 2 || argc > 3)
    {
        printf("Error: Invalid Arguments have been passed\n");
        printf("Usage: ./a.out <hour:minutes> [dd/mm/yy]\n");
        exit(1);
    }

    time_current = time(NULL);
    printf("Current Time: %ld\n", time_current);

    if (argc == 2)
    {
        time_ptr = localtime(&time_current);
        memset(&time_i, 0, sizeof(struct tm));
        strptime(argv[1], "%H:%M", &time_i);

        time_i.tm_mday = time_ptr->tm_mday;
        time_i.tm_mon = time_ptr->tm_mon;
        time_i.tm_year = time_ptr->tm_year;
    }

    if (argc == 3)
    {
        memset(&time_i, 0, sizeof(struct tm));
        strptime(argv[1], "%H:%M", &time_i);
        strptime(argv[2], "%d/%m/%y", &time_i);
    }

    given_time = mktime(&time_i);
    printf("Given time: %ld\n", given_time);

    if ((seconds = given_time - time_current) < 0)
    {
        printf("Error! Invalid time\n");
        exit(2);
    }

    printf("\nAlarm in %ld seconds\n\n", seconds);

    signal(SIGALRM, handler);
    alarm(seconds);

    while (1)
    {
        if (flag)
        {
            printf("Time is up!!!\n");
            printf("Choose the option\n1. Snooze\n2. Quit\n");

            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter Snooze time (in minutes): ");
                scanf("%d", &delay);
                alarm(delay * 60);
                flag = 0;
            }
            else if (choice == 2)
            {
                exit(0);
            }
        }
    }

    return 0;
}

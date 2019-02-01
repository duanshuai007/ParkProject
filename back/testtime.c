#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main()
{
    time_t seconds;
    struct tm time;
    struct tm time2;

    //seconds = time((time_t *)NULL);

    time.tm_year = 2019 - 1900;
    time.tm_mon = 1 - 1;
    time.tm_mday = 25;
    time.tm_hour = 12;
    time.tm_min = 0;
    time.tm_sec = 0;

    time2.tm_year = 2019 - 1900;
    time2.tm_mon = 1 - 1;
    time2.tm_mday = 25;
    time2.tm_hour = 13;
    time2.tm_min = 0;
    time2.tm_sec = 0;
    
    seconds = mktime(&time);
    int seconds2 = mktime(&time2);

    float tinterval = difftime(seconds2, seconds);

    printf("tinterval=%lf\r\n", tinterval);

    printf("%d\r\n", seconds);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TLE.h"
#include <time.h>

typedef struct VERIN
{
    char line1[70];
    char line2[70];
    double startmin;
    double stepmin;
    double stopmin;
    char name[30];
} VERIN;

/**
 * returns the count of verins read and sets the pointer to an array created with malloc.
 */
int readVERINs(VERIN **listptr)
{
    char line[256];
    char pline[256];
    char *str = NULL;
    FILE *in_file = NULL;
    VERIN *verins = NULL;
    int cnt = 0;

    in_file = fopen("../../data/gnss.tle", "r");

    while (fgets(line, 255, in_file) != NULL)
    {
        if (line[0] == '1')
            cnt++;
    }

    if (in_file)
    {
        fclose(in_file);
    }

    verins = malloc(cnt * sizeof(VERIN));
    *listptr = verins;

    cnt = 0;
    in_file = fopen("../../data/gnss.tle", "r");

    while (fgets(line, 255, in_file) != NULL)
    {
        if (line[0] != '1' && line[0] != '2')
        {
            strncpy(pline, line, 30);
        }

        if (line[0] == '1')
        {

            strncpy(verins[cnt].name, pline, 30);

            strncpy(verins[cnt].line1, line, 69);
            verins[cnt].line1[69] = 0;
            fgets(line, 255, in_file);
            strncpy(verins[cnt].line2, line, 69);
            verins[cnt].line2[69] = 0;
            str = &line[70];
            sscanf(str, "%lf %lf %lf", &verins[cnt].startmin, &verins[cnt].stopmin, &verins[cnt].stepmin);

            cnt++;
        }
    }

    if (in_file)
    {
        fclose(in_file);
    }

    return cnt;
}

void runVER(VERIN *verins, int cnt)
{
    time_t rawtime;

    time(&rawtime);
    long millis = rawtime * 1000;
    printf("Эпоха %ld: \n", millis);
    TLE tle;
    double r[3];
    double v[3];
    double mins = 120.0;
    int i;

    for (i = 0; i < cnt; i++)
    {
        parseLines(&tle, verins[i].line1, verins[i].line2);
        getRVForDate(&tle, millis, r, v);
        // printf("/* %s */", verins[i].name);
        printf("%3d %s %16ld %18.6f %18.6f %18.6f\n", i + 1, tle.objectID, tle.epoch, r[0], r[1], r[2]);
    }
}

int main(void)
{
    int cnt = 0;
    int i = 0;
    VERIN *verins = NULL;

    cnt = readVERINs(&verins);

    printf("read %d verins\n", cnt);
    runVER(verins, cnt);

    free(verins);
    return 0;
}

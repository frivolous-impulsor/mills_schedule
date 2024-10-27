#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define DAYS_IN_WEEK 7
#define SLOTS_IN_DAY 10

int schedule[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot

typedef struct
{
    char name[32];
    int hoursAvailable;
    int hours[DAYS_IN_WEEK][SLOTS_IN_DAY];  //willingness, 1 -> available, 2 -> prefered, otherwise(0) -> not available
} student;

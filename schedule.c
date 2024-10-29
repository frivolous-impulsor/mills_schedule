#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "indexMaxPriorityQueue.h"

#define DAYS_IN_WEEK 7
#define SLOTS_IN_DAY 10

int schedule[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot

typedef struct
{
    char name[32];
    int hoursAvailable;
    int hours[DAYS_IN_WEEK][SLOTS_IN_DAY];  //willingness, 1 -> available, 2 -> prefered, otherwise(0) -> not available
} student;


int main(int argc, char* argv[])
{   
    indexMaxPriorityQueue pq;
    pq.size = 0;
    insert(&pq, 0, 2);
    insert(&pq, 1, 1);
    insert(&pq, 2, 3);
    insert(&pq, 3, 4);
    insert(&pq, 4, 8);
    insert(&pq, 5, 10);
    insert(&pq, 6, 8);
    insert(&pq, 7, 22);
    insert(&pq, 8, 3);
    update(&pq, 8, 90);
    update(&pq, 8, 3.5);
    printf("top id %d\n", peekTopId(&pq));
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include "indexMaxPriorityQueue.h"

const int DAYS_IN_WEEK = 7;
const int SLOTS_IN_DAY = 10;

const char responsePath[] = "Response";

int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot
float hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day
int updatedPQ[MAX_QUEUE_SLOT];

char files[MAX_QUEUE_SLOT][MAX_NAME_LENGTH*2];

char student[MAX_QUEUE_SLOT][MAX_NAME_LENGTH];

typedef struct{
    int scheduleId;
    int resultSchedule[DAYS_IN_WEEK][SLOTS_IN_DAY][3];//id in each slot
} schedule;

typedef struct{
    int id;
    int willingness;
} willTuple;

willTuple preferenceMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY][16];

float availableHoursArray[MAX_QUEUE_SLOT];  
//each element with index i denotes the remaining available hours of the student with id i

int gatherCSVs(){
    DIR *dir;
    dir = opendir(responsePath);
}

int preprocessing(){
    //gather csv files from responses

    return 0;
}
//
//TODO


int arrange(){
    int day, slot, spot, peopleNeeded, id;
    float hours;

    for(day = 0; day < DAYS_IN_WEEK; day++){
        for(slot = 0; slot < SLOTS_IN_DAY; slot++){
            peopleNeeded = needMatrix[day][slot];
            hours = hoursMatrix[day][slot];
            //for each slot, form a priority queue with available people, with priority being multifactor 
            indexMaxPriorityQueue pq;
            pq.size = 0;
            //insert all students who have availability at this slot

            //peek <peopleNeeded> times and update their hoursAvailable
            for(spot = 0; spot < peopleNeeded; spot++){
                id = peekTopId(&pq);
                availableHoursArray[id] -= hours;
                //remove the updated studen from PQ
                //TODO

            }
        }
    }
    return 0;
}



int main(int argc, char* argv[])
{   
    willTuple testT = {.id = 0, .willingness =3};
    preferenceMatrix[0][0][0] = testT;
    printf("id %d has willingness %d\n", preferenceMatrix[0][0][0].id,preferenceMatrix[0][0][0].willingness);
    return 0;    
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "indexMaxPriorityQueue.h"

#define NAME_LENGTH 32
#define DAYS_IN_WEEK 7
#define SLOTS_IN_DAY 10

int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot
float hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day

typedef struct{
    int scheduleId;
    int resultSchedule[DAYS_IN_WEEK][SLOTS_IN_DAY][3];//id in each slot
} schedule;


typedef int preferenceMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];  
//willingness, 1 -> available, 2 -> prefered, otherwise(0) -> not available
preferenceMatrix prefMatrixArray[MAX_QUEUE_SLOT];

float availableArray[MAX_QUEUE_SLOT];  
//each element with index i denotes the remaining available hours of the student with id i

//Update prefMatrixArray and availableArray from csv grabed from python convert
//TODO

int main(int argc, char* argv[])
{   
    int day, slot, spot, peopleNeeded, id;
    float hours;



    
    for(day = 0; day < DAYS_IN_WEEK; day++){
        for(slot = 0; slot < SLOTS_IN_DAY; slot++){
            peopleNeeded = needMatrix[day][slot];
            hours = hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
            //for each slot, form a priority queue with available people, with priority being multifactor 
            indexMaxPriorityQueue pq;
            pq.size = 0;
            //insert all students who have availability at this slot

            //peek <peopleNeeded> times and update their hoursAvailable
            for(spot = 0; spot < peopleNeeded; spot++){
                id = peekTopId(&pq);
                availableArray[id] -= hours;
                //remove the updated studen from PQ
                //TODO
                
            }
        }
    }
    
}

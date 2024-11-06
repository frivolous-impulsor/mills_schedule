#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>

const int DAYS_IN_WEEK = 7;
const int SLOTS_IN_DAY = 10;
int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot
float hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day


typedef struct{
    int id;
    int willingness;
} willTuple;

typedef struct willNode{
    willTuple tuple;
    struct willNode* nextWill;
} willNode;    

/*
int append_chain(willChain* chain, willTuple* tuple){
    while(*chain->nextWill != NULL){
        chain = chain->nextWill;
    }
    return 0;
}
*/





int main(int argc, char* argv[])
{   
    int i,j;
    willNode currentNode;
    willTuple currentTuple;
    int count =0;
    willNode *preferenceMatrix = calloc(DAYS_IN_WEEK*SLOTS_IN_DAY, sizeof(willNode));
    for(i = 0; i<DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            currentNode = *((willNode*)preferenceMatrix+i*DAYS_IN_WEEK+j);
            currentTuple = currentNode.tuple;
            printf("id: %d, willingness: %d, next node: %p\n", currentTuple.id, currentTuple.willingness, currentNode.nextWill);
            count++;
        }
    }
    printf("count %d\n", count);
    free(preferenceMatrix);
    return 0;    
}

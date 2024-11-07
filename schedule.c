#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include "indexMaxPriorityQueue.h"

const int DAYS_IN_WEEK = 7;
const int SLOTS_IN_DAY = 10;
int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot
int hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day
int updatedPQ[MAX_QUEUE_SLOT];
float availableHoursArray[MAX_QUEUE_SLOT];  

char student[MAX_QUEUE_SLOT][MAX_NAME_LENGTH];

typedef struct{
    int scheduleId;
    int resultSchedule[DAYS_IN_WEEK][SLOTS_IN_DAY][3];//id in each slot
} schedule;

typedef struct{
    int id;
    int willingness;
} willTuple;

typedef struct willNode{
    willTuple tuple;
    struct willNode* nextWill;
} willNode;    

typedef struct linkedWill{
    willNode *headNode;
    willNode *tailNode;    
} linkedWill;

int initEptLink(linkedWill *link, willNode *node){
    if(node->nextWill != NULL){
        return 1;
    }
    link->headNode = node;
    link->tailNode = node;
    return 0;
}

void append_Link(linkedWill* link, willTuple* tuple){
    willNode currentNode = {.tuple = *tuple, .nextWill = NULL};
    (*(link->tailNode)).nextWill = &currentNode;
    link->tailNode = &currentNode;
}

linkedWill* preferenceMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];


//each element with index i denotes the remaining available hours of the student with id i
char *strtok_single (char * str, char const * delims);
//strictly only accepting csv file that has "DAY,NUM,NUM,NUM.." format and 2D int array for nums in each day 
int csv2array(char *filePath, int *array){
    char buffer[255];
    int i = 0; 
    int j = 0;
    int val;
    FILE *file = fopen(filePath, "r");
    if(file == NULL){
        printf("no template found\n");
        return 1;
    }
   
    while(fgets(buffer, sizeof(buffer), file)){
        char* token;
        token = strtok_single(buffer, ",");
        token = strtok_single(NULL, ",");
        char tok;
        while(token != NULL){
            tok = (char)(*token);
            val = tok - '0';
            if(val < 0 || val > 9){
                val = 0;
            }
            *(array+i*SLOTS_IN_DAY + j) = val;
            token = strtok_single(NULL, ",");
            j++;
        }
        j= 0;
        i++;
    }
    return 0;
}

int templateRead(){
    char templatePath[] = "ProcessData/schedule_template.csv";
    char shiftHourPath[] = "ProcessData/schedule_shiftHour.csv";
    csv2array(templatePath, (int*)needMatrix);
    csv2array(shiftHourPath, (int*)hoursMatrix);
    return 0;
}

int gatherCSVs(const char *dirName, char *files);

int initializeGeneralWillMatrix(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    int i, j;
    willTuple startingTuple = {.id = -1, .willingness = 0};
    willNode startingNode = {.tuple = startingTuple, .nextWill = NULL};
    for(i = 0; i<DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            linkedWill link = {.headNode = &startingNode, .tailNode = &startingNode};
            matrxi[i][j] = &link;
        }
    }
    return 0;
}

int preprocessing(indexMaxPriorityQueue* shiftPQ){
    //fill in needMatrix
    templateRead();

    //gather csv files from responses
    int id;
    int fileCount;
    const char responsePath[] = "./ProcessData/responsesCSV";
    char *files = malloc(sizeof(char) * MAX_QUEUE_SLOT*MAX_NAME_LENGTH*2);

    fileCount = gatherCSVs(responsePath, files);
    char filePath[MAX_NAME_LENGTH*2];
    int willMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
    linkedWill* generalWillMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
    initializeGeneralWillMatrix(generalWillMatrix);
    int i,j;
    int willingness;
    
    printf("total csv files %d\n", fileCount);
    for(id = 0; i< fileCount; i++){
        strcpy(filePath, files+i*MAX_NAME_LENGTH*2);
        printf("start integrating file: %s\n", filePath);
        
        //for each csv, update generalWillMatrix, available hours, indexPQ
        csv2array(filePath, (int*)willMatrix);
        for(i = 0; i<DAYS_IN_WEEK; i++){
            for(j = 0; j<SLOTS_IN_DAY; j++){
                willingness = willMatrix[i][j];
                if(willingness==1 || willingness==2){
                    willTuple currentWill = {.id = id, .willingness = willingness};
                    append_Link(generalWillMatrix[i][j], &currentWill);                   
                }
            }
        }
        availableHoursArray[id] = 10;
        strcpy(student[id], filePath);
        insert(shiftPQ, id, 0);
        id++;
    }
    

    free(files);
    return 0;
}
//
//TODO
int arrange();

int main(int argc, char* argv[])
{   
    int i, j;
    templateRead();
    indexMaxPriorityQueue shiftPQ;
    shiftPQ.size = 0;
    preprocessing(&shiftPQ);
    for(i = 0; i<7; i++){
        for(j=0; j< SLOTS_IN_DAY; j++){
            printf(" |%d| ", needMatrix[i][j]);

        }
        printf("\n");
    }
    return 0;    
}

char *strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;

  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;

  } else if (*src) {
    ret = src;
    src = NULL;
  }

  return ret;
}

int gatherCSVs(const char *dirName, char *files){
    DIR *dir;
    struct dirent *entry;
    dir = opendir(dirName);
    if(dir == NULL){
        printf("unable to open directory, check if Responses exists\n");
        return -1;
    }
    int fileCount = 0;
    while((entry = readdir(dir))){
        char *ext = strrchr(entry->d_name, '.');
        if( (ext != NULL)  && (!strcmp(ext, ".csv"))  ){
            strcpy(files+fileCount*MAX_NAME_LENGTH*2, entry->d_name);
            fileCount++;
        }
    }
    closedir(dir);
    return fileCount;
}

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
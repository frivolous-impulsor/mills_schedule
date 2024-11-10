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


typedef struct willNode{
    int id;
    int willingness;
    struct willNode* nextWill;
} willNode;    

typedef struct linkedWill{
    willNode *headNode;
    willNode *tailNode;    
} linkedWill;



void append_Link(linkedWill* link, willNode* node){
    link->tailNode->nextWill = node;
    link->tailNode = node;
}



int initializeGeneralWillMatrix(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    int i, j;
    
    for(i = 0; i<DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            willNode *headNode = (willNode*)malloc(sizeof(willNode));
            headNode->id = -1;
            headNode->willingness = 0;
            headNode->nextWill = NULL;
            linkedWill *link = (linkedWill*)malloc(sizeof(linkedWill));
            link->headNode = headNode;
            link->tailNode = headNode;
            matrxi[i][j] = link;
        }
    }
    return 0;
}

void freeGeneralWillMatrix(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    int i, j;
    free(matrxi[0][0]->headNode);
    for(i = 0; i<DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            free(matrxi[i][j]);
        }
    }
}


//each element with index i denotes the remaining available hours of the student with id i
char *strtok_single (char * str, char const * delims);
//strictly only accepting csv file that has "DAY,NUM,NUM,NUM.." format and 2D int array for nums in each day 
int csv2array(char *filePath, int *array);

int templateRead();

void printCSV(int* csvMatrix, char* name){
    int i, j;
    for(i = 0; i < DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            printf(" |%d| ", *(csvMatrix+i*SLOTS_IN_DAY +j));
        }
        printf("\n");
    }
}

int gatherCSVs(const char *dirName, char *files);

linkedWill* generalWillMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];

void printGeneralWillMat(){
    int i, j;
    for(i = 0; i < DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            printf(" |");
            linkedWill *currentLink = generalWillMatrix[i][j];
            willNode *currentNode = currentLink->headNode;
            while(1){
                if(currentNode->id != -1){
                    printf("(id: %d, will: %d) ", currentNode->id, currentNode->willingness);
                }
                currentNode = currentNode->nextWill;
                if(currentNode == NULL){
                    break;
                }
            }
            printf("| ");
        }
        printf("\n");
    }
}

int preprocessing(indexMaxPriorityQueue* shiftPQ){
    //fill in needMatrix
    templateRead();

    //gather csv files from responses
    int id;
    int fileCount;
    char *responsePath = "./PRDAT/RSP/";
    char *files = malloc(sizeof(char)*MAX_QUEUE_SLOT*MAX_NAME_LENGTH*2);
    if(files == NULL){
        fprintf(stderr, "Fatal: failed to allocate for files.\n");
        
    }

    
    fileCount = gatherCSVs(responsePath, files);
    for(int z = 0; z < fileCount; z++){
        printf("%s\n", files+z*MAX_NAME_LENGTH*2);
    }
    int willMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
    initializeGeneralWillMatrix(generalWillMatrix);
    int i,j;
    int willingness;
    
    printf("total csv files %d\n", fileCount);
    printf("filecount: %d\n", fileCount);
    for(int i_file = 0; i_file< fileCount; i_file++){
        int id = i_file;
        char fileName[MAX_NAME_LENGTH*2];
        strcpy(fileName, files+id*MAX_NAME_LENGTH*2);
        char *filePath = malloc(strlen(fileName) + strlen(responsePath) + 1);
        if(filePath == NULL){
            fprintf(stderr, "Fatal: failed to allocate for filePath.\n");
        }
        strcpy(filePath, responsePath);
        strcat(filePath, fileName);
        printf("start integrating file: %s\n", filePath);
        
        //for each csv, update generalWillMatrix, available hours, indexPQ
        csv2array(filePath, (int*)willMatrix);
        printCSV((int*)willMatrix, filePath);
        
        for(i = 0; i<DAYS_IN_WEEK; i++){
            for(j = 0; j<SLOTS_IN_DAY; j++){
                willingness = willMatrix[i][j];

                if(willingness == 1 || willingness == 2){

                    willNode *currentNode = (willNode*)malloc(sizeof(willNode));    
                    currentNode->id = id;
                    currentNode->willingness = willingness;
                    currentNode->nextWill = NULL;
                    append_Link(generalWillMatrix[i][j], currentNode);  
                }
                
            }
        }
        
        //availableHoursArray[id] = 10;
        //strcpy(student[id], "PRDAT/RSP/bumblebee.csv");
        //insert(shiftPQ, id, 0);
        
        free(filePath);
    }
    

    printGeneralWillMat();

    free(files);
    return 0;
}
//
//TODO
int arrange();

void test(){
    
}

int main(int argc, char* argv[])
{   
    int i, j;
    templateRead();
    indexMaxPriorityQueue shiftPQ;
    shiftPQ.size = 0;
    preprocessing(&shiftPQ);


    //test();
    freeGeneralWillMatrix(generalWillMatrix);
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

int templateRead(){
    char peopleNeededPath[] = "PRDAT/peopleNeeded.csv";
    char shiftHourPath[] = "PRDAT/shiftHour.csv";
    csv2array(peopleNeededPath, (int*)needMatrix);
    csv2array(shiftHourPath, (int*)hoursMatrix);
    return 0;
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


int csv2array(char *filePath, int *array){
    char buffer[255];
    int i = 0; 
    int j = 0;
    int val;
    FILE *file = fopen(filePath, "r");
    if(file == NULL){
        printf("no file found with name: %s\n", filePath);
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
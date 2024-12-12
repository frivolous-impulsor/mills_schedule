#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include <sysinfoapi.h>
#include "indexMaxPriorityQueue.h"

#define DAYS_IN_WEEK 7
#define SLOTS_IN_DAY 10
int studentNum;
int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot
int hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day
int shiftPriorityMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
int updatedPQ[MAX_QUEUE_SLOT];
int availableHoursArray[MAX_QUEUE_SLOT];
int resultHours[MAX_QUEUE_SLOT];

char student[MAX_QUEUE_SLOT][MAX_NAME_LENGTH];

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

linkedWill* result[DAYS_IN_WEEK][SLOTS_IN_DAY];


int initializeLinkedMat(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]);
void freeLinkedWillMat(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]);
//each element with index i denotes the remaining available hours of the student with id i
char *strtok_single (char * str, char const * delims);
//strictly only accepting csv file that has "DAY,NUM,NUM,NUM.." format and 2D int array for nums in each day 
int csv2array(char *filePath, int *array);
int templateRead();
void printCSV(int* csvMatrix);
int gatherCSVs(const char *dirName, char *files);
linkedWill* generalWillMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
void printLinkedMat(linkedWill* mat[DAYS_IN_WEEK][SLOTS_IN_DAY]);
int preprocessing(indexMaxPriorityQueue* shiftPQ);



int willDenseProcessing(indexMaxPriorityQueue* slotPQ){ //construct slotPQ that reflects popularity of shifts, arranging from least to most later
    int day, slot, count, slotIndex;
    int asteriskWeight, priority;
    int *needOpMatrix = calloc(DAYS_IN_WEEK*SLOTS_IN_DAY, sizeof(int)); 
    memcpy(needOpMatrix, needMatrix, DAYS_IN_WEEK*SLOTS_IN_DAY*sizeof(int));


    for(day = 0; day < DAYS_IN_WEEK; day++){
        for(slot = 0; slot < SLOTS_IN_DAY; slot++){
            count = 0;

            slotIndex = day*SLOTS_IN_DAY + slot;
            linkedWill *currentLink = generalWillMatrix[day][slot];
            willNode *currentNode = currentLink->headNode;
            while(1){
                if(currentNode->id == -1){
                    if(currentNode->nextWill == NULL){
                        break;
                    }
                    currentNode = currentNode->nextWill;
                    continue;
                }

                count++;
                currentNode = currentNode->nextWill;
                if(currentNode == NULL){
                    break;
                }
            }
            asteriskWeight = (15 + rand()%20 ) * shiftPriorityMatrix[day][slot];
            priority = studentNum-count + asteriskWeight;
            insert(slotPQ, slotIndex, priority);//value in queue for each slot will be inverse of popularity
        }
    }
    free(needOpMatrix);
    return 0;
}

void randWeight(int *willW, int *availW, int *workW){
    *willW = 1 + rand() % 9;
    *availW = 0 + rand() % 1;
    *workW = 1 + rand() % 6;
}

void arrange(indexMaxPriorityQueue *pq, indexMaxPriorityQueue *slotPopulorPQ, float *ratio, float *satisfaction){
    int day, slot, spot, peopleNeeded, id, slotIndex;

    
    

    float hours;
    int willWeight, availWeight, workedWeight;
    randWeight(&willWeight, &availWeight, &workedWeight);
    bool updatedMap[MAX_QUEUE_SLOT] = {false};//a record for all students that's been considered with priority. for clean up reference at the end of each slot
    initializeLinkedMat(result);
    int positionTotal = 0;
    int positionCovered = 0;
    int positionPrefered = 0;
    int *willArray = (int *)calloc(MAX_QUEUE_SLOT, sizeof(int));

    int *worked = (int*)calloc(MAX_QUEUE_SLOT*DAYS_IN_WEEK, sizeof(int)) ;//a record of wether a student worked on any given day, reference for priority calc.

        for(int i = 0; i< studentNum; i++){
        availableHoursArray[i] = 10;
    }
    do{
        slotIndex = removeTop(slotPopulorPQ);
        day = slotIndex / SLOTS_IN_DAY;
        slot = slotIndex % SLOTS_IN_DAY;
        peopleNeeded = needMatrix[day][slot];
        positionTotal += peopleNeeded;
        hours = hoursMatrix[day][slot];
        
        //for each slot, update priority queue with available people, with priority being multifactor
        
        //for each person in generalWillMat on that linkedWill, calc their priority by p = (availableH + availableH*will)
        linkedWill *currentLink = generalWillMatrix[day][slot];
        willNode *currentNode = currentLink->headNode;
        while(1){
            if(currentNode->id == -1){
                if(currentNode->nextWill == NULL){
                    break;
                }
                currentNode = currentNode->nextWill;
                continue;
            }
            
            id = currentNode->id;
            int will = currentNode->willingness;
            *(willArray + id) = will;
            //printf("willingness: %d\n", will);
            int priority = (willWeight + will) * ( (availWeight + availableHoursArray[id]) - (workedWeight + (*(worked + id*DAYS_IN_WEEK + day))) ) ;//logic needs tunning
            //update all students who have availability at this slot
            update(pq, id, priority);
            updatedMap[id] = true;

            currentNode = currentNode->nextWill;
            if(currentNode == NULL){
                break;
            }
        }

        //peek <peopleNeeded> times and update their hoursAvailable
        linkedWill* resulLink = result[day][slot];
        for(spot = 0; spot < peopleNeeded; spot++){

            int topId = peekTopId(pq);
            //printf("peeped val %f for %s at slot %d day %d\n", pq->values[topId], student[topId], slot, day);
            //printf("priority: %f\n", pq->values[topId]);
            if(pq->values[topId] <= 0){
                topId = -2;//if val(willingness) of the top person is not even suitable for shift, -2 indicate vacancy
            }
            willNode *resultNode = (willNode*)malloc(sizeof(willNode));
            resultNode->id = topId;
            resultNode->willingness = -1;
            resultNode->nextWill = NULL;
            append_Link(resulLink, resultNode);
            if(topId != -2){
                //printf("willingness: %d\n",   *(willArray+topId));
                if(  *(willArray+topId) == 2  ){
                    positionPrefered++;
                }
                update(pq, topId, 0);
                positionCovered++;
                *(worked + id*DAYS_IN_WEEK + day) = 1;

                availableHoursArray[topId] -= hours; 
                
            }
        }
        memset(willArray, 0, sizeof(int)*MAX_QUEUE_SLOT);


        for(int cleanI = 0; cleanI < MAX_QUEUE_SLOT; cleanI++){
            if(updatedMap[cleanI]){
                update(pq, cleanI, 0);
                updatedMap[cleanI] = false;
            }
        }
    }while(slotPopulorPQ->size > 0);   

    *ratio = ((float)positionCovered)/positionTotal;
    *satisfaction = ((float)positionPrefered)/positionTotal;

    free(willArray);
    free(worked);
}


void writeResult(linkedWill * resultMat[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    
    FILE *fd = fopen("PRDAT/result.csv", "w");
    int currentId;
    if(fd == NULL){
        perror("result file open failed\n");
    }
    char *days[] = {"SUN", "MON", "TUE", "WED", "THR", "FRI", "SAT"};
    for(int i = 0; i < DAYS_IN_WEEK; i++){
        fprintf(fd, "%s,", days[i]);
        for(int j = 0; j < SLOTS_IN_DAY; j++){
            linkedWill *currentLink = resultMat[i][j];
            willNode *currentNode = currentLink->headNode->nextWill;

            while(1){
                if(currentNode == NULL){
                    break;
                }
                if(currentNode->id == -2){
                    fprintf(fd, "[       ]");
                }else{
                    fprintf(fd, "[%s]", student[currentNode->id]);
                }
                currentNode = currentNode->nextWill;
            }
            fprintf(fd, ",");
        }
        if(i < 6){
            fprintf(fd, "\n");
        }
    }
}

void printResultHour(){
    int i;
    for(i = 0; i< studentNum; i++){
        printf("%-15s: %3d\n", student[i], resultHours[i]);
    }
}


void calcAvailDeviation(int *deviation){
    int i;
    int diffSqure, sum;

    sum = 0;
    for(i = 0; i<studentNum; i++){
        diffSqure = pow(availableHoursArray[i], 2);
        sum +=diffSqure;
    }
    *deviation = sum;
}



int main(int argc, char* argv[])
{   
    SYSTEMTIME st;
    GetSystemTime(&st);
    srand(st.wMilliseconds);
    int i, repeat;
    float coveredRatio, //ratio of covered position count to needed position count
        satisfaction,
        newScore,
        highScore;   //ratio of position allocated to a student with 2 to total positions
    repeat = 1000;
    highScore = -100000;
    float *reading = (float*)malloc(sizeof(float));
    int *deviation = (int*)malloc(sizeof(int));
    indexMaxPriorityQueue shiftPQ;
    shiftPQ.size = 0;

    indexMaxPriorityQueue slotPQ;   //popularity of each shift
    slotPQ.size = 0;

    preprocessing(&shiftPQ);

    willDenseProcessing(&slotPQ);

    for(i = 0; i < repeat; i++){
        arrange(&shiftPQ, &slotPQ, &coveredRatio, &satisfaction);
        calcAvailDeviation(deviation);
        //printf("deviation %d\n", *deviation);
        //printf("cover: %f sat: %f dev: %d\n", coveredRatio, satisfaction, *deviation);
        newScore = coveredRatio * 3 + satisfaction * 2 - pow(*deviation, 1/2);
        //printf("new score: %f\n", newScore);
        if(newScore - highScore > 0.000001){
            printf("iteration: %4d-> new high score: %6f\n", i, newScore);
            writeResult(result);
            highScore = newScore;
            memcpy(resultHours, availableHoursArray, MAX_QUEUE_SLOT*sizeof(int));
        }

        willDenseProcessing(&slotPQ);
    }
    printResultHour();
    freeLinkedWillMat(generalWillMatrix);
    freeLinkedWillMat(result);
    free(reading);
    free(deviation);
    return 0;    
}

void freeLinkedWillMat(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    int i, j;
    for(i = 0; i<DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            free(matrxi[i][j]->headNode);
            free(matrxi[i][j]);
        }
    }
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

void printCSV(int* csvMatrix){
    int i, j;
    for(i = 0; i < DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            printf(" |%d| ", *(csvMatrix+i*SLOTS_IN_DAY +j));
        }
        printf("\n");
    }
}

int templateRead(){
    char peopleNeededPath[] = "PRDAT/peopleNeeded.csv";
    char shiftHourPath[] = "PRDAT/shiftHour.csv";
    char shiftPriorityPath[] = "PRDAT/shiftPriority.csv";
    csv2array(peopleNeededPath, (int*)needMatrix);
    csv2array(shiftHourPath, (int*)hoursMatrix);
    csv2array(shiftPriorityPath, (int*)shiftPriorityMatrix);
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
    errno = 0;
    int val;
    char* token;
    char* end;
    FILE *file = fopen(filePath, "r");
    if(file == NULL){
        printf("no file found with name: %s\n", filePath);
        return 1;
    }
   
    while(fgets(buffer, sizeof(buffer), file)){
        token = strtok(buffer, ",");
        token = strtok(NULL, ",");
        val = strtol(token, &end, 10);

        if(errno == ERANGE){
            val = 0;
        }
        
        while(token != end){
            if((val < 0) || (val > MAX_QUEUE_SLOT)){
                val = 0;
            }
            *(array+i*SLOTS_IN_DAY + j) = val;
            token = strtok(NULL, ",");
            val = strtol(token, &end, 10);
            j++;
        }
        j= 0;
        i++;
    }
    return 0;
}

void printLinkedMat(linkedWill* mat[DAYS_IN_WEEK][SLOTS_IN_DAY]){
    int i, j;
    for(i = 0; i < DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            printf(" |");
            linkedWill *currentLink = mat[i][j];
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

int initializeLinkedMat(linkedWill* matrxi[DAYS_IN_WEEK][SLOTS_IN_DAY]){
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

int preprocessing(indexMaxPriorityQueue* shiftPQ){
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
    studentNum = fileCount;
    int willMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];
    initializeLinkedMat(generalWillMatrix);
    int i,j;
    int willingness;
    
    //printf("total responses %d\n", fileCount);
    for(int i_file = 0; i_file< fileCount; i_file++){
        int id = i_file;
        char fileName[MAX_NAME_LENGTH*2];
        strcpy(fileName, files+id*MAX_NAME_LENGTH*2);
        strcpy(student[id], files+id*MAX_NAME_LENGTH*2);
        char *filePath = malloc(strlen(fileName) + strlen(responsePath) + 1);
        if(filePath == NULL){
            fprintf(stderr, "Fatal: failed to allocate for filePath.\n");
        }
        strcpy(filePath, responsePath);
        strcat(filePath, fileName);
        //printf("start integrating file: %s for %s\n", filePath, student[id]);
        
        //for each csv, update generalWillMatrix, available hours, indexPQ
        csv2array(filePath, (int*)willMatrix);
        //printCSV((int*)willMatrix, filePath);
        
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
        availableHoursArray[id] = 10;
        insert(shiftPQ, id, 0);
        //printf("finished integrating file: %s\n", filePath);
        free(filePath);

    }
    //printGeneralWillMat();
    free(files);
    return 0;
}
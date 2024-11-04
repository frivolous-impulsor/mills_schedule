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
float hoursMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];    //hours in each slot in each day
int updatedPQ[MAX_QUEUE_SLOT];


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

char *
strtok_single (char * str, char const * delims)
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

int csv2array(char *filePath, char *array, int eleLength){
    char buffer[255];
    int i = 0; 
    int j = 0;
    FILE *file = fopen(filePath, "r");
    if(file == NULL){
        printf("no template found\n");
        return 1;
    }
    fgets(buffer, sizeof(buffer), file);
   
    while(fgets(buffer, sizeof(buffer), file)){
        char* token;
        token = strtok_single(buffer, ",");
        char tok[32];
        strcpy(tok, token);
        strcpy((array+14*eleLength*i + eleLength*j), tok);
        i++;
        while(token != NULL){
            token = strtok_single(NULL, ",");
            char tok[32];
            strcpy(tok, token);
            strcpy((array+14*eleLength*i + eleLength*j), tok);
            i++;
        }
        j++;
    }
    return 0;
}

int templateRead(){
    char templatePath[] = "ProcessData/schedule_template.csv";
    char buffer[255];
    bool validSlot[DAYS_IN_WEEK];
    bool isValRow = false;
    FILE *template = fopen(templatePath, "r");
    if(template == NULL){
        printf("no template found\n");
        return 1;
    }
    printf("found template\n");
    fgets(buffer, sizeof(buffer), template);
    //enter the first slot
    while (fgets(buffer, sizeof(buffer), template)){
        //get tokens 
        char* token;
        token = strtok_single(buffer, ",");
        while(token!=NULL){
            printf(" <%s> ", token);

            token = strtok_single(NULL, ",");
        }
        printf("\n");
    }
    fclose(template);
    return 0;
}

int gatherCSVs(const char *dirName, char *files);

int preprocessing(){
    //fill in needMatrix


    //gather csv files from responses
    int fileCount;
    const char responsePath[] = "./ProcessData/responsesCSV";
    char *files = malloc(sizeof(char) * MAX_QUEUE_SLOT*MAX_NAME_LENGTH*2);

    fileCount = gatherCSVs(responsePath, files);
    char filePath[MAX_NAME_LENGTH*2];
    printf("total csv files %d\n", fileCount);
    for(int i = 0; i< fileCount; i++){
        strcpy(filePath, files+i*MAX_NAME_LENGTH*2);
        printf("start integrating file: %s\n", filePath);
        //for each csv, 
    }
    

    free(files);
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
    char filePath[] = "ProcessData/schedule_template.csv";
    char *csvData = malloc(sizeof(char)*32*14*10);
    csv2array(filePath, csvData, 32);
    printf("%c",csvData[0]);
    //preprocessing();
    free(csvData);
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

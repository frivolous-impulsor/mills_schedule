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

#define DAYS_IN_WEEK 7
#define SLOTS_IN_DAY 10
int studentNum;
int needMatrix[DAYS_IN_WEEK][SLOTS_IN_DAY];   //needed number of people in each slot

void printCSV(int* csvMatrix){
    int i, j;
    for(i = 0; i < DAYS_IN_WEEK; i++){
        for(j = 0; j<SLOTS_IN_DAY; j++){
            printf(" |%d| ", *(csvMatrix+i*SLOTS_IN_DAY +j));
        }
        printf("\n");
    }
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
            if((val < 0) || (val > 40)){
                val = 99;
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

int main(){
    csv2array("PRDAT/peopleNeeded.csv", (int*)needMatrix);
    printCSV((int*)needMatrix);
}
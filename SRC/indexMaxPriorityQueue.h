#ifndef INDEXMAXPRIORITYQUEUE_H
#define INDEXMAXPRIORITYQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_QUEUE_SLOT 20
#define MAX_NAME_LENGTH 32

typedef struct
{
    int size;
    float values[MAX_QUEUE_SLOT];   //return the priority value given a keyIndex
    int positionMap[MAX_QUEUE_SLOT];//return the position of a node given a keyIndex
    int inverseMap[MAX_QUEUE_SLOT]; //return the keyIndex of a node given a position
} indexMaxPriorityQueue;

int insert(indexMaxPriorityQueue* pPQ, int keyId, float value);

int removeTop(indexMaxPriorityQueue* pPQ);

int update(indexMaxPriorityQueue* pPQ, int keyId, float newVal);

int peekTopId(indexMaxPriorityQueue* pPQ);

#endif
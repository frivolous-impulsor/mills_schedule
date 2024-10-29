#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "indexMaxPriorityQueue.c"

#define MAX_QUEUE_SLOT 20
#define MAX_NAME_LENGTH 16

struct indexMaxPriorityQueue;

int printPM(indexMaxPriorityQueue *pPQ);
int printIM(indexMaxPriorityQueue *pPQ);
int swap(indexMaxPriorityQueue* pPQ, int position1, int position2);
int swim(indexMaxPriorityQueue* pPQ, int keyPosition);
int sink(indexMaxPriorityQueue* pPQ, int keyPosition);
int insert(indexMaxPriorityQueue* pPQ, int keyId, float value);

int update(indexMaxPriorityQueue* pPQ, int keyId, float newVal);

int peekTopId(indexMaxPriorityQueue* pPQ);
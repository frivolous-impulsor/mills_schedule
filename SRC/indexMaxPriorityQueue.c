#include "indexMaxPriorityQueue.h"


int printPM(indexMaxPriorityQueue *pPQ){
    int size = pPQ->size;
    int i;
    if(size){
        printf("position map: ");
        for(i = 0; i<size; i++){
            printf("%d ", pPQ->positionMap[i]);
        }
        printf("\n");
        return 0;
    }
    return 1;
}
int printIM(indexMaxPriorityQueue *pPQ){
    int size = pPQ->size;
    int i;
    if(size){
        printf("inverse map: ");
        for(i = 0; i<size; i++){
            printf("%d ", pPQ->inverseMap[i]);
        }
        printf("\n");
        return 0;
    }
    return 1;
}

int swap(indexMaxPriorityQueue* pPQ, int position1, int position2){
    pPQ->positionMap[pPQ->inverseMap[position1]] = position2;
    pPQ->positionMap[pPQ->inverseMap[position2]] = position1;
    int temp = pPQ->inverseMap[position1];
    pPQ->inverseMap[position1] = pPQ->inverseMap[position2];
    pPQ->inverseMap[position2] = temp;
    return 0;
}

int swim(indexMaxPriorityQueue* pPQ, int keyPosition){
    int keyId = pPQ->inverseMap[keyPosition];
    float keyVal = pPQ->values[keyId];

    int parentPosition = (keyPosition-1) / 2;
    while (parentPosition >=0){
        int parentId = pPQ->inverseMap[parentPosition];
        float parentVal = pPQ->values[parentId];
        if(parentVal < keyVal){
            swap(pPQ, keyPosition, parentPosition);
            keyPosition = parentPosition;
            parentPosition = (keyPosition-1)/2;
        }else{
            break;
        }
    }
    return 0;
}

int sink(indexMaxPriorityQueue* pPQ, int keyPosition){
    
    float keyVal = pPQ->values[pPQ->inverseMap[keyPosition]];
    
    while(1){
        int leftPosition = keyPosition*2 + 1;
        int rightPosition = keyPosition*2 + 2;
        if(leftPosition >= pPQ->size){
            break;
        }
        int biggerPosition = leftPosition;
        int leftId = pPQ->inverseMap[leftPosition];
        float leftVal = pPQ->values[leftId];
        if(rightPosition < pPQ->size && pPQ->values[pPQ->inverseMap[rightPosition]] > leftVal){
            biggerPosition = rightPosition;
        }
        float biggerVal = pPQ->values[pPQ->inverseMap[biggerPosition]];
        if(keyVal < biggerVal){
            swap(pPQ, keyPosition, biggerPosition);
            keyPosition = biggerPosition;
        }else{
            break;
        }
    }
    return 0;
}

int insert(indexMaxPriorityQueue* pPQ, int keyId, float value){
    int size = pPQ->size;
    if(size < MAX_QUEUE_SLOT){
        pPQ->values[keyId] = value;
        pPQ->positionMap[keyId] = size;
        pPQ->inverseMap[size] = keyId;
        swim(pPQ, size);
        pPQ->size++;
        return 0;
    }
    perror("queue is currently full, consider increase the slot\n");
    return 1;
}

int removeTop(indexMaxPriorityQueue* pPQ){//not tested yet
    if(pPQ->size < 1){
        perror("empty PQ, can't remove");
    }
    int idTop = pPQ->inverseMap[0];
    swap(pPQ, 0, pPQ->size - 1);
    pPQ->size--;
    sink(pPQ, 0);
    
    return idTop;
}

int update(indexMaxPriorityQueue* pPQ, int keyId, float newVal){
    int initVal = pPQ->values[keyId];
    pPQ->values[keyId] = newVal;
    int keyPosition = pPQ->positionMap[keyId];
    if(initVal > newVal){
        sink(pPQ, keyPosition);   
    }else if(initVal < newVal){
        swim(pPQ, keyPosition);
    }else {
        return 1;
    }
    return 0;
}

int peekTopId(indexMaxPriorityQueue* pPQ){
    int topId = pPQ->inverseMap[0];
    return topId;
}
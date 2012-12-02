/* 
 * File:   customQueue.h
 * Author: arka
 *
 * Created on December 2, 2012, 12:04 PM
 */

#ifndef CUSTOMQUEUE_H
#define	CUSTOMQUEUE_H

/*******************CUSTOM QUEUE**************/
#include <stdio.h>
#include <stdlib.h>
#include "GraphData.h"

typedef struct heapData {
    int nodeNum;
    int parent;
} heapData;
 
typedef struct heapNode {
    float value;
    Node data;               //dummy
} heapNode;
 
typedef struct PQ {
    heapNode* heap;
    int size;
} PQ;
 
void insert(heapNode aNode, heapNode* heap, int size) {
    int idx;
    heapNode tmp;
    idx = size + 1;
    heap[idx] = aNode;
    while (heap[idx].value < heap[idx/2].value && idx > 1) {
    tmp = heap[idx];
    heap[idx] = heap[idx/2];
    heap[idx/2] = tmp;
    idx /= 2;
    }
}
 
void shiftdown(heapNode* heap, int size, int idx) {
    int cidx;        //index for child
    heapNode tmp;
    for (;;) {
        cidx = idx*2;
        if (cidx > size) {
            break;   //it has no child
        }
        if (cidx < size) {
            if (heap[cidx].value > heap[cidx+1].value) {
                ++cidx;
            }
        }
        //swap if necessary
        if (heap[cidx].value < heap[idx].value) {
            tmp = heap[cidx];
            heap[cidx] = heap[idx];
            heap[idx] = tmp;
            idx = cidx;
        } else {
            break;
        }
    }
}
 
heapNode removeMin(heapNode* heap, int size) {
    //int cidx;
    heapNode rv = heap[1];
    //printf("%d:%d:%d\n", size, heap[1].value, heap[size].value);
    heap[1] = heap[size];
    --size;
    shiftdown(heap, size, 1);
    return rv;
}
void pq_enqueue(heapNode node, PQ *q) {
    insert(node, q->heap, q->size);
    ++q->size;
}
 
heapNode pq_dequeue(PQ *q) {
   heapNode rv = removeMin(q->heap, q->size);
   --q->size;
   return rv; 
}
heapNode pq_peek(PQ *q) {
   heapNode rv = q->heap[1];
   //--q->size;
   return rv; 
} 
void pq_initQueue(PQ *q, int n) {
   q->size = 0;
   q->heap = (heapNode*)malloc(sizeof(heapNode)*(n+1));
}

bool pq_empty(PQ *q){
    return (q->size==0);
}
/*******************CUSTOM QUEUE**************/
void pq_free(PQ *q){
    free(q->heap);
    q->size = 0;
}
#endif	/* CUSTOMQUEUE_H */


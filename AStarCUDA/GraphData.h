/* 
 * File:   GraphData.h
 * Author: arka
 *
 * Created on December 1, 2012, 10:37 AM
 */

#ifndef GRAPHDATA_H
#define	GRAPHDATA_H

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
//#include<CL/cl.h>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstddef>
#include "StringSplit.h"

typedef struct
{
    // (V) This contains a pointer to the edge list for each vertex
    int *vertexArray;

    // Vertex count
    unsigned int vertexCount;

    // (E) This contains pointers to the vertices that each edge is attached to
    int *edgeArray;

    // Edge count
    unsigned int edgeCount;

    // (W) Weight array
    float *weightArray;
    
    //Coordinate latitudes
    float *latitudes;
    
    //Coordinate longitudes
    float *longitudes;

} GraphData;

//typedef struct{
//    float latitude;
//    float longitude;
//} CoordinateData;

typedef struct{
    int adjList[8];
    float weight[8];
} AdjacencyList;


typedef struct{
    int nodeNum;
    float fCost;
    int parent;
    size_t pos;
}Node;

void generateGraph(GraphData *graph);

void getNodeDetails(GraphData *graph, int vertex);

bool compareNodeCost(void* a,void* b);

//static int
//cmp_fCost(double next, double curr)
//{
//	return (next > curr);
//}

//
//static double
//get_fCost(void *a)
//{
//	return (double) ((Node*) a)->fCost;
//}
//
//
//static void
//set_fCost(void *a, double fCost)
//{
//	((Node*) a)->fCost = fCost;
//}
//
//
//static size_t
//get_pos(void *a)
//{
//	return ((Node*) a)->pos;
//}
//
//
//static void
//set_pos(void *a, size_t pos)
//{
//	((Node *) a)->pos = pos;
//}


 

 
//int main(int argc, char **argv) {
//    int n; 
//    int i;
//    PQ q;
//    heapNode hn;
//    n = atoi(argv[1]);
//    initQueue(&q, n);
//    srand(time(NULL));
//    for (i = 0; i < n; ++i) {
//        hn.value = rand()%10000;
//        printf("enqueue node with value: %d\n", hn.value);
//        enqueue(hn, &q);
//    }
//    printf("\ndequeue all values:\n");
//    for (i = 0; i < n; ++i) {
//        hn = dequeue(&q);
//        printf("dequeued node with value: %d, queue size after removal: %d\n", hn.value, q.size);
//    }
//}

#endif	/* GRAPHDATA_H */


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

static int
cmp_fCost(double next, double curr)
{
	return (next < curr);
}


static double
get_fCost(void *a)
{
	return (double) ((Node*) a)->fCost;
}


static void
set_fCost(void *a, double fCost)
{
	((Node*) a)->fCost = fCost;
}


static size_t
get_pos(void *a)
{
	return ((Node*) a)->pos;
}


static void
set_pos(void *a, size_t pos)
{
	((Node *) a)->pos = pos;
}

#endif	/* GRAPHDATA_H */


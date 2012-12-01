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
    int vertexCount;

    // (E) This contains pointers to the vertices that each edge is attached to
    int *edgeArray;

    // Edge count
    int edgeCount;

    // (W) Weight array
    float *weightArray;

} GraphData;

typedef struct{
    float latitude;
    float longitude;
} CoordinateData;

typedef struct{
    int adjList[8];
    float weight[8];
} AdjacencyList;


void generateGraph(GraphData *graph, CoordinateData** coordinates);

void getNodeDetails(GraphData *graph, CoordinateData *coordinate_values, int vertex);
#endif	/* GRAPHDATA_H */


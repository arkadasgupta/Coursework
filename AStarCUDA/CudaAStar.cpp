/* 
 * File:   main.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:36 AM
 */

#include "GraphData.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    GraphData graph;
    CoordinateData* coordinate_values;
    
    generateGraph(&graph, &coordinate_values);
    cout<< "Vertex Count:" << graph.vertexCount<< endl;
    cout<< "Edge Count:" << graph.edgeCount<< endl;
    
    getNodeDetails(&graph,coordinate_values,2);
    
    bool* visitedArray = (bool*)malloc(graph.vertexCount*sizeof(bool));
    memset(visitedArray,0,graph.vertexCount*sizeof(bool));
    float *gCostArray = (float*)malloc(graph.vertexCount*sizeof(float));
    memset(gCostArray,0,graph.vertexCount*sizeof(float));
    float *hCostArray = (float*)malloc(graph.vertexCount*sizeof(float));
    memset(hCostArray,0,graph.vertexCount*sizeof(float));
    float *fCostArray = (float*)malloc(graph.vertexCount*sizeof(float));
    memset(fCostArray,0,graph.vertexCount*sizeof(float));
    
    int *vertexArrayDevice, *edgeArrayDevice;
    float *weightArrayDevice, *gCostArrayDevice, *hCostArrayDevice , *fCostArrayDevice;
    bool *visitedArrayDevice;
    CoordinateData *coordinateArrayDevice;
    
    /* Allocate and populate buffers */
//    cudaMalloc((void**)&vertexArrayDevice,(graph.vertexCount)*sizeof(int));
//    cudaMalloc((void**)&edgeArrayDevice,(graph.edgeCount)*sizeof(int));
//    cudaMalloc((void**)&weightArrayDevice,(graph.edgeCount)*sizeof(float));
//    cudaMalloc((void**)&gCostArrayDevice,(graph.vertexCount)*sizeof(float));
//    cudaMalloc((void**)&hCostArrayDevice,(graph.vertexCount)*sizeof(float));
//    cudaMalloc((void**)&fCostArrayDevice,(graph.vertexCount)*sizeof(float));
//    cudaMalloc((void**)&visitedArrayDevice,(graph.vertexCount)*sizeof(bool));
//    cudaMalloc((void**)&coordinateArrayDevice,(graph.vertexCount)*sizeof(CoordinateData));
//    
//    cudaMemcpy(vertexArrayDevice, graph.vertexArray, (graph.vertexCount)*sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(edgeArrayDevice, graph.edgeArray, (graph.edgeCount)*sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(weightArrayDevice, graph.weightArray, (graph.edgeCount)*sizeof(float), cudaMemcpyHostToDevice);
//    cudaMemcpy(gCostArrayDevice, gCostArray, (graph.vertexCount)*sizeof(float), cudaMemcpyHostToDevice);
//    cudaMemcpy(hCostArrayDevice, hCostArray, (graph.vertexCount)*sizeof(float), cudaMemcpyHostToDevice);
//    cudaMemcpy(fCostArrayDevice, fCostArray, (graph.vertexCount)*sizeof(float), cudaMemcpyHostToDevice);
//    cudaMemcpy(visitedArrayDevice, visitedArray, (graph.vertexCount)*sizeof(bool), cudaMemcpyHostToDevice);
//    
    /* Allocate and populate buffers */
}


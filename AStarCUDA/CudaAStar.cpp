/* 
 * File:   main.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:36 AM
 */

#include "GraphData.h"
#include "PQ.h"
#include <math.h>

using namespace std;


void findByAStar(GraphData *graph,int src_vertex,int from_vertex);
void expand(Node **currentNode,GraphData *graph,float goalLat,float goalLong,
        pq_t *frontier,float **gCostArray, float **hCostArray, float **fCostArray);
float computeHeuristic(float latitude,float longitude,float goalLat,float goalLong);
/*
 * 
 */
int main(int argc, char** argv) {

    GraphData graph;
    //CoordinateData* coordinate_values;
    
    generateGraph(&graph);
    cout<< "Vertex Count:" << graph.vertexCount<< endl;
    cout<< "Edge Count:" << graph.edgeCount<< endl;
    
    //getNodeDetails(&graph,2);
    
    
    
    int src_vertex=1, from_vertex=200;
    findByAStar(&graph,src_vertex,from_vertex);
    
    int *vertexArrayDevice, *edgeArrayDevice;
    float *weightArrayDevice, *gCostArrayDevice, *hCostArrayDevice , *fCostArrayDevice;
    bool *visitedArrayDevice;
    //CoordinateData *coordinateArrayDevice;
    
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
    
    
    free((void*)graph.edgeArray);
    free((void*)graph.vertexArray);
    free((void*)graph.weightArray);
    free((void*)graph.latitudes);
    free((void*)graph.longitudes);
    
}

void findByAStar(GraphData *graph,int src_vertex,int goal_vertex){
    if (src_vertex==goal_vertex){
        cout<< "Solution Found:From=To";
    }
    
    bool* visitedArray = (bool*)malloc(graph->vertexCount*sizeof(bool));
    memset(visitedArray,0,graph->vertexCount*sizeof(bool));
    float *gCostArray = (float*)malloc(graph->vertexCount*sizeof(float));
    memset(gCostArray,0,graph->vertexCount*sizeof(float));
    float *hCostArray = (float*)malloc(graph->vertexCount*sizeof(float));
    memset(hCostArray,0,graph->vertexCount*sizeof(float));
    float *fCostArray = (float*)malloc(graph->vertexCount*sizeof(float));
    memset(fCostArray,0,graph->vertexCount*sizeof(float));
    
    pq_status_t pq_stat;
    pq_t frontier = pq_new_queue(graph->vertexCount,&compareNodeCost,&pq_stat);
    
    Node src_node = {src_vertex,0,0};
    Node goal_node = {goal_vertex,0,-1};
    float goalLat = graph->latitudes[goal_vertex-1];
    float goalLong = graph->longitudes[goal_vertex-1];
    pq_enqueue(frontier,(void*)(&src_node),&pq_stat);
    Node* currentNode;
    currentNode = (Node*)pq_dequeue(frontier,&pq_stat);
    cout<<"Vertex:"<<currentNode->nodeNum<<endl;
    while(pq_inspect_next(frontier,&pq_stat)!=NULL){
            currentNode = (Node*)pq_dequeue(frontier,&pq_stat);
            cout<<"Vertex:"<<currentNode->nodeNum<<endl;
            if(currentNode->nodeNum == goal_vertex){
                cout<<"Solution found"<<endl;
                return;
            }else{
                if(!(*(visitedArray+currentNode->nodeNum-1))){
                expand(&currentNode,graph,goalLat,goalLong,&frontier,
                        &gCostArray,&hCostArray,&fCostArray);
                *(visitedArray+currentNode->nodeNum-1)=true;
                }else{
                    continue;
                }
            }        
        }
    free((void*)visitedArray);
    free((void*)gCostArray);
    free((void*)hCostArray);
    free((void*)fCostArray);
}

void expand(Node **currentNode,GraphData *graph,float goalLat,float goalLong,
        pq_t *frontier,float **gCostArray, float **hCostArray, float **fCostArray){
    int earliest_edge = graph->vertexArray[(*(currentNode))->nodeNum -1];
    int final_edge = graph->vertexArray[(*(currentNode))->nodeNum];
    for(int i=earliest_edge;i<final_edge;i++){
        int child = graph->edgeArray[i];
        if(child== (*(currentNode))->parent){
          continue;
        }
        //childNode.gCost = currentNode.gCost+e.weight;
        *(*(gCostArray) + child -1) = *(*(gCostArray) + ((*(currentNode))->nodeNum -1)) + graph->weightArray[i];
        float latitude = graph->latitudes[child-1];
        float longitude = graph->longitudes[child-1];
        //childNode.hCost=Graph.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
        *(*(hCostArray) + child -1) = computeHeuristic(latitude,longitude,goalLat,goalLong);
        //childNode.fCost=childNode.gCost+childNode.hCost;
        *(*(fCostArray) + child -1) = *(*(gCostArray) + child -1) + *(*(hCostArray) + child -1);
        //childNode.parent=currentNode;
        Node childNode = {child,*(*(fCostArray) + child -1),(*(currentNode))->nodeNum};
        pq_enqueue(*frontier,(void*)(&childNode),NULL);
    }
    
    
}

float computeHeuristic(float latitude,float longitude,float goalLat,float goalLong){
        return sqrt(pow((latitude-goalLat)*364812.21,2)+
        pow((longitude-goalLong)*274944.87,2));
        
    }
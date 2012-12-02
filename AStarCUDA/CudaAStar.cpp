/* 
 * File:   main.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:36 AM
 */

#include "GraphData.h"
//#include "PQ.h"
#include "customQueue.h"
#include <math.h>

using namespace std;



void findByAStar(GraphData *graph, int src_vertex, int from_vertex);
float computeHeuristic(float latitude, float longitude, float goalLat, float goalLong);

/*
 * 
 */
int main(int argc, char** argv) {

    GraphData graph;
    generateGraph(&graph);
    cout << "Vertex Count:" << graph.vertexCount << endl;
    cout << "Edge Count:" << graph.edgeCount << endl;

    //getNodeDetails(&graph,1363);



    int src_vertex = 1, from_vertex = 200000;
    findByAStar(&graph, src_vertex, from_vertex);




    free((void*) graph.edgeArray);
    free((void*) graph.vertexArray);
    free((void*) graph.weightArray);
    free((void*) graph.latitudes);
    free((void*) graph.longitudes);

}

void findByAStar(GraphData *graph, int src_vertex, int goal_vertex) {
    if (src_vertex == goal_vertex) {
        cout << "Solution Found:From=To";
    }

    bool* visitedArray = (bool*)malloc(graph->vertexCount * sizeof (bool));
    memset(visitedArray, 0, graph->vertexCount * sizeof (bool));
    float *gCostArray = (float*) malloc(graph->vertexCount * sizeof (float));
    memset(gCostArray, 0, graph->vertexCount * sizeof (float));
    float *hCostArray = (float*) malloc(graph->vertexCount * sizeof (float));
    memset(hCostArray, 0, graph->vertexCount * sizeof (float));
    float *fCostArray = (float*) malloc(graph->vertexCount * sizeof (float));
    memset(fCostArray, 0, graph->vertexCount * sizeof (float));

    
    PQ frontier;                                        //priority queue
    pq_initQueue(&frontier, graph->vertexCount);        //initialized to number of vertices
    Node src_node = {src_vertex, 0, 0};
    heapNode src = {0,src_node};
    float goalLat = graph->latitudes[goal_vertex - 1];
    float goalLong = graph->longitudes[goal_vertex - 1];
    pq_enqueue(src, &frontier);                                 //put src node in pq
    heapNode currentHNode;
    bool solnFound = false;
    while (!pq_empty(&frontier)) {                              //A-Star loop starts
        if(solnFound) break;
        currentHNode = pq_dequeue(&frontier);
        Node* currentNode = &(currentHNode.data);
        if (currentNode->nodeNum == goal_vertex) {
            cout << "Solution found" << endl;
            solnFound = true;
        } else {
            if (!(*(visitedArray + currentNode->nodeNum - 1))) {
                int earliest_edge = graph->vertexArray[currentNode->nodeNum - 1];
                int final_edge = graph->vertexArray[currentNode->nodeNum];
                for (int i = earliest_edge; i < final_edge; i++) {
                    int child = graph->edgeArray[i];
                    if (child == currentNode->parent) {
                        continue;
                    }
                    *(gCostArray + child - 1) = *(gCostArray+currentNode->nodeNum-1)+ graph->weightArray[i];
                    float latitude = graph->latitudes[child - 1];
                    float longitude = graph->longitudes[child - 1];
                    *(hCostArray + child - 1) = computeHeuristic(latitude, longitude, goalLat, goalLong);
                    *(fCostArray + child - 1) = *(gCostArray + child - 1) + *(hCostArray + child - 1);
                    Node childNode = {child, (*(fCostArray + child - 1)), currentNode->nodeNum};
                    heapNode childHNode = {(*(fCostArray + child - 1)),childNode};
                    pq_enqueue(childHNode, &frontier);
                    
                }
                *(visitedArray + currentNode->nodeNum - 1) = true;
            } else {
                continue;
            }
        }
    }
    pq_free(&frontier);
    free((void*) visitedArray);
    free((void*) gCostArray);
    free((void*) hCostArray);
    free((void*) fCostArray);
}



float computeHeuristic(float latitude, float longitude, float goalLat, float goalLong) {
    return sqrt(pow((latitude - goalLat)*364812.21, 2) +
            pow((longitude - goalLong)*274944.87, 2));

}
/* 
 * File:   main.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:36 AM
 */

#include "GraphData.h"
//#include "PQ.h"
#include "pqueue.h"
#include <math.h>

using namespace std;

/*******************CUSTOM QUEUE**************/
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

int pq_empty(PQ *q){
    return q->size;
}
/*******************CUSTOM QUEUE**************/

void findByAStar(GraphData *graph, int src_vertex, int from_vertex);
void expand(Node **currentNode, GraphData *graph, float goalLat, float goalLong,
        pqueue_t** frontier, float **gCostArray, float **hCostArray, float **fCostArray);
float computeHeuristic(float latitude, float longitude, float goalLat, float goalLong);

/*
 * 
 */
int main(int argc, char** argv) {

    GraphData graph;
    //CoordinateData* coordinate_values;

    generateGraph(&graph);
    cout << "Vertex Count:" << graph.vertexCount << endl;
    cout << "Edge Count:" << graph.edgeCount << endl;

    //getNodeDetails(&graph,1363);



    int src_vertex = 1, from_vertex = 200;
    findByAStar(&graph, src_vertex, from_vertex);

    //    int *vertexArrayDevice, *edgeArrayDevice;
    //    float *weightArrayDevice, *gCostArrayDevice, *hCostArrayDevice , *fCostArrayDevice;
    //    bool *visitedArrayDevice;
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

    /*********** APACHE QUEUE**************/
//    pqueue_t *frontier;
//    frontier = pqueue_init(graph->vertexCount, cmp_fCost, get_fCost, set_fCost, get_pos, set_pos);
//    //cout << "coming here" << endl;
//    Node src_node = {src_vertex, 0, 0};
//    //Node goal_node = {goal_vertex,0,-1};
//    float goalLat = graph->latitudes[goal_vertex - 1];
//    float goalLong = graph->longitudes[goal_vertex - 1];
//    pqueue_insert(frontier, &src_node);
//    Node* currentNode;
//    while (pqueue_peek(frontier) != NULL) {
//        currentNode = (Node*) pqueue_pop(frontier);
//        //cout<<"Vertex:"<<currentNode->nodeNum<<endl;
//        if (currentNode->nodeNum == goal_vertex) {
//            cout << "Solution found" << endl;
//            return;
//        } else {
//            if (!(*(visitedArray + currentNode->nodeNum - 1))) {
//                //                expand(&currentNode,graph,goalLat,goalLong,&frontier,
//                //                        &gCostArray,&hCostArray,&fCostArray);
//                int earliest_edge = graph->vertexArray[currentNode->nodeNum - 1];
//                int final_edge = graph->vertexArray[currentNode->nodeNum];
//                for (int i = earliest_edge; i < final_edge; i++) {
//                    int child = graph->edgeArray[i];
//                    if (child == currentNode->parent) {
//                        continue;
//                    }
//                    //childNode.gCost = currentNode.gCost+e.weight;
//                    //*(*(gCostArray) + child - 1) = *(*(gCostArray) + ((*(currentNode))->nodeNum - 1)) + graph->weightArray[i];
//                    *(gCostArray + child - 1) = *(gCostArray+currentNode->nodeNum-1)+ graph->weightArray[i];
//                    //cout<<"gcost:"<<*(gCostArray + child - 1);
//                    float latitude = graph->latitudes[child - 1];
//                    float longitude = graph->longitudes[child - 1];
//                    //childNode.hCost=Graph.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
//                    *(hCostArray + child - 1) = computeHeuristic(latitude, longitude, goalLat, goalLong);
//                    //cout<<"hcost:"<<*(hCostArray + child - 1);
//                    //childNode.fCost=childNode.gCost+childNode.hCost;
//                    *(fCostArray + child - 1) = *(gCostArray + child - 1) + *(hCostArray + child - 1);
//                    //cout<<"fcost:"<<*(fCostArray + child - 1);
//                    //childNode.parent=currentNode;
//                    Node childNode = {child, (*(fCostArray + child - 1)), currentNode->nodeNum};
//                    //cout << "childNode info:" << childNode.nodeNum << ":" << childNode.fCost << ":" << childNode.parent << endl;
//                    pqueue_insert(frontier, (void*) (&childNode));
//                    Node* test = (Node*)pqueue_peek(frontier);
//                    cout<<"after insert"<<test->nodeNum<<"--"<<test->fCost<<endl;
//                
//                }
//                *(visitedArray + currentNode->nodeNum - 1) = true;
//            } else {
//                continue;
//            }
//        }
//    }
    /*********** APACHE QUEUE**************/
    
    /**************CUSTOM QUEUE*******/
    PQ frontier;
    pq_initQueue(&frontier, graph->vertexCount);
    Node src_node = {src_vertex, 0, 0};
    heapNode src = {0,src_node};
    //Node goal_node = {goal_vertex,0,-1};
    float goalLat = graph->latitudes[goal_vertex - 1];
    float goalLong = graph->longitudes[goal_vertex - 1];
    pq_enqueue(src, &frontier);
    cout<<"froniter not emptyt"<<endl;
    heapNode currentHNode;
    while (pq_empty(&frontier)) {
        //cout<<"froniter not emptyt"<<endl;
        currentHNode = pq_dequeue(&frontier);
        Node* currentNode = &(currentHNode.data);
        //cout<<"node output:"<<currentNode->nodeNum<<endl;
        if (currentNode->nodeNum == goal_vertex) {
            cout << "Solution found" << endl;
            return;
        } else {
            if (!(*(visitedArray + currentNode->nodeNum - 1))) {
                //                expand(&currentNode,graph,goalLat,goalLong,&frontier,
                //                        &gCostArray,&hCostArray,&fCostArray);
                int earliest_edge = graph->vertexArray[currentNode->nodeNum - 1];
                int final_edge = graph->vertexArray[currentNode->nodeNum];
                for (int i = earliest_edge; i < final_edge; i++) {
                    int child = graph->edgeArray[i];
                    if (child == currentNode->parent) {
                        continue;
                    }
                    //childNode.gCost = currentNode.gCost+e.weight;
                    //*(*(gCostArray) + child - 1) = *(*(gCostArray) + ((*(currentNode))->nodeNum - 1)) + graph->weightArray[i];
                    *(gCostArray + child - 1) = *(gCostArray+currentNode->nodeNum-1)+ graph->weightArray[i];
                    //cout<<"gcost:"<<*(gCostArray + child - 1);
                    float latitude = graph->latitudes[child - 1];
                    float longitude = graph->longitudes[child - 1];
                    //childNode.hCost=Graph.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
                    *(hCostArray + child - 1) = computeHeuristic(latitude, longitude, goalLat, goalLong);
                    //cout<<"hcost:"<<*(hCostArray + child - 1);
                    //childNode.fCost=childNode.gCost+childNode.hCost;
                    *(fCostArray + child - 1) = *(gCostArray + child - 1) + *(hCostArray + child - 1);
                    //cout<<"fcost:"<<*(fCostArray + child - 1);
                    //childNode.parent=currentNode;
                    Node childNode = {child, (*(fCostArray + child - 1)), currentNode->nodeNum};
                    //cout << "childNode info:" << childNode.nodeNum << ":" << childNode.fCost << ":" << childNode.parent << endl;
                    //pqueue_insert(frontier, (void*) (&childNode));
                    heapNode childHNode = {(*(fCostArray + child - 1)),childNode};
                    pq_enqueue(childHNode, &frontier);
                    //Node* test = (Node*)pqueue_peek(frontier);
                    //cout<<"after insert"<<test->nodeNum<<"--"<<test->fCost<<endl;
                
                }
                *(visitedArray + currentNode->nodeNum - 1) = true;
            } else {
                continue;
            }
        }
    }
    /**************CUSTOM QUEUE*******/
    
    free((void*) visitedArray);
    free((void*) gCostArray);
    free((void*) hCostArray);
    free((void*) fCostArray);
}

void expand(Node **currentNode, GraphData *graph, float goalLat, float goalLong,
        pqueue_t** frontier, float **gCostArray, float **hCostArray, float **fCostArray) {
    int earliest_edge = graph->vertexArray[(*(currentNode))->nodeNum - 1];
    int final_edge = graph->vertexArray[(*(currentNode))->nodeNum];
    for (int i = earliest_edge; i < final_edge; i++) {
        int child = graph->edgeArray[i];
        if (child == (*(currentNode))->parent) {
            continue;
        }
        //childNode.gCost = currentNode.gCost+e.weight;
        *(*(gCostArray) + child - 1) = *(*(gCostArray) + ((*(currentNode))->nodeNum - 1)) + graph->weightArray[i];
        float latitude = graph->latitudes[child - 1];
        float longitude = graph->longitudes[child - 1];
        //childNode.hCost=Graph.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
        *(*(hCostArray) + child - 1) = computeHeuristic(latitude, longitude, goalLat, goalLong);
        //childNode.fCost=childNode.gCost+childNode.hCost;
        *(*(fCostArray) + child - 1) = *(*(gCostArray) + child - 1) + *(*(hCostArray) + child - 1);
        //childNode.parent=currentNode;
        Node childNode = {child, *(*(fCostArray) + child - 1), (*(currentNode))->nodeNum};
        cout << "childNode info:" << childNode.nodeNum << ":" << childNode.fCost << ":" << childNode.parent << endl;
        pqueue_insert(*frontier, (void*) (&childNode));
    }

    Node* node = (Node*) pqueue_peek(*frontier);
    cout << "top info:" << node->nodeNum << endl;
}

float computeHeuristic(float latitude, float longitude, float goalLat, float goalLong) {
    return sqrt(pow((latitude - goalLat)*364812.21, 2) +
            pow((longitude - goalLong)*274944.87, 2));

}
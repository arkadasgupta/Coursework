/* 
 * File:   newmain.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:21 PM
 */

#include <cstdlib>
#include "PQ.h"
#include<iostream>

//using namespace std;

typedef struct{
    int nodeNum;
    float fCost;
    int parent;
}Node;

bool compareNodeCost(void* a,void* b){
    return ((Node*)a)->fCost<((Node*)b)->fCost;
}



using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    Node src_node = {1,2,0};
    Node goal_node = {2,1,-1};
    
    pq_status_t pq_stat;
    pq_t frontier = pq_new_queue(5,&compareNodeCost,&pq_stat);
    pq_enqueue(frontier,(void*)(&src_node),&pq_stat);
    pq_enqueue(frontier,(void*)(&goal_node),&pq_stat);
    Node* currentNode = (Node*)pq_inspect_next(frontier,&pq_stat); //WORKING
    cout<<currentNode->nodeNum<<endl;
    Node* currentNode = (Node*)pq_dequeue(frontier,&pq_stat); //GIVING SEGFAULT FROM INSIDE THIS
    cout<<currentNode->nodeNum<<endl;
    return 0;
}


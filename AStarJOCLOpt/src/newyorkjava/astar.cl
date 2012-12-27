/**
 * @author arka
 */

#define LAT_RAD 364812.22F
#define LONG_RAD 274944.87F
#define POWER 2

//typedef struct{
//    int nodeNum;
//    float fCost;
//    int parent;
//    size_t pos;
//}Node;
//
//typedef struct heapData {
//    int nodeNum;
//    int parent;
//} heapData;
// 
//typedef struct heapNode {
//    float value;
//    Node data;               //dummy
//} heapNode;
// 
//typedef struct PQ {
//    heapNode* heap;
//    int size;
//} PQ;

void insert(int vertex, global int* pq, int size,global float *fCostArray) {
    int idx;
    int tmp;
    idx = size + 1;
    pq[idx] = vertex;
    //while (fCostArray[idx] > fCostArray[idx/2] && idx > 1) {
    while (fCostArray[pq[idx]-1] < fCostArray[pq[idx/2]-1] && idx > 1) {    
    tmp = pq[idx];
    pq[idx] = pq[idx/2];
    pq[idx/2] = tmp;
    idx /= 2;
    }
}
 
void shiftdown(global int* pq, int size, int idx,global float *fCostArray) {
    int cidx;        //index for child
    int tmp;
    for (;;) {
        cidx = idx*2;
        if (cidx > size) {
            break;   //it has no child
        }
        if (cidx < size) {
            //if (fCostArray[cidx] < fCostArray[cidx+1]) {
            if (fCostArray[pq[cidx]-1] > fCostArray[pq[cidx+1]-1]) {    
                ++cidx;
            }
        }
        //swap if necessary
        //if (fCostArray[cidx] > fCostArray[idx]) {
        if (fCostArray[pq[cidx]-1] < fCostArray[pq[idx]-1]) {    
            tmp = pq[cidx];
            pq[cidx] = pq[idx];
            pq[idx] = tmp;
            idx = cidx;
        } else {
            break;
        }
    }
}
 
int removeMin(global int* pq, int size,global float *fCostArray) {
    //int cidx;
    int rv = pq[1];
    
    pq[1] = pq[size];
    --size;
    shiftdown(pq, size, 1,fCostArray);
    return rv;
}
void pq_enqueue(int vertex, global int* pq,global float *fCostArray) {
    insert(vertex, pq, pq[0],fCostArray);
    pq[0]=pq[0]+1;
}
 
int pq_dequeue(global int* pq, global float *fCostArray) {
   int rv = removeMin(pq, pq[0],fCostArray);
   pq[0]=pq[0]-1;
   return rv; 
}
int pq_peek(global int* pq) {
   int rv = pq[1];
   //--q->size;
   return rv; 
} 

bool pq_empty(global int* pq){
    return (pq[0]==0);
}



inline float computeHeuristic(float srclat,float srclong,float goallat, float goallong){
    return sqrt(pow((goallat-srclat)*LAT_RAD,POWER)+ pow((goallong-srclong)*LONG_RAD,POWER));
}

kernel void astar(global int* vertArr, global int* edgeArr, global float* weightArr,
                global float* latitudeArr, global float* longitudeArr,
                global float* gCostArr, global float* hCostArr, global float* fCostArr,
                global int* visitedArray, global int* priorityQueue,
                global int* childList, global float* childfCost,
                const int nodeNum, const int goalNodeNum, const int numThreads){
    int id = get_global_id(0);                  //returns thread id. 
    if (id >= numThreads) {
        return;
    }
    int firstEdge = vertArr[nodeNum-1];         
    int nextNodeEdge = vertArr[nodeNum];
    int children = nextNodeEdge-firstEdge;
    if(id>=children){
        childList[id]=0;                        //will be handled in host
        childfCost[id]=0;
    }
    else{
    int childNode = edgeArr[firstEdge+id];

    float gCost = gCostArr[nodeNum-1]+weightArr[firstEdge+id];
    float hCost = computeHeuristic(latitudeArr[childNode-1],longitudeArr[childNode-1],
                    latitudeArr[goalNodeNum-1],longitudeArr[goalNodeNum-1]);
    float fCost = gCost + hCost;
    gCostArr[childNode-1]=gCost;
    hCostArr[childNode-1]=hCost;
    fCostArr[childNode-1]=fCost;
    childList[id]=childNode;
    childfCost[id]=fCost;
    }
    barrier(CLK_GLOBAL_MEM_FENCE);
}

kernel void testKernel(global int* vertArr, global int* edgeArr, global float* weightArr,
                global float* latitudeArr, global float* longitudeArr,
                global float* gCostArr, global float* hCostArr, global float* fCostArr,
                global int* visitedArray, global int* priorityQueue,global int* found,
                //global int* childList, global float* childfCost,
                const int goalNodeNum, const int numThreads){
    int id = get_global_id(0);                  //returns thread id. 
    local int currentNode;
    local int sharedChildList[8];                     //local list updated by each thread
    local int childUpdateFlag[8];
    if(id==0){
       currentNode =  pq_dequeue(priorityQueue, fCostArr);      //topmost node
        *found=currentNode;
       //*found=fCostArr[currentNode-1]; 
       visitedArray[currentNode-1] = 1;                         //mark visited
       if(currentNode==goalNodeNum){
          *found=currentNode;                                             //goal found
       }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    if(*found!=goalNodeNum){
        childUpdateFlag[id]=0;
        int firstEdge = vertArr[currentNode-1];                 //first child of node
        int nextNodeEdge = vertArr[currentNode];                //first child of next node
        int children = nextNodeEdge-firstEdge;                  //number of children of current node
        if(id>=children){
            sharedChildList[id]=0;                        //thread id exceeds number of children
            childUpdateFlag[id]=0;
        }
        else{
        int childNode = edgeArr[firstEdge+id];              //each thread picks a child

        float gCost = gCostArr[currentNode-1]+weightArr[firstEdge+id];
        float hCost = computeHeuristic(latitudeArr[childNode-1],longitudeArr[childNode-1],
                        latitudeArr[goalNodeNum-1],longitudeArr[goalNodeNum-1]);
        float fCost = gCost + hCost;
        if(visitedArray[childNode-1]==0){           //node is either in frontier or hasn't yet been encountered
            if(gCostArr[childNode-1]==0){           //node is not visited yet
                gCostArr[childNode-1]=gCost;
                hCostArr[childNode-1]=hCost;
                fCostArr[childNode-1]=fCost;
                sharedChildList[id]=childNode;
                childUpdateFlag[id]=1;
            }else{
                if(gCost<gCostArr[childNode-1]){    //node is in frontier and lower cost path is found
                   gCostArr[childNode-1]=gCost;
                   fCostArr[childNode-1]=fCost;
                   sharedChildList[id]=childNode;
                   childUpdateFlag[id]=0;
                }
            }
          }else{
            sharedChildList[id]=childNode;
            childUpdateFlag[id]=0;
          }
        }
        barrier(CLK_LOCAL_MEM_FENCE|CLK_GLOBAL_MEM_FENCE);
        if(id==0){
            for(int i=0;i<children;i++){
                int vertex = sharedChildList[i];
                if(visitedArray[vertex-1]==0 && childUpdateFlag[i]==1){
                    pq_enqueue(vertex, priorityQueue,fCostArr); //if thread id 0 put all children back in queue
                }
            }
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
}

kernel void initializeCostArrays(global float* gCostArr, global float* hCostArr, global float* fCostArr,
                                 global int* visitedArray,global int* priorityQueue,global int* found,
                                const int srcVert,const int size){
    // access thread id
    int tid = get_global_id(0);
    visitedArray[tid] = 0;
    priorityQueue[tid] = 0;
    gCostArr[tid] = 0.0f;
    hCostArr[tid] = 0.0f;
    fCostArr[tid] = 0.0f;
    
    //maintain size of the pq in index 0, which is unused in this implementation
    
    if(tid==0){
        *found=0;
        priorityQueue[tid] = size;
        //priorityQueue[tid+1] = srcVert;
    }
    if(tid==srcVert){
        priorityQueue[1] = srcVert;
    }
}
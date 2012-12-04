// created on Dec 3, 2012

/**
 * @author arka
 */

#define LAT_RAD 364812.22F
#define LONG_RAD 274944.87F
#define POWER 2

inline float computeHeuristic(float srclat,float srclong,float goallat, float goallong){
    return sqrt(pow((goallat-srclat)*LAT_RAD,POWER)+ pow((goallong-srclong)*LONG_RAD,POWER));
}

kernel void astar(global int* vertArr, global int* edgeArr, global float* weightArr,
                    global float* latitudeArr, global float* longitudeArr,
                    global float* gCostArr, global float* hCostArr, global float* fCostArr,
                    global int* visitedArray, global int* childList, global float* childfCost,
                    const int nodeNum, const int goalNodeNum, const int numThreads) {
    int id = get_global_id(0);                  //returns thread id. 
    if (id >= numThreads) {
        return;
    }
//    if(id==0){
//       visitedArray[nodeNum-1] =1;
//    }
//    int firstEdge = vertArr[nodeNum-1];
//    int nextNodeEdge = vertArr[nodeNum];
//    if(id>=(nextNodeEdge-firstEdge)){
//        return;
//    }
//    int childNode = edgeArr[firstEdge+id];
//    if(visitedArray[childNode-1]){
//        return;
//    }
//    float gCost = gCostArr[nodeNum-1]+weightArr[firstEdge+id];
//    float hCost = computeHeuristic(latitudeArr[childNode-1],longitudeArr[childNode-1],
//                    latitudeArr[goalNodeNum-1],longitudeArr[goalNodeNum-1]);
//    float fCost = gCost + hCost;
//    gCostArr[childNode-1]=gCost;
//    hCostArr[childNode-1]=hCost;
//    fCostArr[childNode-1]=fCost;
    childList[0]=200;
    childfCost[0]=1;
    barrier(CLK_GLOBAL_MEM_FENCE);
}

kernel void test(global int* vertArr, global int* edgeArr, global float* weightArr,
                global float* latitudeArr, global float* longitudeArr,
                global float* gCostArr, global float* hCostArr, global float* fCostArr,
                global int* visitedArray, global int* childList, global float* childfCost,
                const int nodeNum, const int goalNodeNum, const int numThreads){
    int id = get_global_id(0);                  //returns thread id. 
    if (id >= numThreads) {
        return;
    }
//    if(id==0){
//       visitedArray[nodeNum-1] =1;
//    }
    int firstEdge = vertArr[nodeNum-1];
    int nextNodeEdge = vertArr[nodeNum];
    int children = nextNodeEdge-firstEdge;
    if(id>=children){
        childList[id]=0;
        childfCost[id]=0;
    }
    else{
    int childNode = edgeArr[firstEdge+id];
//    if(visitedArray[childNode-1]==1){
//        return;
//    }
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
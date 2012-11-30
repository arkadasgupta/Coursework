// created on Nov 29, 2012

/**
 * @author arka
 */

typedef struct{
    float latitude;
    float longitude;
} CoordinateData;

float computeHeuristic(int parent, int child, Coordinate* coordinateArray){
    CoordinateData parentCoordinate = *(coordinateArray+parent-1);
    CoordinateData childCoordinate = *(coordinateArray+child-1);
    //return Math.sqrt(Math.pow((lat1 - lat2) * LAT, 2) + Math.pow((lng1 - lng2) * LONG, 2));
    return sqrt(pow((parentCoordinate.latitude-childCoordinate.latitude)*364812.21,2)+
        pow((parentCoordinate.longitude-childCoordinate.longitude)*274944.87,2));
}

//HOW TO IMPLEMENT THESE??
void extractFromPriorityQueue();
void insertIntoPriorityQueue();
//HOW TO IMPLEMENT THESE??

__kernel void startAStar(//necessary inputs) {
    int id = get_global_id(0);
    int currentNode = extractFromPriorityQueue(priorityQueueArray,id);
    if(currentNode==0){
        return;
    }
    int earliest_edge = vertexArray[currentNode-1];
    int next_vertex_edge = vertexArray[currentNode];
    for(int i=earliest_edge;i<next_vertex_edge;i++){
        int child = edgeArray[i];
        float weight = weightArray[i];
        gCostArray[child-1] = gCostArray[currentNode] + weight;
        hCostArray[child-1] = computeHeuristic(currentNode,child,coordinateArray);
        fCostArray[child-1] = gCostArray[child-1] + hCostArray[child-1];
        insertIntoPriorityQueue(priorityQueueArray,child);
    }
}


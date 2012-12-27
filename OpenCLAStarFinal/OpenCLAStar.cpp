/* 
 * File:   OpenCLAStar.cpp
 * Author: arka
 *
 * Created on December 5, 2012, 10:06 PM
 */

#include <cstdlib>
//#include "oclAStarKernel.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstddef>
#include "OCLCommon.h"
#include <queue>
#include <functional>
#include <time.h>

using namespace std;

//Overload the < operator.
bool operator< (const Node& node1, const Node &node2)
{
	return node1.fCost > node2.fCost;	
}
//Overload the > operator.
bool operator> (const Node& node1, const Node &node2)
{
	return node1.fCost < node2.fCost;	
}

static cl_device_id getMaxFlopsDev(cl_context cxGPUContext)
{
    size_t szParmDataBytes;
    cl_device_id* cdDevices;

    // get the list of GPU devices associated with context
    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, 0, NULL,
            &szParmDataBytes);
    cdDevices = (cl_device_id*) malloc(szParmDataBytes);
    size_t device_count = szParmDataBytes / sizeof(cl_device_id);

    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, szParmDataBytes,
            cdDevices, NULL);

    cl_device_id max_flops_device = cdDevices[0];
    int max_flops = 0;

    size_t current_device = 0;

    // CL_DEVICE_MAX_COMPUTE_UNITS
    cl_uint compute_units;
    clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(compute_units), &compute_units, NULL);

    // CL_DEVICE_MAX_CLOCK_FREQUENCY
    cl_uint clock_frequency;
    clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_CLOCK_FREQUENCY,
            sizeof(clock_frequency), &clock_frequency, NULL);

    max_flops = compute_units * clock_frequency;
    ++current_device;

    while (current_device < device_count)
    {
        // CL_DEVICE_MAX_COMPUTE_UNITS
        cl_uint compute_units;
        clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_COMPUTE_UNITS,
                sizeof(compute_units), &compute_units, NULL);

        // CL_DEVICE_MAX_CLOCK_FREQUENCY
        cl_uint clock_frequency;
        clGetDeviceInfo(cdDevices[current_device],
                CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency),
                &clock_frequency, NULL);

        int flops = compute_units * clock_frequency;
        if (flops > max_flops)
        {
            max_flops = flops;
            max_flops_device = cdDevices[current_device];
        }
        ++current_device;
    }

    free(cdDevices);

    return max_flops_device;
}


void findByAStar( GraphData* graph, int src_vert, int goal_vert){
    
    int source = 1;
    int goal = 264340;
    
    cl_platform_id platform;
    cl_int errNum;
    
    
    cl_uint numPlatforms;
    errNum = clGetPlatformIDs(1, &platform, &numPlatforms);
    cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };
    
    cl_context context = clCreateContextFromType(cps, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    
    cl_device_id deviceId=getMaxFlopsDev(context);
    size_t maxWorkGroupSize;
    clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
    int remainder=graph->vertexCount%maxWorkGroupSize;
    size_t globalWorkSizeForAlloc = (remainder==0)? graph->vertexCount : graph->vertexCount + maxWorkGroupSize - remainder;
    
    cl_command_queue commandQueue;
    commandQueue = clCreateCommandQueue(context, deviceId, 0, &errNum );
    if (errNum != CL_SUCCESS)
    {
        cout<<"Something wrong"<<endl;
    }
    cl_program program = loadAndBuildProgram( context, "astar.cl" );
        
    cl_mem vertexArrayDevice;
    cl_mem edgeArrayDevice;
    cl_mem weightArrayDevice;
    cl_mem latitudeArrayDevice;
    cl_mem longitudeArrayDevice;
    cl_mem priorityQueueArrayDevice;
    cl_mem visitedArrayDevice;
    cl_mem gCostArrayDevice;
    cl_mem hCostArrayDevice;
    cl_mem fCostArrayDevice;
    cl_mem temporaryListDevice;
    cl_mem temporaryFCostDevice;
    cl_mem solnFoundDevice;
    
    size_t localWorkSize = 8, globalWorkSize = 8;
    
//    float costArray[graph->vertexCount];
//    memset(costArray,0,sizeof(float)*graph->vertexCount);
//    int visitedArray1[graph->vertexCount];
//    memset(visitedArray1,0,sizeof(int)*graph->vertexCount);

    allocateOCLBuffers( context, commandQueue, graph, //costArray, visitedArray1,
                        &vertexArrayDevice, &edgeArrayDevice, &weightArrayDevice,
                        &latitudeArrayDevice, &longitudeArrayDevice,
                        &priorityQueueArrayDevice,&visitedArrayDevice, 
                        &gCostArrayDevice, &hCostArrayDevice, &fCostArrayDevice,
                        &temporaryListDevice,&temporaryFCostDevice,&solnFoundDevice
                        ,globalWorkSizeForAlloc);
    
    int pq_size=1;
    cl_kernel allocationKernel;
    allocationKernel = clCreateKernel(program, "initializeCostArrays", &errNum);
    errNum |= clSetKernelArg(allocationKernel, 0, sizeof(cl_mem), &gCostArrayDevice);
    errNum |= clSetKernelArg(allocationKernel, 1, sizeof(cl_mem), &hCostArrayDevice);
    errNum |= clSetKernelArg(allocationKernel, 2, sizeof(cl_mem), &fCostArrayDevice);
    errNum |= clSetKernelArg(allocationKernel, 3, sizeof(cl_mem), &visitedArrayDevice);
    errNum |= clSetKernelArg(allocationKernel, 4, sizeof(cl_mem), &priorityQueueArrayDevice);
    errNum |= clSetKernelArg(allocationKernel, 5, sizeof(cl_mem), &temporaryListDevice);
    errNum |= clSetKernelArg(allocationKernel, 6, sizeof(cl_mem), &temporaryFCostDevice);
    errNum |= clSetKernelArg(allocationKernel, 7, sizeof(cl_mem), &solnFoundDevice);
    errNum |= clSetKernelArg(allocationKernel, 8, sizeof(int), &source);
    errNum |= clSetKernelArg(allocationKernel, 9, sizeof(int), &pq_size);
    
    errNum = clEnqueueNDRangeKernel(commandQueue, allocationKernel, 1, NULL, &globalWorkSizeForAlloc, &maxWorkGroupSize,
                                    0, NULL, NULL);
    
    
    cl_kernel astarKernel;
    //astarKernel = clCreateKernel(program, "astar", &errNum);
    astarKernel = clCreateKernel(program, "testKernel1", &errNum);
    
    errNum |= clSetKernelArg(astarKernel, 0, sizeof(cl_mem), &vertexArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 1, sizeof(cl_mem), &edgeArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 2, sizeof(cl_mem), &weightArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 3, sizeof(cl_mem), &latitudeArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 4, sizeof(cl_mem), &longitudeArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 5, sizeof(cl_mem), &gCostArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 6, sizeof(cl_mem), &hCostArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 7, sizeof(cl_mem), &fCostArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 8, sizeof(cl_mem), &visitedArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 9, sizeof(cl_mem), &priorityQueueArrayDevice);
    errNum |= clSetKernelArg(astarKernel, 10, sizeof(cl_mem), &solnFoundDevice);
    errNum |= clSetKernelArg(astarKernel, 11, sizeof(cl_mem), &temporaryListDevice);
    errNum |= clSetKernelArg(astarKernel, 12, sizeof(cl_mem), &temporaryFCostDevice);
    errNum |= clSetKernelArg(astarKernel, 13, sizeof(int), &goal);
    errNum |= clSetKernelArg(astarKernel, 14, sizeof(int), &localWorkSize);
    //checkError(errNum, CL_SUCCESS);
    
//    bool* visitedArray = (bool*)malloc(graph->vertexCount * sizeof (bool));
//    memset(visitedArray, 0, graph->vertexCount * sizeof (bool));
    
    int* tempChildList = (int*)malloc(8 * sizeof (int));     
    int* tempfCostList = (int*)malloc(8 * sizeof (float));
    
//    priority_queue<Node, vector<Node>,less<vector<Node>::value_type> > frontier;
//    Node src_node = {src_vert, 0, 0};
//    //heapNode src = {0,src_node};
//    frontier.push(src_node);                                 //put src node in pq
//    //heapNode chk = pq_peek(&frontier);
//    //Node* chkr = &(chk.data);
//    //Node chk = frontier.top();
//    //heapNode currentHNode;
    int* solnFound = (int*)malloc(sizeof (int)); ;
    clock_t begin, end;
    double time_spent;
    begin = clock();
    int iterCount=0;
    while (true) {                              //A-Star loop starts
//        if(solnFound) break;
//        //currentHNode = pq_dequeue(&frontier);
//        Node currentNode = frontier.top();
//        frontier.pop();
//        visitedArray[currentNode.nodeNum - 1] = true;
//        if (currentNode.nodeNum == goal_vert) {
//            end = clock();
//                time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//            cout << "Solution found:" <<time_spent<< endl;
//            /* here, do your time-consuming job */
//        
//            solnFound = true;
//        } else {
//            int currNode = currentNode.nodeNum;
//            errNum |= clSetKernelArg(astarKernel, 11, sizeof(int), &currNode);
//            errNum = clEnqueueNDRangeKernel(commandQueue, astarKernel, 1, 0, &globalWorkSize, &localWorkSize,
//                                               0, NULL, NULL);
//            cl_event readDone;
//            errNum = clEnqueueReadBuffer( commandQueue, temporaryListDevice, CL_FALSE, 0, sizeof(int) * 8,
//                                      tempChildList, 0, NULL, &readDone);
//            clWaitForEvents(1, &readDone);
//            errNum = clEnqueueReadBuffer( commandQueue, temporaryFCostDevice, CL_FALSE, 0, sizeof(float) * 8,
//                                      tempfCostList, 0, NULL, &readDone);
//            clWaitForEvents(1, &readDone);
//            for(int i=0;i<8;i++){
//                if(tempChildList[i]==0){
//                    break;
//                }
//                if(!visitedArray[tempChildList[i] - 1]){
//                    int child = tempChildList[i];
//                    float fCost = tempfCostList[i];
//                    Node childNode = {child, fCost, currentNode.nodeNum};
//                    //heapNode childHNode = {fCost,childNode};
//                    //pq_enqueue(childHNode, &frontier);
//                    frontier.push(childNode);
//                }
//            }
//        }
          errNum = clEnqueueNDRangeKernel(commandQueue, astarKernel, 1, 0, &globalWorkSize, &localWorkSize,
                                               0, NULL, NULL);
          cl_event readDone;
          errNum = clEnqueueReadBuffer( commandQueue, solnFoundDevice, CL_FALSE, 0, sizeof(int),
                                      solnFound, 0, NULL, &readDone);
          clWaitForEvents(1, &readDone);
          //cout<<"itercount:"<<(++iterCount)<<"and:found:"<<(*solnFound)<<"goal:"<<goal<<endl;
          if(*solnFound==goal){
              end = clock();
              time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
              cout << "Solution found:" <<time_spent<< endl;
              break;
          }
    }
    free(solnFound);
    free(tempChildList);
    free(tempfCostList);
}


int main(int argc, char** argv) {

    GraphData graph;
    generateGraph(&graph);
    cout << "Vertex Count:" << graph.vertexCount << endl;
    cout << "Edge Count:" << graph.edgeCount << endl;
    
    int src_vertex = 1, from_vertex = 200;
    findByAStar(&graph, src_vertex, from_vertex);

    free((void*) graph.edgeArray);
    free((void*) graph.vertexArray);
    free((void*) graph.weightArray);
    free((void*) graph.latitudes);
    free((void*) graph.longitudes);
    
    return 0;
}


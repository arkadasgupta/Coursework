/* 
 * File:   oclAstar.cpp
 * Author: arka
 *
 * 
 */

#include <cstdlib>
//#include "oclAStarKernel.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include<CL/cl.h>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstddef>


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
/*
 * 
 */

typedef struct{
    double latitude;
    double longitude;
} CoordinateData;

typedef struct{
    int adjList[8];
    float weight[8];
} AdjacencyList;

/********
 *   CODE TAKEN FROM http://www.cplusplus.com/faq/sequences/strings/split/
 */
struct split
{
  enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& split(
  Container&                                 result,
  const typename Container::value_type&      s,
  typename Container::value_type::value_type delimiter
   )
{
  result.clear();
  std::istringstream ss( s );
  while (!ss.eof())
  {
    typename Container::value_type field;
    getline( ss, field, delimiter );
    if (field.empty()) continue;
    result.push_back( field );
  }
  return result;
}
/******  
 * CODE TAKEN FROM http://www.cplusplus.com/faq/sequences/strings/split/
 */


void generateGraph(GraphData *graph, CoordinateData** coordinates){
    std::ifstream graphFile("USA-road-d.NY.gr");
    std::string line,buf;
    AdjacencyList* adj_list;
    int gr_lineCount = 1;
    while(graphFile.good()){
        getline(graphFile,line);
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){

            std::vector<std::string> graph_info;

            split( graph_info, line, ' ' );
            int num_vertices = atoi((graph_info[2]).c_str());
            graph->vertexCount = num_vertices;
            graph->edgeCount = atoi((graph_info[3]).c_str());
            adj_list = (AdjacencyList*)malloc((num_vertices)*(sizeof (AdjacencyList)));
            memset(adj_list, 0 ,num_vertices*(sizeof (AdjacencyList)));
            
            continue;
        }
        
        if(gr_lineCount > graph->edgeCount)
            break;
        std::vector<std::string> edge_info;
        split( edge_info, line, ' ' );
        //std::cout << line;
        int from_vertex = atoi((edge_info[1]).c_str());
        int to_vertex = atoi((edge_info[2]).c_str());
        float weight = atof((edge_info[3]).c_str());
        AdjacencyList temp_adj = adj_list[from_vertex-1];
        for(int i=0;i<8;i++){
            if(temp_adj.adjList[i]==0){
                temp_adj.adjList[i]=to_vertex;
                temp_adj.weight[i]=weight;
                break;
            }
        }
         adj_list[from_vertex-1] = temp_adj;
        gr_lineCount++;
    }
    graphFile.close();
    
    graph->vertexArray = (int*) malloc(graph->vertexCount * sizeof(int));
    graph->edgeArray = (int*)malloc(graph->edgeCount * sizeof(int));
    graph->weightArray = (float*)malloc(graph->edgeCount * sizeof(float));
    int current_edge = 0;
    
    for(int i=0;i<graph->vertexCount;i++){
        AdjacencyList temp_adj = *(adj_list+i);
        graph->vertexArray[i] = current_edge;
        for(int j=0;j<8;j++){
            if(temp_adj.adjList[j]!=0){
            graph->edgeArray[current_edge] = temp_adj.adjList[j];
            graph->weightArray[current_edge] = temp_adj.weight[j];
            current_edge++;
            }else{
                break;
            }
        }
    }
    free(adj_list);
    
    std::ifstream coordFile("USA-road-d-mod.NY.co");
    int co_lineCount = 1;
    int num_vertices_co = 0;
    while(coordFile.good()){
        getline(coordFile,line);
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            //std::istringstream ss(line);
            std::vector<std::string> coordinate_info;
//            while (ss >> buf)
//                coordinate_info.push_back(buf);
            split( coordinate_info, line, ' ' );
            num_vertices_co = atoi((coordinate_info[4]).c_str());
            //std::cout << "value of vertieces" << num_vertices_co << std::endl;
            *coordinates = (CoordinateData*)malloc(num_vertices_co*(sizeof (CoordinateData)));
            memset(*coordinates, 0 ,num_vertices_co*(sizeof (CoordinateData)));
            continue;
        }
        //std::istringstream ss(line);
        if(co_lineCount > num_vertices_co)
            break;
        std::vector<std::string> coordinate_info;
//        while (ss >> buf)
//             coordinate_info.push_back(buf);
        split( coordinate_info, line, ' ' );
        //std::cout << line << std::endl;
        int vertex_num = atoi((coordinate_info[1]).c_str());
        double longitude = atof((coordinate_info[2]).c_str());
        double latitude = atof((coordinate_info[3]).c_str());
        CoordinateData data = {latitude,longitude};
        //std::cout << "Struct data:" << data.latitude << ":" << data.longitude << std::endl;
        *(*(coordinates)+vertex_num-1) = data;
      co_lineCount++;  
    }
    coordFile.close();

//    int earliest_edge = graph->vertexArray[1];
////    //handle boundary case for final node
//    int final_edge = graph->vertexArray[2];
//    std::cout<<"Edge values:"<<earliest_edge<<":"<<final_edge<<std::endl;
}

void getNodeDetails(GraphData *graph, CoordinateData *coordinate_values, int vertex){
    CoordinateData coordinates = *(coordinate_values+vertex-1);
    std::cout << "Coordinates:" << coordinates.latitude << ":" << coordinates.longitude << std::endl;
    int earliest_edge = graph->vertexArray[vertex-1];
//    //handle boundary case for final node
    int final_edge = graph->vertexArray[vertex];
    //std::cout<<"Edge values:"<<earliest_edge<<":"<<final_edge<<std::endl;
    for(int i=earliest_edge;i<final_edge;i++)
        std::cout << "Adjacency:" << graph->edgeArray[i] << "Weight:" << graph->weightArray[i] << std::endl;
}


///
/// Gets the id of device with maximal FLOPS from the context (from NVIDIA SDK)
///
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


void runAStar( cl_context context, cl_device_id deviceId, GraphData* graph,
                  CoordinateData** coordinates, int src_vert, int goal_vert){
    cl_int errNum;
    cl_command_queue commandQueue;
    commandQueue = clCreateCommandQueue( context, deviceId, 0, &errNum );
    if (errNum != CL_SUCCESS)
    {
        printf("Failed to create command queue\n");
        //return 1;
    }
    
    size_t maxWorkGroupSize;
    clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
    std::cout << "MAX_WORKGROUP_SIZE: " << maxWorkGroupSize << std::endl;
}


int main(int argc, char** argv) {


    cl_platform_id platform;
    cl_context gpuContext;
    cl_context cpuContext;
    cl_int errNum;
    cl_device_id device_id;
    
    // First, select an OpenCL platform to run on.  For this example, we
    // simply choose the first available platform.  Normally, you would
    // query for all available platforms and select the most appropriate one.
    cl_uint numPlatforms;
    errNum = clGetPlatformIDs(1, &platform, &numPlatforms);
    printf("Number of OpenCL Platforms: %d\n", numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        printf("Failed to find any OpenCL platforms.\n");
        return 1;
    }
    
    cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };
    
    // create the OpenCL context on available GPU devices
    gpuContext = clCreateContextFromType(cps, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
        printf("No GPU devices found.\n");
    }

    // Create an OpenCL context on available CPU devices
    cpuContext = clCreateContextFromType(cps, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
        printf("No CPU devices found.\n");
    }

    // Generate the graph in forward star representation
    GraphData graph;
    CoordinateData* coordinate_values;
//    generateGraph(&graph, &coordinate_values);
//
//    printf("Vertex Count: %d\n", graph.vertexCount);
//    printf("Edge Count: %d\n", graph.edgeCount);
//
//    getNodeDetails(&graph,coordinate_values,2);
 
    
    //Start A-Star
    int src_vert = 20;
    int goal_vert = 2000;
    
    runAStar( gpuContext, getMaxFlopsDev(gpuContext), &graph,
                  &coordinate_values, src_vert, goal_vert);

    clReleaseContext(gpuContext);

}


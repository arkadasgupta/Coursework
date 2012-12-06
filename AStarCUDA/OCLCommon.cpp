#include "OCLCommon.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

cl_program loadAndBuildProgram( cl_context gpuContext, const char *fileName )
{
    pthread_mutex_lock(&mutex);

    cl_int errNum;
    cl_program program;

    // Load the OpenCL source code from the .cl file
    std::ifstream kernelFile(fileName, std::ios::in);
    if (!kernelFile.is_open())
    {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return NULL;
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();

    std::string srcStdStr = oss.str();
    const char *source = srcStdStr.c_str();

    //checkError(source != NULL, true);

    // Create the program for all GPUs in the context
    program = clCreateProgramWithSource(gpuContext, 1, (const char **)&source, NULL, &errNum);
    //checkError(errNum, CL_SUCCESS);
    // build the program for all devices on the context
    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
//    if (errNum != CL_SUCCESS)
//    {
//        char cBuildLog[10240];
//        clGetProgramBuildInfo(program, getFirstDev(gpuContext), CL_PROGRAM_BUILD_LOG,
//                              sizeof(cBuildLog), cBuildLog, NULL );
//
//        cerr << cBuildLog << endl;
//        checkError(errNum, CL_SUCCESS);
//    }

    pthread_mutex_unlock(&mutex);
    return program;
}

void allocateOCLBuffers(cl_context gpuContext, cl_command_queue commandQueue, GraphData *graph,
                        float* costArray, int* visitedArray,
                        cl_mem *vertexArrayDevice, cl_mem *edgeArrayDevice, cl_mem *weightArrayDevice,
                        cl_mem *latitudeArrayDevice,cl_mem *longitudeArrayDevice,
                        //cl_mem *priorityQueueArrayDevice,
                        cl_mem *visitedArrayDevice, 
                        cl_mem *gCostArrayDevice, cl_mem *hCostArrayDevice,cl_mem *fCostArrayDevice,
                        cl_mem *temporaryListDevice,cl_mem* temporaryFCostDevice){
    
    cl_int errNum;
    cl_mem hostVertexArrayBuffer;
    cl_mem hostEdgeArrayBuffer;
    cl_mem hostWeightArrayBuffer;
    cl_mem hostLatitudeArrayBuffer;
    cl_mem hostLongitudeArrayBuffer;
    //cl_mem priorityQueueArrayBuffer;
    cl_mem visitedArrayBuffer;
    cl_mem gCostArrayBuffer; 
    cl_mem hCostArrayBuffer;
    cl_mem fCostArrayBuffer;
    cl_mem tempListBuffer;
    cl_mem tempfCostBuffer;
    
    hostVertexArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(int) * graph->vertexCount, graph->vertexArray, &errNum);
    hostEdgeArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(int) * graph->edgeCount, graph->edgeArray, &errNum);
    hostWeightArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->edgeCount, graph->weightArray, &errNum);
    hostLatitudeArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->vertexCount, graph->latitudes, &errNum);
    hostLongitudeArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->vertexCount, graph->longitudes, &errNum);
//    priorityQueueArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
//                                           sizeof(int) * graph->vertexCount, visitedArray, &errNum);
    visitedArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(int) * graph->vertexCount, visitedArray, &errNum);
    gCostArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->vertexCount, costArray, &errNum);
    hCostArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->vertexCount, costArray, &errNum);
    fCostArrayBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * graph->vertexCount, costArray, &errNum);
    tempListBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(int) * 8, visitedArray, &errNum);
    tempfCostBuffer = clCreateBuffer(gpuContext, CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR,
                                           sizeof(float) * 8, costArray, &errNum);
    
    *vertexArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(int) * graph->vertexCount, NULL, &errNum);
    *edgeArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(int) * graph->edgeCount, NULL, &errNum);
    *weightArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(float) * graph->edgeCount, NULL, &errNum);
    *latitudeArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_WRITE, sizeof(float) * graph->vertexCount, NULL, &errNum);
    *longitudeArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_WRITE, sizeof(float) * graph->vertexCount, NULL, &errNum);
    //*priorityQueueArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(int) * graph->vertexCount, NULL, &errNum);
    *visitedArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(int) * graph->vertexCount, NULL, &errNum);
    *gCostArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(float) * graph->vertexCount, NULL, &errNum);
    *hCostArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(float) * graph->vertexCount, NULL, &errNum);
    *fCostArrayDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(float) * graph->vertexCount, NULL, &errNum);
    *temporaryListDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(int) * 8, NULL, &errNum);
    *temporaryFCostDevice = clCreateBuffer(gpuContext, CL_MEM_READ_ONLY, sizeof(float) * 8, NULL, &errNum);
    
    errNum = clEnqueueCopyBuffer(commandQueue, hostVertexArrayBuffer, *vertexArrayDevice, 0, 0,
                                 sizeof(int) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, hostEdgeArrayBuffer, *edgeArrayDevice, 0, 0,
                                 sizeof(int) * graph->edgeCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, hostWeightArrayBuffer, *weightArrayDevice, 0, 0,
                                 sizeof(float) * graph->edgeCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, hostLatitudeArrayBuffer, *latitudeArrayDevice, 0, 0,
                                 sizeof(float) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, hostLongitudeArrayBuffer, *longitudeArrayDevice, 0, 0,
                                 sizeof(float) * graph->vertexCount, 0, NULL, NULL);
//    errNum = clEnqueueCopyBuffer(commandQueue, priorityQueueArrayBuffer, *priorityQueueArrayDevice, 0, 0,
//                                 sizeof(int) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, visitedArrayBuffer, *visitedArrayDevice, 0, 0,
                                 sizeof(int) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, gCostArrayBuffer, *gCostArrayDevice, 0, 0,
                                 sizeof(float) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, hCostArrayBuffer, *hCostArrayDevice, 0, 0,
                                 sizeof(float) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, fCostArrayBuffer, *fCostArrayDevice, 0, 0,
                                 sizeof(float) * graph->vertexCount, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, tempListBuffer, *temporaryListDevice, 0, 0,
                                 sizeof(int) * 8, 0, NULL, NULL);
    errNum = clEnqueueCopyBuffer(commandQueue, tempfCostBuffer, *temporaryFCostDevice, 0, 0,
                                 sizeof(float) * 8, 0, NULL, NULL);
    
    clReleaseMemObject(hostVertexArrayBuffer);
    clReleaseMemObject(hostEdgeArrayBuffer);
    clReleaseMemObject(hostWeightArrayBuffer);
    clReleaseMemObject(hostLatitudeArrayBuffer);
    clReleaseMemObject(hostLongitudeArrayBuffer);
    //clReleaseMemObject(priorityQueueArrayBuffer);
    clReleaseMemObject(visitedArrayBuffer);
    clReleaseMemObject(gCostArrayBuffer);
    clReleaseMemObject(hCostArrayBuffer);
    clReleaseMemObject(fCostArrayBuffer);
    clReleaseMemObject(tempListBuffer);
    clReleaseMemObject(tempfCostBuffer);
    
    if(errNum!=CL_SUCCESS)
        std::cout<<"Gone wrong somewhere:"<<errNum<<std::endl;
    
}


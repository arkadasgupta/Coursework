/* 
 * File:   OCLCommon.h
 * Author: arka
 *
 * Created on December 5, 2012, 10:09 PM
 */

#ifndef OCLCOMMON_H
#define	OCLCOMMON_H

#include<CL/cl.h>
#include "GraphData.h"

//static cl_device_id getMaxFlopsDev(cl_context cxGPUContext);

cl_program loadAndBuildProgram( cl_context gpuContext, const char *fileName );

void allocateOCLBuffers(cl_context gpuContext, cl_command_queue commandQueue, GraphData *graph,
                        float* costArray, int* visitedArray,
                        cl_mem *vertexArrayDevice, cl_mem *edgeArrayDevice, cl_mem *weightArrayDevice, 
                        cl_mem *latitudeArrayDevice,cl_mem *longitudeArrayDevice,
                        //cl_mem *priorityQueueArrayDevice,
                        cl_mem *visitedArrayDevice, 
                        cl_mem *gCostArrayDevice, cl_mem *hCostArrayDevice,cl_mem *fCostArrayDevice,
                        cl_mem *temporaryListDevice,cl_mem* temporaryFCostDevice);

#endif	/* OCLCOMMON_H */


/* 
 * File:   oclAStarKernel.h
 * Author: arka
 *
 * Created on November 23, 2012, 11:38 AM
 */

#ifndef OCLASTARKERNEL_H
#define	OCLASTARKERNEL_H

#include<CL/cl.h>
#include<CL/cl_platform.h>

///
//  Types
//
//
//  This data structure and algorithm implementation is based on
//  Accelerating large graph algorithms on the GPU using CUDA by
//  Parwan Harish and P.J. Narayanan
//
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

#endif	/* OCLASTARKERNEL_H */


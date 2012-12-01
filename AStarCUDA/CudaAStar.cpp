/* 
 * File:   main.cpp
 * Author: arka
 *
 * Created on December 1, 2012, 10:36 AM
 */

#include "GraphData.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    GraphData graph;
    CoordinateData* coordinate_values;
    
    generateGraph(&graph, &coordinate_values);
    cout<< "Vertex Count:" << graph.vertexCount<< endl;
    cout<< "Edge Count:" << graph.edgeCount<< endl;
    
    getNodeDetails(&graph,coordinate_values,2);
}


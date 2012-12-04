/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import java.util.List;
import newyorkjava.AdjacencyList;
import newyorkjava.Graph;

/**
 *
 * @author arka
 */
public class ConvertGraphToArrays {
    public static int[] createVertexArray(Graph graph){
        AdjacencyList adjacencyList = graph.getAdjList();
        List<Node> nodeCoordinates = graph.getNodecoordinates();
        int[] vertexArray = new int[GraphSpecs.vertexCount];
        int[] edgeArray = new int[GraphSpecs.edgeCount];
        float[] weightArray = new float[GraphSpecs.edgeCount];
        float[] latitudes = new float[GraphSpecs.vertexCount];
        float[] longitudes = new float[GraphSpecs.vertexCount];
        boolean[] visitedArray = new boolean[GraphSpecs.vertexCount];
        int edgeCount = 0;
        for(int i=0;i<GraphSpecs.vertexCount;i++){
            List<Edge> edges = adjacencyList.getAdjacent(new Node(i+1));
            vertexArray[i] = edges.get(0).to.number;
            for(Edge e: edges){
                edgeArray[edgeCount]= e.to.number;
                weightArray[edgeCount++] = e.weight;
                latitudes[i] = nodeCoordinates.get(i).latitude;
                longitudes[i] = nodeCoordinates.get(i).longitude;
            }
        }
        return null;
    }
}

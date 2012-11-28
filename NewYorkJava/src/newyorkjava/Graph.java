/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author arka
 */
public class Graph {
    
    static final double LAT = 364812.2109103587;
    static final double LONG = 274944.8683871276;
    
    private AdjacencyList adjList;
    private List<Node> nodecoordinates;
    private Set<Node> exploredSet;
    //private Set<Edge> traversedEdges;
    
    public void createGraph(){
        BufferedReader in = null;
        String line;
        try {
            in = new BufferedReader(new FileReader("USA-road-d.NY.gr"));
            while((line=in.readLine())!=null){
               if (line.startsWith("c")) {
                    continue;
                }
               if(line.startsWith("p")){
                  String graphSpecs[] = line.split("\\ ");
                  int listSize = Integer.parseInt(graphSpecs[2]);
                  listSize = listSize + (listSize/10);
                  adjList = new AdjacencyList(listSize);
                  //traversedEdges = new HashSet<>(listSize);
                  continue;
               }
               String[] arcInfo = line.split("\\ ");
               adjList.addEdge(new Node(Integer.parseInt(arcInfo[1])),
                    new Node(Integer.parseInt(arcInfo[2])),Integer.parseInt(arcInfo[3]));
            }
        } catch (IOException ex) {
            //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
        }finally{
            try {
                in.close();
            } catch (IOException ex) {
                //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        int maxAdj = 0;
        for(Node s: adjList.getSourceNodeSet()){
            if(adjList.getAdjacent(s).size()>maxAdj)
                maxAdj= adjList.getAdjacent(s).size();
        }
        //System.out.println("Max adja:"+ maxAdj);
        //System.out.println("Adjacency list size"+adjList.getAdjacent(new Node(298)));
        //System.out.println("node 2--->"+adjList.getAdjacent(new Node(2)));   
    }
    public void populateCoordinates(){
        BufferedReader in = null;
        String line;
        try {
            in = new BufferedReader(new FileReader("USA-road-d-mod.NY.co"));
            while((line=in.readLine())!=null){
               if (line.startsWith("c")) {
                    continue;
                }
               if(line.startsWith("p")){
                  String graphSpecs[] = line.split("\\ ");
                  int mapSize = Integer.parseInt(graphSpecs[4]);
                  mapSize = mapSize +(mapSize/10);
                  nodecoordinates = new ArrayList<>(mapSize);
                  exploredSet = new HashSet<>(mapSize, 0.95f);
                  continue;
               }
               String[] lineInfo = line.split("\\ ");
               nodecoordinates.add(
                       new Node(Integer.parseInt(lineInfo[1]), Double.parseDouble(lineInfo[3]), Double.parseDouble(lineInfo[2])));
            }
        } catch (IOException ex) {
            //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
        }finally{
            try {
                in.close();
            } catch (IOException ex) {
                //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        
        //System.out.println("Node Coordinates"+nodecoordinates.get(1));
    }
    
    public boolean addExploredNode(Node n){
        if(exploredSet.contains(n)){
            return false;
        }
        exploredSet.add(n);
        return true;
    }
    
    public static double approxDist(double lat1, double lng1, double lat2, double lng2) {
        return Math.sqrt(Math.pow((lat1 - lat2) * LAT, 2) + Math.pow((lng1 - lng2) * LONG, 2));
    }

    public AdjacencyList getAdjList() {
        return adjList;
    }

    public List<Node> getNodecoordinates() {
        return nodecoordinates;
    }
    
    
}

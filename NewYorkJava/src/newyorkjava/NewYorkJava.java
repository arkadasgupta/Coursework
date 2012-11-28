/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import algowiki.Graph;
import algowiki.Node;
import algowiki.Edge;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author arka
 */
public class NewYorkJava {

    static final double LAT = 364812.2109103587;
    static final double LONG = 274944.8683871276;
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //new algowiki.Graph1().createGraph();
        //System.out.println("in dere"+toDegrees(73530767));
        //System.out.println("in radians"+distFrom(41085396,73530767,41086098,73530538));
//        System.out.println("in radians"+distFrom(toDegrees(41048796),
//                toDegrees(73519366),toDegrees(41048654),toDegrees(73519377)));
//        System.out.println("approximate dist"+approxDist(toDegrees(41048796),
//                toDegrees(73519366),toDegrees(41048654),toDegrees(73519377)));
        //changeCoordinatesToDegrees();
        Graph graph = new Graph();
        graph.createGraph();
        System.out.println("Graph Creation Done");
        graph.populateCoordinates();
        System.out.println("Coordinate Processing Done");
        
        long millis = System.currentTimeMillis();
        new NewYorkJava().findRouteByAStar(1,264346,graph);
        System.out.println("Time taken by A-Star:"+(System.currentTimeMillis()-millis));
    }

    public static double distFrom(double lat1, double lng1, double lat2, double lng2) {
        double earthRadius = 3958.75;
        double dLat = Math.toRadians(lat2 - lat1);
        double dLng = Math.toRadians(lng2 - lng1);
        double sindLat = Math.sin(dLat / 2);
        double sindLng = Math.sin(dLng / 2);
        double a = Math.pow(sindLat, 2) + Math.pow(sindLng, 2)
                * Math.cos(Math.toRadians(lat1)) * Math.cos(Math.toRadians(lat2));
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        double dist = earthRadius * c;

        return dist * 5280;
    }

    public static double toDegrees(int s) {
        int degrees = s / 1000000;
        s = s % 1000000;
        int minutes = s / 10000;
        double minutesInDegree = minutes / 60.0;
        s = s % 10000;
        double secondsInDegree = s / 3600.0 / 100;
        double deg = degrees + minutesInDegree + secondsInDegree;
        //System.out.println("in degrees" + deg);
        return deg;
    }

    private static double approxDist(double lat1, double lng1, double lat2, double lng2) {
        return Math.sqrt(Math.pow((lat1 - lat2) * LAT, 2) + Math.pow((lng1 - lng2) * LONG, 2));
    }

    private static void changeCoordinatesToDegrees() {
        BufferedReader in = null;
        PrintWriter out = null;
        String line;
        try {
            in = new BufferedReader(new FileReader("USA-road-d.NY.co"));
            out = new PrintWriter(new BufferedWriter(new FileWriter("USA-road-d-mod.NY.co")));
            while ((line = in.readLine()) != null) {
                if (line.startsWith("c")||line.startsWith("p")) {
                    out.println(line);
                    continue;
                }
                String[] lineInfo = line.split("\\ ") ;
                double latitude = toDegrees(Integer.parseInt(lineInfo[3]));
                double longitude = toDegrees(Integer.parseInt(lineInfo[2])*(-1));
                StringBuilder sb = new StringBuilder(lineInfo[0]);
                sb.append(" ").append(lineInfo[1]).append(" ").
                        append(longitude).append(" ").append(latitude);
                out.println(sb.toString());
            }

        } catch (IOException ex) {
            Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            out.close();
            try {
                in.close();
                
            } catch (IOException ex) {
                Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
            }
            
        }
    }

    public void findRouteByAStar(int from, int to, Graph graph) {
        System.out.println("Source Node:"+from+"Goal node:"+to);
        if (from==to){
            printSolution(new Node(from));
        }
        Node goalNode = new Node(to);
        goalNode.latitude=graph.getNodecoordinates().get(to-1).latitude;
        goalNode.longitude=graph.getNodecoordinates().get(to-1).longitude;
        Queue<Node> frontier = new PriorityQueue<>();
        frontier.add(new Node(from));
        Node currentNode;
        while(!frontier.isEmpty()){
            currentNode = frontier.poll();
            if(currentNode.equals(goalNode)){
                printSolution(currentNode);
                return;
            }else{
                if(graph.addExploredNode(currentNode)){
                expand(currentNode,graph,goalNode,frontier);
                }else{
                    continue;
                }
            }        
        }
    }

    public void printSolution(Node goal) {
        System.out.println("Solution path:");
        while (goal!=null){
            System.out.println("node:"+goal.name+" gcost:"+goal.gCost);
            goal=goal.parent;
        }
    }

    public void expand(Node currentNode, Graph graph, Node goalNode, Queue<Node> frontier) {
        List<Edge> edges = graph.getAdjList().getAdjacent(currentNode);
        for(Edge e : edges){
            Node childNode = e.to;
            if(childNode.parent!=null && childNode.name==currentNode.name){
                continue;
            }
            childNode.gCost = currentNode.gCost+e.weight;
            double latitude = graph.getNodecoordinates().get(childNode.name-1).latitude;
            double longitude = graph.getNodecoordinates().get(childNode.name-1).longitude;
            childNode.hCost=Graph.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
            childNode.fCost=childNode.gCost+childNode.hCost;
            childNode.parent=currentNode;
            frontier.add(childNode);
        }
    }
}

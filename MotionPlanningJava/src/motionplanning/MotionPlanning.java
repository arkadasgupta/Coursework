/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

import java.util.PriorityQueue;
import java.util.Queue;

/**
 *
 * @author arka
 */
public class MotionPlanning {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        Grid problemGrid = new Grid(5,5);
        Coordinate startCoordinate = new Coordinate(1,0);
        Coordinate goalCoordinate = new Coordinate(4,5);
        
        startCoordinate.setParent(null);
        
        startMotion(problemGrid,startCoordinate,goalCoordinate);
    }

    private static void startMotion(Grid problemGrid, Coordinate startCoordinate, Coordinate goalCoordinate) {
        if(startCoordinate.equals(goalCoordinate)){
            printSolution(startCoordinate);
        }
        Queue<Coordinate> frontier = new PriorityQueue<>();
        Coordinate agentCoordinate = startCoordinate;
    }

    private static void printSolution(Coordinate startCoordinate) {
        throw new UnsupportedOperationException("Not yet implemented");
    }

}

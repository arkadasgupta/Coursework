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
        
        Grid problemGrid = new Grid(100,100);   
        Coordinate startCoordinate = new Coordinate(1,0);
        Coordinate goalCoordinate = new Coordinate(98,99);
        problemGrid.generateObstacles(1000);
        startCoordinate.setParent(null);
        startCoordinate.setgCost(0);
        startMotion(problemGrid,startCoordinate,goalCoordinate);
    }

    private static void startMotion(Grid problemGrid, Coordinate startCoordinate, Coordinate goalCoordinate) {
        if(startCoordinate.equals(goalCoordinate)){
            printSolution(startCoordinate);
        }
        Queue<Coordinate> frontier = new PriorityQueue<>();
        frontier.add(startCoordinate);
        Coordinate agentCoordinate;
        while(!frontier.isEmpty()){
            agentCoordinate = frontier.poll();
            if(agentCoordinate.equals(goalCoordinate)){
                printSolution(agentCoordinate);
                return;
            }else{
                if(problemGrid.addExploredCoordinate(agentCoordinate)){
                moveAgent(problemGrid,agentCoordinate,frontier,goalCoordinate);
                }else{
                    continue;
                }
            }        
        }
    }

    private static void printSolution(Coordinate agentCoordinate) {
        System.out.println("\nSolution Path");
        Coordinate c = agentCoordinate;
        while(c.getParent()!=null){
            System.out.println("\n"+c.getXValue()+","+c.getYValue());
            c=c.getParent();
        }
        System.out.println("\n"+c.getXValue()+","+c.getYValue());
    }

    private static void moveAgent(Grid problemGrid, Coordinate agentCoordinate, Queue<Coordinate> frontier, Coordinate goalCoordinate) {
        Coordinate newCoordinate;
        
        newCoordinate=new Coordinate(agentCoordinate.getXValue()-1, agentCoordinate.getYValue());
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new Coordinate(agentCoordinate.getXValue()+1, agentCoordinate.getYValue());
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()-1);
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()+1);
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        
    }

    private static void processCoordinate(Grid problemGrid, Queue<Coordinate> frontier, Coordinate agentCoordinate, Coordinate goalCoordinate, Coordinate newCoordinate) {
        if(problemGrid.isValidCoordinate(newCoordinate)){
            newCoordinate.setParent(agentCoordinate);
            newCoordinate.setgCost(agentCoordinate.getgCost()+1);
            newCoordinate.calculateFCost(goalCoordinate);
            frontier.add(newCoordinate);
        }
    }

}

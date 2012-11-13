/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanningST;

import java.util.PriorityQueue;
import java.util.Queue;

/**
 *
 * @author arka
 */
public class MotionPlanningST {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        GridST problemGrid = new GridST(25,25);   
        CoordinateST startCoordinate = new CoordinateST(1,0);
        CoordinateST goalCoordinate = new CoordinateST(24,25);
        problemGrid.generateObstacles(25);
        startCoordinate.setParent(null);
        startCoordinate.setgCost(0);
        startMotion(problemGrid,startCoordinate,goalCoordinate);
    }

    private static void startMotion(GridST problemGrid, CoordinateST startCoordinate, CoordinateST goalCoordinate) {
        if(startCoordinate.equals(goalCoordinate)){
            printSolution(startCoordinate);
        }
        Queue<CoordinateST> frontier = new PriorityQueue<>();
        frontier.add(startCoordinate);
        CoordinateST agentCoordinate;
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

    private static void printSolution(CoordinateST agentCoordinate) {
        System.out.println("\nSolution Path");
        CoordinateST c = agentCoordinate;
        while(c.getParent()!=null){
            System.out.println("\n"+c.getXValue()+","+c.getYValue());
            c=c.getParent();
        }
        System.out.println("\n"+c.getXValue()+","+c.getYValue());
    }

    private static void moveAgent(GridST problemGrid, CoordinateST agentCoordinate, Queue<CoordinateST> frontier, CoordinateST goalCoordinate) {
        CoordinateST newCoordinate;
        
        newCoordinate=new CoordinateST(agentCoordinate.getXValue()-1, agentCoordinate.getYValue());
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new CoordinateST(agentCoordinate.getXValue()+1, agentCoordinate.getYValue());
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new CoordinateST(agentCoordinate.getXValue(), agentCoordinate.getYValue()-1);
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        newCoordinate=new CoordinateST(agentCoordinate.getXValue(), agentCoordinate.getYValue()+1);
        processCoordinate(problemGrid,frontier,agentCoordinate,goalCoordinate,newCoordinate);
        
    }

    private static void processCoordinate(GridST problemGrid, Queue<CoordinateST> frontier, CoordinateST agentCoordinate, CoordinateST goalCoordinate, CoordinateST newCoordinate) {
        if(problemGrid.isValidCoordinate(newCoordinate)){
            newCoordinate.setParent(agentCoordinate);
            newCoordinate.setgCost(agentCoordinate.getgCost()+1);
            newCoordinate.calculateFCost(goalCoordinate);
            frontier.add(newCoordinate);
        }
    }

}

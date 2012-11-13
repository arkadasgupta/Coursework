/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author arka
 */
public class MotionPlanning {

    public static final Queue<Coordinate> frontier = new PriorityBlockingQueue<>();
    public static final Grid problemGrid = new Grid(25,25);
    public static final Coordinate startCoordinate = new Coordinate(1,0);
    public static final Coordinate goalCoordinate = new Coordinate(24,25);
    public static boolean solutionFound = false;
    public static int searches=0;
    
    public static volatile int activeThreadCount = 0;
    
    static{
        problemGrid.generateObstacles(100);
        startCoordinate.setParent(null);
        startCoordinate.setgCost(0);
    }    
    public static void main(String[] args) {
        
//        Grid problemGrid = new Grid(1000,1000);   
//        Coordinate startCoordinate = new Coordinate(1,0);
//        Coordinate goalCoordinate = new Coordinate(981,994);
//        problemGrid.generateObstacles(200000);
//        startCoordinate.setParent(null);
//        startCoordinate.setgCost(0);
        MotionPlanning plan = new MotionPlanning();
        plan.startMotion();
        if(!solutionFound) {
            System.out.println("No solution.Searches:"+searches);
            plan.printGrid(plan.createGrid());
        }
    }

    private void startMotion() {
        if(startCoordinate.equals(goalCoordinate)){
            printSolution(startCoordinate);
        }
        ExecutorService service = Executors.newFixedThreadPool(2);
        List<Future> futures=new ArrayList<>();
        Coordinate agentCoordinate;
        frontier.add(startCoordinate);

        while(true){
            //boolean activeTasks=true;
            if(frontier.isEmpty()){
              for(Future f : futures){
                    try {
                        f.get();
                    } catch (InterruptedException | ExecutionException ex) {
                        Logger.getLogger(MotionPlanning.class.getName()).log(Level.SEVERE, null, ex);
                    }
              }
              if(frontier.isEmpty()){
                break;
              }
            }
           
            agentCoordinate = frontier.poll();
            searches++;
            if(agentCoordinate.equals(goalCoordinate)){
                solutionFound = true;
                service.shutdownNow();
                printSolution(agentCoordinate);
                return;
            }else{
                if(problemGrid.addExploredCoordinate(agentCoordinate)){
                    activeThreadCount++;
                 futures.add(service.submit(new MotionAgent(agentCoordinate)));   
                //moveAgent(problemGrid,agentCoordinate,frontier,goalCoordinate);
                }else{
                    continue;
                }
            }        
        }
        service.shutdown();
    }

    private void printSolution(Coordinate agentCoordinate) {
        System.out.println("\nSolution Path");
        char[][] gridMatrix= createGrid();
        Coordinate c = agentCoordinate;
        while(c.getParent()!=null){
            //System.out.println("\n"+c.getXValue()+","+c.getYValue());
            gridMatrix[c.getXValue()][c.getYValue()]='*';
            c=c.getParent();
        }
        //System.out.println("\n"+c.getXValue()+","+c.getYValue());
        gridMatrix[c.getXValue()][c.getYValue()]='*';
        System.out.println ("\nSearches:"+searches+"\n\n");
        printGrid(gridMatrix);
        
    }
    
    public char[][] createGrid() {
        char[][] gridMatrix = new char[problemGrid.getxDimension()+1][problemGrid.getyDimension()+1];
        for(int i=0;i<=problemGrid.getxDimension();i++){
            for(int j=0;j<=problemGrid.getyDimension();j++){
                if(problemGrid.getObstacles().contains(new Coordinate(i, j))){
                    gridMatrix[i][j]='x';
                }else{
                    gridMatrix[i][j]='.';
                }
            }
        }
        gridMatrix[MotionPlanning.startCoordinate.getXValue()][MotionPlanning.startCoordinate.getYValue()]='S';
        gridMatrix[MotionPlanning.goalCoordinate.getXValue()][MotionPlanning.goalCoordinate.getYValue()]='G';
        
        return gridMatrix;
    }

    public void printGrid(char[][] gridMatrix) {
        for(int j=problemGrid.getyDimension();j>=0;j--){
            for(int i=0;i<=problemGrid.getxDimension();i++){
                System.out.print(" "+gridMatrix[i][j]);
            }
            System.out.println("\n");
        }
    }
}

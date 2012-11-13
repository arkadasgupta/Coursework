/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanningST;

import java.math.BigInteger;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Random;
import java.util.Set;

/**
 *
 * @author arka
 */
public class GridST {

    private int xDimension;
    private int yDimension;
    private CoordinateST agentCoordinate;
    private CoordinateST startCoordinate;
    private CoordinateST goalCoordinate;
    private Map<CoordinateST, Boolean> obstacles;
    private Set<CoordinateST> exploredCoordinates;
    
    public GridST(int xDimension, int yDimension) {
        this.xDimension = xDimension;
        this.yDimension = yDimension;
        exploredCoordinates = new HashSet<>();
        
        BigInteger bigX = BigInteger.valueOf(Integer.valueOf(this.xDimension)*10);
        BigInteger bigY = BigInteger.valueOf(Integer.valueOf(this.yDimension)*10);
        BigInteger nextXPrime = bigX.nextProbablePrime();
        CoordinateST.hashValueX = nextXPrime.intValue();
        if(xDimension!=yDimension){
            CoordinateST.hashValueY = bigY.nextProbablePrime().intValue();
        }else{
            CoordinateST.hashValueY = nextXPrime.nextProbablePrime().intValue();
        }
    }

    public int getxDimension() {
        return xDimension;
    }

    public void setxDimension(int xDimension) {
        this.xDimension = xDimension;
    }

    public int getyDimension() {
        return yDimension;
    }

    public void setyDimension(int yDimension) {
        this.yDimension = yDimension;
    }

    public CoordinateST getAgentCoordinate() {
        return agentCoordinate;
    }

    public void setAgentCoordinate(CoordinateST agentCoordinate) {
        this.agentCoordinate = agentCoordinate;
    }

    public CoordinateST getStartCoordinate() {
        return startCoordinate;
    }

    public void setStartCoordinate(CoordinateST startCoordinate) {
        this.startCoordinate = startCoordinate;
    }

    public CoordinateST getGoalCoordinate() {
        return goalCoordinate;
    }

    public void setGoalCoordinate(CoordinateST goalCoordinate) {
        this.goalCoordinate = goalCoordinate;
    }

    public void generateObstacles(int numberOfObstacles) {
        obstacles = new HashMap<>();
        Random xGenerator = new Random();
        Random yGenerator = new Random();
        for(int i=0;i<numberOfObstacles;i++){
        CoordinateST obstacle = new CoordinateST(xGenerator.nextInt(xDimension + 1), yGenerator.nextInt(xDimension + 1));
        obstacles.put(obstacle, Boolean.TRUE);
        }
        System.out.println("\nNumber of obstacles:"+obstacles.size()+"\nObstacles:"+obstacles.toString());
    }

    public boolean isValidCoordinate(CoordinateST newCoordinate) {
        return ((newCoordinate.getXValue()<=this.xDimension)&&(newCoordinate.getYValue()<=this.yDimension)&&(!obstacles.containsKey(newCoordinate)));
    }
    
    public Boolean addExploredCoordinate(CoordinateST exploredCoordinate){
        if(exploredCoordinates.contains(exploredCoordinate)) {
            return Boolean.FALSE;
        }
        exploredCoordinates.add(exploredCoordinate);
        return Boolean.TRUE;
    }
}

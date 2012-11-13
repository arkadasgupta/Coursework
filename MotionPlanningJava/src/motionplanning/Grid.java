/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

import java.math.BigInteger;
import java.util.Collections;
import java.util.HashSet;
import java.util.Random;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 *
 * @author arka
 */
public class Grid {

    private int xDimension;
    private int yDimension;
    private Coordinate agentCoordinate;
    private Coordinate startCoordinate;
    private Coordinate goalCoordinate;
    private Set<Coordinate> obstacles;
    private Set<Coordinate> exploredCoordinates;
    
    public Grid(int xDimension, int yDimension) {
        this.xDimension = xDimension;
        this.yDimension = yDimension;
        exploredCoordinates = Collections.newSetFromMap(new ConcurrentHashMap<Coordinate,Boolean>());
        
        BigInteger bigX = BigInteger.valueOf(Integer.valueOf(this.xDimension)*10);
        BigInteger bigY = BigInteger.valueOf(Integer.valueOf(this.yDimension)*10);
        BigInteger nextXPrime = bigX.nextProbablePrime();
        Coordinate.hashValueX = nextXPrime.intValue();
        if(xDimension!=yDimension){
            Coordinate.hashValueY = bigY.nextProbablePrime().intValue();
        }else{
            Coordinate.hashValueY = nextXPrime.nextProbablePrime().intValue();
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

    public Coordinate getAgentCoordinate() {
        return agentCoordinate;
    }

    public void setAgentCoordinate(Coordinate agentCoordinate) {
        this.agentCoordinate = agentCoordinate;
    }

    public Coordinate getStartCoordinate() {
        return startCoordinate;
    }

    public void setStartCoordinate(Coordinate startCoordinate) {
        this.startCoordinate = startCoordinate;
    }

    public Coordinate getGoalCoordinate() {
        return goalCoordinate;
    }

    public void setGoalCoordinate(Coordinate goalCoordinate) {
        this.goalCoordinate = goalCoordinate;
    }

    public Set<Coordinate> getObstacles() {
        return obstacles;
    }

    
    
    public void generateObstacles(int numberOfObstacles) {
        obstacles = new HashSet<>();
        Random xGenerator = new Random();
        Random yGenerator = new Random();
        for(int i=0;i<numberOfObstacles;i++){
        Coordinate obstacle = new Coordinate(xGenerator.nextInt(xDimension + 1), yGenerator.nextInt(xDimension + 1));
        if(obstacles.contains(obstacle)){
            i--;
        }
        obstacles.add(obstacle);
        }
        System.out.println("\nNumber of obstacles:"+obstacles.size()
                +"\nObstacles:"+obstacles.toString());
        
        //createGrid();
    }

    public boolean isValidCoordinate(Coordinate newCoordinate) {
        return (newCoordinate.getXValue()>=0 && newCoordinate.getYValue()>=0
                &&(newCoordinate.getXValue()<=this.xDimension)
                &&(newCoordinate.getYValue()<=this.yDimension)
                &&(!obstacles.contains(newCoordinate)));
    }
    
    public Boolean addExploredCoordinate(Coordinate exploredCoordinate){
        if(exploredCoordinates.contains(exploredCoordinate)) {
            return Boolean.FALSE;
        }
        exploredCoordinates.add(exploredCoordinate);
        return Boolean.TRUE;
    }

    
}

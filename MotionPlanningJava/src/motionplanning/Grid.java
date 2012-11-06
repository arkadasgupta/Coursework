/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

/**
 *
 * @author arka
 */
public class Grid {
    private int xDimension;
    private int yDimension;

    private Coordinate agentCCoordinate;
    
    public Grid(int xDimension, int yDimension) {
        this.xDimension = xDimension;
        this.yDimension = yDimension;
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

    public Coordinate getAgentCCoordinate() {
        return agentCCoordinate;
    }

    public void setAgentCCoordinate(Coordinate agentCCoordinate) {
        this.agentCCoordinate = agentCCoordinate;
    }
    
    
}

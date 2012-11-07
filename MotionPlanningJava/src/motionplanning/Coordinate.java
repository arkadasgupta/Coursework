/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

/**
 *
 * @author arka
 */
public class Coordinate implements Comparable<Coordinate> {
    private int xValue;
    private int yValue;
    public static int hashValueX;
    public static int hashValueY;
    
    private Coordinate parent;
    
    private int gCost;
    private int hCost;
    private int fCost;
    
    public Coordinate(int xValue, int yValue) {
        this.xValue = xValue;
        this.yValue = yValue;
        
    }

    public int getXValue() {
        return xValue;
    }

    public void setXValue(int xValue) {
        this.xValue = xValue;
    }

    public int getYValue() {
        return yValue;
    }

    public void setYValue(int yValue) {
        this.yValue = yValue;
    }

    public Coordinate getParent() {
        return parent;
    }

    public void setParent(Coordinate parent) {
        this.parent = parent;
    }

    
    @Override
    public int hashCode() {
        int hash = 23;
        hash=hash*hashValueX+xValue;
        hash=hash*hashValueY+yValue;
        return hash;
    }

    /**
     *
     * @param o
     * @return
     */
    @Override
    public boolean equals(Object o) {
        if(!(o instanceof Coordinate)){
            return false;
        }
        final Coordinate other = (Coordinate)o;
        return (other.xValue==this.xValue && other.yValue==this.yValue);
    }

    @Override
    public String toString() {
        return "Coordinate{" + "xValue=" + xValue + ", yValue=" + yValue + '}';
    }

    

    public int getgCost() {
        return gCost;
    }

    public void setgCost(int gCost) {
        this.gCost = gCost;
    }
    
    
    public int calculateHCost(Coordinate goalCoordinate){
        this.hCost = (goalCoordinate.xValue-this.xValue)+(goalCoordinate.yValue-this.yValue);
        return hCost;
    }
    public int calculateFCost(Coordinate goalCoordinate){
        this.fCost = calculateHCost(goalCoordinate) +this.gCost;
        return fCost;
    }

    @Override
    public int compareTo(Coordinate t) {
        return this.fCost-t.fCost;
    }
}

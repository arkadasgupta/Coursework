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
        return super.hashCode();
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
        return super.toString();
    }
    
    
    
    public int calculateGCost(Coordinate startCoordinate){
        return ((startCoordinate.xValue-this.xValue)+(startCoordinate.yValue-this.yValue));
    }
    public int calculateHCost(Coordinate goalCoordinate){
        return ((goalCoordinate.xValue-this.xValue)+(goalCoordinate.yValue-this.yValue));
    }
    public int calculateFCost(){
        return fCost+gCost;
    }

    @Override
    public int compareTo(Coordinate t) {
        return this.fCost-t.fCost;
    }
}

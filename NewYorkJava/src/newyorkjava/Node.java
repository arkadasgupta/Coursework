/*
 * This code is copied from algowiki.net
 */

package newyorkjava;

public class Node implements Comparable<Node> {
   
   public final int name;
   public boolean visited = false;   // used for Kosaraju's algorithm and Edmonds's algorithm
   
   public double latitude;
   public double longitude;
   
   public Node parent;
   public double gCost;
   public double hCost;
   public double fCost;
   
   public Node(final int argName) {
       name = argName;
   }

    public Node(int name, double latitude, double longitude) {
        this.name = name;
        this.latitude = latitude;
        this.longitude = longitude;
    }

   
   
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 17 * hash + this.name;
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Node other = (Node) obj;
        if (this.name != other.name) {
            return false;
        }
        return true;
    }
   
   
   
    @Override
   public int compareTo(final Node argNode) {
        if(this.fCost>argNode.fCost){
            return 1;
        }else if(this.fCost<argNode.fCost){
            return -1;
        }else{
            return 0;
        }
            
   }

    @Override
    public String toString() {
        return "Node{" + "name=" + name + ", visited=" + visited + ", latitude=" + latitude + ", longitude=" + longitude + '}';
    }

       
    
}
/*
 * This code is copied from algowiki.net
 */

package newyorkjava;

public class Node implements Comparable<Node> {
   
   public final int number;
   public boolean visited = false;   // used for Kosaraju's algorithm and Edmonds's algorithm
   
   public float latitude;
   public float longitude;
   
   public Node parent;
   public float gCost;
   public float hCost;
   public float fCost;
   
   public Node(final int argName) {
       number = argName;
   }

    public Node(int name, float latitude, float longitude) {
        this.number = name;
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public Node(int number, float fCost) {
        this.number = number;
        this.fCost = fCost;
    }

   
   
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 17 * hash + this.number;
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
        if (this.number != other.number) {
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
        return "Node{" + "name=" + number + ", visited=" + visited + ", latitude=" + latitude + ", longitude=" + longitude + '}';
    }

       
    
}
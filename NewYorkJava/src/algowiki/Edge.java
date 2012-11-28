/*
 * This code is copied from algowiki.net
 */

package algowiki;

public class Edge implements Comparable<Edge> {
   
   public final Node from, to; 
   public final int weight;
   
   public Edge(final Node argFrom, final Node argTo, final int argWeight){
       from = argFrom;
       to = argTo;
       weight = argWeight;
   }
   
    @Override
   public int compareTo(final Edge argEdge){
       return weight - argEdge.weight;
   }

    @Override
    public String toString() {
        return "Edge{" + "from=" + from + ", to=" + to + ", weight=" + weight + '}';
    }
    
    
}
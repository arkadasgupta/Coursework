/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author arka
 */
public class Graph1 {
    List<Vertex> vertexList = null;
    List<Edge1> edgeList = null;
    List<String> file = null;
    
    public void createGraph(){
        BufferedReader in = null;
        String line;
        try {
            in = new BufferedReader(new FileReader("USA-road-d.NY.gr"));
            while((line=in.readLine())!=null){
               if (line.startsWith("c")) {
                    continue;
                }
               if(line.startsWith("p")){
                  String graphSpecs[] = line.split("\\ ");
                  vertexList = new ArrayList<>(Integer.parseInt(graphSpecs[2]));
                  edgeList = new ArrayList<>(Integer.parseInt(graphSpecs[3]));
                  file = new ArrayList<>(Integer.parseInt(graphSpecs[3])+20);
                  continue;
               }
               file.add(line);
            }
        } catch (IOException ex) {
            Logger.getLogger(Graph1.class.getName()).log(Level.SEVERE, null, ex);
        }finally{
            try {
                in.close();
            } catch (IOException ex) {
                Logger.getLogger(Graph1.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        
        
    }
}

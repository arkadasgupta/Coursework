/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;

/**
 *
 * @author arka
 */
public class AStarUtil {

    public static float distFrom(float lat1, float lng1, float lat2, float lng2) {
        float earthRadius = 3958.75f;
        float dLat = (float)Math.toRadians(lat2 - lat1);
        float dLng = (float)Math.toRadians(lng2 - lng1);
        float sindLat = (float)Math.sin(dLat / 2);
        float sindLng = (float)Math.sin(dLng / 2);
        float a = (float)(Math.pow(sindLat, 2) + Math.pow(sindLng, 2) * Math.cos(Math.toRadians(lat1)) * Math.cos(Math.toRadians(lat2)));
        float c = (float)(2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a)));
        float dist = earthRadius * c;
        return dist * 5280;
    }

    public static float toDegrees(int s) {
        int degrees = s / 1000000;
        s = s % 1000000;
        int minutes = s / 10000;
        float minutesInDegree = minutes / 60.0f;
        s = s % 10000;
        float secondsInDegree = s / 3600.0f / 100;
        float deg = degrees + minutesInDegree + secondsInDegree;
        return deg;
    }

    public static float approxDist(float lat1, float lng1, float lat2, float lng2) {
        return (float)(Math.sqrt(Math.pow((lat1 - lat2) * Main.LAT, 2) + Math.pow((lng1 - lng2) * Main.LONG, 2)));
    }

    public static void changeCoordinatesToDegrees() {
        BufferedReader in = null;
        PrintWriter out = null;
        String line;
        try {
            in = new BufferedReader(new FileReader("USA-road-d.NY.co"));
            out = new PrintWriter(new BufferedWriter(new FileWriter("USA-road-d-mod.NY.co")));
            while ((line = in.readLine()) != null) {
                if (line.startsWith("c") || line.startsWith("p")) {
                    out.println(line);
                    continue;
                }
                String[] lineInfo = line.split("\\ ");
                float latitude = AStarUtil.toDegrees(Integer.parseInt(lineInfo[3]));
                float longitude = AStarUtil.toDegrees(Integer.parseInt(lineInfo[2]) * (-1));
                StringBuilder sb = new StringBuilder(lineInfo[0]);
                sb.append(" ").append(lineInfo[1]).append(" ").append(longitude).append(" ").append(latitude);
                out.println(sb.toString());
            }
        } catch (IOException ex) {
            //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            out.close();
            try {
                in.close();
            } catch (IOException ex) {
                //Logger.getLogger(newyorkjava.Graph1.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
    public static String readFile(String filename) {
            File f = new File(filename);
            try {
                byte[] bytes = Files.readAllBytes(f.toPath());
                return new String(bytes,"UTF-8");
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return "";
    }
}

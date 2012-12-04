/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package newyorkjava;

import static com.jogamp.opencl.CLMemory.Mem.*;
import com.jogamp.opencl.CLBuffer;
import com.jogamp.opencl.CLCommandQueue;
import com.jogamp.opencl.CLContext;
import com.jogamp.opencl.CLDevice;
import com.jogamp.opencl.CLKernel;
import com.jogamp.opencl.CLPlatform;
import com.jogamp.opencl.CLProgram;
import java.io.File;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;

/**
 *
 * @author arka
 */
public class Main {

    static final double LAT = 364812.2109103587;
    static final double LONG = 274944.8683871276;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        //new algowiki.Graph1().createGraph();
        //System.out.println("in dere"+toDegrees(73530767));
        //System.out.println("in radians"+distFrom(41085396,73530767,41086098,73530538));
//        System.out.println("in radians"+distFrom(toDegrees(41048796),
//                toDegrees(73519366),toDegrees(41048654),toDegrees(73519377)));
//        System.out.println("approximate dist"+approxDist(toDegrees(41048796),
//                toDegrees(73519366),toDegrees(41048654),toDegrees(73519377)));
        //changeCoordinatesToDegrees();
        Graph graph = new Graph();
        graph.createGraph();
        System.out.println("Graph Creation Done");
        graph.populateCoordinates();
        System.out.println("Coordinate Processing Done");

        CLPlatform[] clPl = CLPlatform.listCLPlatforms();

        System.out.println("List Platforms");
        for (int i = 0; i < clPl.length; i++) {
            System.out.println(clPl[i].getName());
            System.out.println("Devices per platform :" + clPl[i].listCLDevices().length);
            System.out.println("Compute Devices on max flops device:"
                    + clPl[i].getMaxFlopsDevice().getMaxComputeUnits());
            System.out.println(" Max flops device:"
                    + clPl[i].getMaxFlopsDevice().getMaxClockFrequency());
        }
        CLContext context = CLContext.create(clPl[0]);
        System.out.println("created " + context);

        String fname = "newyorkjava/astar.cl";
        File f = new File(fname);

        System.out.println("Current Path:" + f.getAbsolutePath());
        if (f.exists()) {
            System.out.println("Found Kernel File");
        } else {
            System.out.println("Assuming running from IDE folder.");
            String fname2 = "./src/" + fname;
            f = new File(fname2);
            if (f.exists()) {
                System.out.println("Found Kernel File");
                fname = "./src/" + fname;
            } else {
                System.out.println("Assuming running from Netbeans distribution folder.");
                fname2 = "../src/" + fname;
                f = new File(fname2);
                if (f.exists()) {
                    System.out.println("Found Kernel File");
                    fname = "../src/" + fname;
                } else {
                    System.out.println("Kernel File Not Found");
                }
            }
        }

        System.out.println("fname:" + fname);
        String sourceCode = AStarUtil.readFile(fname);

        new Main().findRouteByAStar(graph, context, sourceCode);
        context.release();
    }

    public void findRouteByAStar(Graph graph, CLContext context, String sourceCode) {
        int from = 1, to = 264340;
        int localWorkSize = 8, globalWorkSize = 8;
        boolean[] visitedArray = new boolean[GraphSpecs.vertexCount];
        for (int i = 0; i < GraphSpecs.vertexCount; i++) {
            visitedArray[i] = false;
        }
        System.out.println("Source Node:" + from + "Goal node:" + to);
        if (from == to) {
            printSolution(new Node(from));
        }

        /* creating the clbuffers */
        CLBuffer<IntBuffer> vertexArrayDevice = context.createIntBuffer(GraphSpecs.vertexCount, READ_ONLY);
        CLBuffer<IntBuffer> edgeArrayDevice = context.createIntBuffer(GraphSpecs.edgeCount, READ_ONLY);
        CLBuffer<FloatBuffer> weightArrayDevice = context.createFloatBuffer(GraphSpecs.edgeCount, READ_ONLY);
        CLBuffer<FloatBuffer> latitudeArrayDevice = context.createFloatBuffer(GraphSpecs.vertexCount, READ_ONLY);
        CLBuffer<FloatBuffer> longitudeArrayDevice = context.createFloatBuffer(GraphSpecs.vertexCount, READ_ONLY);
        CLBuffer<FloatBuffer> gCostArrayDevice = context.createFloatBuffer(GraphSpecs.vertexCount, READ_WRITE);
        CLBuffer<FloatBuffer> hCostArrayDevice = context.createFloatBuffer(GraphSpecs.vertexCount, READ_WRITE);
        CLBuffer<FloatBuffer> fCostArrayDevice = context.createFloatBuffer(GraphSpecs.vertexCount, READ_WRITE);
        CLBuffer<IntBuffer> visitedArrayDevice = context.createIntBuffer(GraphSpecs.vertexCount, READ_WRITE);
        CLBuffer<IntBuffer> tempNodeListDevice = context.createIntBuffer(globalWorkSize, WRITE_ONLY);
        CLBuffer<FloatBuffer> tempCostListDevice = context.createFloatBuffer(100, WRITE_ONLY);

        System.out.println("used device memory: "
                + (vertexArrayDevice.getCLSize() + edgeArrayDevice.getCLSize() + weightArrayDevice.getCLSize()
                + latitudeArrayDevice.getCLSize() + longitudeArrayDevice.getCLSize()
                + gCostArrayDevice.getCLSize() + hCostArrayDevice.getCLSize() + fCostArrayDevice.getCLSize()
                + visitedArrayDevice.getCLSize() + tempNodeListDevice.getCLSize() + tempCostListDevice.getCLSize())
                / 1000000 + "MB");
        /* creating the clbuffers */

        // select fastest device
        CLDevice device = context.getDevices()[0];
        System.out.println("using " + device);

        // create command queue on device.
        CLCommandQueue queue = device.createCommandQueue();
        CLProgram program = null;
        try {
            // load sources, create and build program
            program = context.createProgram(sourceCode).build();

            System.out.println("prog: " + program);
        } catch (Exception e) {
            e.printStackTrace();
        }
        populateGraphBuffers(vertexArrayDevice.getBuffer(), edgeArrayDevice.getBuffer(),
                weightArrayDevice.getBuffer(), latitudeArrayDevice.getBuffer(), longitudeArrayDevice.getBuffer(),
                graph);
        populateCostandVisitedBuffers(gCostArrayDevice.getBuffer(), hCostArrayDevice.getBuffer(),
                fCostArrayDevice.getBuffer(), visitedArrayDevice.getBuffer());

        queue.putWriteBuffer(vertexArrayDevice, false).putWriteBuffer(edgeArrayDevice, false).
                putWriteBuffer(weightArrayDevice, false).putWriteBuffer(latitudeArrayDevice, false).
                putWriteBuffer(longitudeArrayDevice, false).putWriteBuffer(gCostArrayDevice, false).
                putWriteBuffer(hCostArrayDevice, false).putWriteBuffer(fCostArrayDevice, false)
                .putWriteBuffer(visitedArrayDevice, false);
        CLKernel kernel = program.createCLKernel("test");
//        kernel.putArgs(vertexArrayDevice,edgeArrayDevice,weightArrayDevice,latitudeArrayDevice,
//                longitudeArrayDevice,gCostArrayDevice,hCostArrayDevice,fCostArrayDevice,
//                visitedArrayDevice,tempNodeListDevice,tempCostListDevice);
//        kernel.putArg(0);               //This is the current node number. It will change
//        kernel.putArg(to);              //This is the goal node number. Remains fixed.
//        kernel.putArg(localWorkSize);   //numthreads, also constant
        kernel.putArgs(vertexArrayDevice, edgeArrayDevice, weightArrayDevice, latitudeArrayDevice,
                longitudeArrayDevice, gCostArrayDevice, hCostArrayDevice, fCostArrayDevice,
                visitedArrayDevice, tempNodeListDevice, tempCostListDevice);
        kernel.putArg(0);
        kernel.putArg(to);
        kernel.putArg(localWorkSize);
        Node goalNode = new Node(to);
        goalNode.latitude = graph.getNodecoordinates().get(to - 1).latitude;
        goalNode.longitude = graph.getNodecoordinates().get(to - 1).longitude;
        Queue<Node> frontier = new PriorityQueue<Node>();
        frontier.add(new Node(from));
        Node currentNode;
        long millis= System.currentTimeMillis();
        while (!frontier.isEmpty()) {
            currentNode = frontier.poll();
            visitedArray[currentNode.number - 1] = true;
            if (currentNode.equals(goalNode)) {
                //printSolution(currentNode);
                System.out.println("Solution found" + currentNode.fCost);
                System.out.println("tIME TAKEN---->"+(System.currentTimeMillis()-millis));
                return;
            } else {
//                if (graph.addExploredNode(currentNode)) {
//                    expand(currentNode, graph, goalNode, frontier);
//                } else {
//                    continue;
//                }
                kernel.setArg(11, currentNode.number);
                queue.put1DRangeKernel(kernel, 0, globalWorkSize, localWorkSize);
                queue.putReadBuffer(tempNodeListDevice, true).putReadBuffer(tempCostListDevice, true);
                while (tempNodeListDevice.getBuffer().hasRemaining()) {
                    int nodeNum = tempNodeListDevice.getBuffer().get();
                    if (nodeNum == 0 || visitedArray[nodeNum - 1]) {
                        continue;
                    }
                    frontier.add(new Node(nodeNum, tempCostListDevice.getBuffer().get()));
                }
                tempNodeListDevice.getBuffer().rewind();tempCostListDevice.getBuffer().rewind();
            }
        }
    }

    public void printSolution(Node goal) {
        System.out.println("Solution path:");
        while (goal != null) {
            System.out.println("node:" + goal.number + " gcost:" + goal.fCost);
            goal = goal.parent;
        }
    }

    public void expand(Node currentNode, Graph graph, Node goalNode, Queue<Node> frontier) {
        List<Edge> edges = graph.getAdjList().getAdjacent(currentNode);
        for (Edge e : edges) {
            Node childNode = e.to;
            if (currentNode.parent != null && childNode.number == currentNode.parent.number) {
                //System.out.println("enters this");
                continue;
            }
            childNode.gCost = currentNode.gCost + e.weight;
            float latitude = graph.getNodecoordinates().get(childNode.number - 1).latitude;
            float longitude = graph.getNodecoordinates().get(childNode.number - 1).longitude;
            childNode.hCost = AStarUtil.approxDist(latitude, longitude, goalNode.latitude, goalNode.longitude);
            childNode.fCost = childNode.gCost + childNode.hCost;
            childNode.parent = currentNode;
            frontier.add(childNode);
        }
    }

    private void populateGraphBuffers(IntBuffer vertBuffer, IntBuffer edgeBuffer, FloatBuffer weightBuffer,
            FloatBuffer latitudeBuffer, FloatBuffer longitudeBuffer, Graph graph) {
        AdjacencyList adjacencyList = graph.getAdjList();
        List<Node> nodeCoordinates = graph.getNodecoordinates();
        int edgeCount = 0;
        for (int i = 0; i < GraphSpecs.vertexCount; i++) {
            List<Edge> edges = adjacencyList.getAdjacent(new Node(i + 1));
            vertBuffer.put(edgeCount);
            edgeCount += edges.size();
            for (Edge e : edges) {
                edgeBuffer.put(e.to.number);
                weightBuffer.put(e.weight);
            }
            latitudeBuffer.put(nodeCoordinates.get(i).latitude);
            longitudeBuffer.put(nodeCoordinates.get(i).longitude);
        }
        vertBuffer.rewind();
        edgeBuffer.rewind();
        weightBuffer.rewind();
        latitudeBuffer.rewind();
        longitudeBuffer.rewind();


    }

    private void populateCostandVisitedBuffers(FloatBuffer buffer, FloatBuffer buffer0, FloatBuffer buffer1, IntBuffer buffer2) {
        while (buffer.remaining() != 0) {
            buffer.put(0f);
        }
        while (buffer0.remaining() != 0) {
            buffer0.put(0f);
        }
        while (buffer1.remaining() != 0) {
            buffer1.put(0f);
        }
        while (buffer2.remaining() != 0) {
            buffer2.put(0);
        }

        buffer.rewind();
        buffer0.rewind();
        buffer1.rewind();
        buffer2.rewind();
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package BlocksWorld;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;

/**
 *
 * @author arka
 */
public class PuzzleMain {

    //Map<State,State> frontierMap = new HashMap<State, State>();
    State goalState = new State();

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        (new PuzzleMain()).solve(3);
    }

    private void solve(int numberOfBlocks) {
        State rootState = new State();

        setAsGoalState(numberOfBlocks, goalState);
        //System.out.println(goalState);

        rootState.setStart(numberOfBlocks);
        rootState.setGoalState(goalState);
        rootState.setNumberOfBlocks(numberOfBlocks);
        rootState.setParent(null);
        rootState.setPathCost(0);

        //System.out.println(rootState);

        solveByAStar(rootState, goalState);
    }

    private void solveByAStar(State rootState, State goalState) {
        Queue<State> frontier = new PriorityQueue<>();
        Set<State> exploredSet = new HashSet<>();
        State currentState;

        frontier.offer(rootState);
        //frontierMap.put(rootState, rootState);

        while (true) {
            currentState = frontier.poll();
            exploredSet.add(currentState);
            if (currentState.isGoal()) {
                printSolutionPath(currentState);
                return;
            } else {
                expandNode(currentState, frontier, exploredSet);
            }
        }
    }

    private void printSolutionPath(State currentState) {
        Stack<State> printStack = new Stack<>();
        while (currentState.getParent() != null) {
            printStack.add(currentState);
            currentState = currentState.getParent();
        }
        printStack.add(currentState);
        System.out.println("Solution Path:\n");
        while (!printStack.empty()) {
            System.out.println(printStack.pop().toString() + "\n");
        }
    }

    private void expandNode(State currentState, Queue<State> frontier, Set<State> exploredState) {
        State newState;
        for (int i = 1; i <= currentState.leftStack.size(); i++) {
            newState = move(i, (State) copy(currentState), BlockPosition.LEFT_STACK, BlockPosition.MIDDLE_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
            newState = move(i, (State) copy(currentState), BlockPosition.LEFT_STACK, BlockPosition.RIGHT_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
        }
        for (int i = 1; i <= currentState.midStack.size(); i++) {
            newState = move(i, (State) copy(currentState), BlockPosition.MIDDLE_STACK, BlockPosition.LEFT_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
            newState = move(i, (State) copy(currentState), BlockPosition.MIDDLE_STACK, BlockPosition.RIGHT_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
        }
        for (int i = 1; i <= currentState.rightStack.size(); i++) {
            newState = move(i, (State) copy(currentState), BlockPosition.RIGHT_STACK, BlockPosition.LEFT_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
            newState = move(i, (State) copy(currentState), BlockPosition.RIGHT_STACK, BlockPosition.MIDDLE_STACK, currentState);
            checkAndReplaceInFrontier(newState, frontier, exploredState);
        }

    }

    private State move(int numberOfBlocks, State currentState, int from, int to, State parent) {
        List<BlockPosition> tempList;
        switch (from) {
            case BlockPosition.LEFT_STACK:
                switch (to) {
                    case BlockPosition.MIDDLE_STACK:
                        for (BlockPosition p : currentState.midStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.midStack.addAll(0, currentState.leftStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.midStack) {
                            p.setStackNumber(BlockPosition.MIDDLE_STACK);
                        }
                        break;
                    case BlockPosition.RIGHT_STACK:
                        for (BlockPosition p : currentState.rightStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.rightStack.addAll(0, currentState.leftStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.rightStack) {
                            p.setStackNumber(BlockPosition.RIGHT_STACK);
                        }
                        break;
                }
                tempList = new ArrayList<>(currentState.leftStack.subList(0, numberOfBlocks));
                currentState.leftStack.removeAll(tempList);
                for (BlockPosition p : currentState.leftStack) {
                    p.setBlockDepth(p.getBlockDepth() - numberOfBlocks);
                }
                break;
            case BlockPosition.MIDDLE_STACK:
                switch (to) {
                    case BlockPosition.LEFT_STACK:
                        for (BlockPosition p : currentState.leftStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.leftStack.addAll(0, currentState.midStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.leftStack) {
                            p.setStackNumber(BlockPosition.LEFT_STACK);
                        }
                        break;
                    case BlockPosition.RIGHT_STACK:
                        for (BlockPosition p : currentState.rightStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.leftStack.addAll(0, currentState.midStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.rightStack) {
                            p.setStackNumber(BlockPosition.RIGHT_STACK);
                        }
                        break;
                }
                tempList = new ArrayList<>(currentState.midStack.subList(0, numberOfBlocks));
                currentState.midStack.removeAll(tempList);
                for (BlockPosition p : currentState.midStack) {
                    p.setBlockDepth(p.getBlockDepth() - numberOfBlocks);
                }
                break;
            case BlockPosition.RIGHT_STACK:
                switch (to) {
                    case BlockPosition.LEFT_STACK:
                        for (BlockPosition p : currentState.leftStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.leftStack.addAll(0, currentState.rightStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.leftStack) {
                            p.setStackNumber(BlockPosition.LEFT_STACK);
                        }
                        break;
                    case BlockPosition.MIDDLE_STACK:
                        for (BlockPosition p : currentState.midStack) {
                            p.setBlockDepth(p.getBlockDepth() + numberOfBlocks);
                        }
                        currentState.midStack.addAll(0, currentState.rightStack.subList(0, numberOfBlocks));
                        for (BlockPosition p : currentState.midStack) {
                            p.setStackNumber(BlockPosition.MIDDLE_STACK);
                        }
                        break;
                }
                tempList = new ArrayList<>(currentState.rightStack.subList(0, numberOfBlocks));
                currentState.rightStack.removeAll(tempList);
                for (BlockPosition p : currentState.rightStack) {
                    p.setBlockDepth(p.getBlockDepth() - numberOfBlocks);
                }
                break;
        }
        currentState.setParent(parent);
        currentState.setPathCost(parent.pathCost + 1);
        currentState.setGoalState(goalState);
        currentState.setNumberOfBlocks(parent.getNumberOfBlocks());

        for (int i = 0; i < currentState.rightStack.size(); i++) {
            currentState.currentState.put(currentState.rightStack.get(i).getBlockId(), currentState.rightStack.get(i));
        }
        for (int i = 0; i < currentState.leftStack.size(); i++) {
            currentState.currentState.put(currentState.leftStack.get(i).getBlockId(), currentState.leftStack.get(i));
        }
        for (int i = 0; i < currentState.midStack.size(); i++) {
            currentState.currentState.put(currentState.midStack.get(i).getBlockId(), currentState.midStack.get(i));
        }
        return currentState;
    }

    public static Object copy(Object orig) {
        Object obj = null;
        try {
            // Write the object out to a byte array
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            try (ObjectOutputStream out = new ObjectOutputStream(bos)) {
                out.writeObject(orig);
                out.flush();
            }

            // Make an input stream from the byte array and read
            // a copy of the object back in.
            ObjectInputStream in = new ObjectInputStream(
                    new ByteArrayInputStream(bos.toByteArray()));
            obj = in.readObject();
        } catch (IOException | ClassNotFoundException e) {
        }
        return obj;
    }

    private void checkAndReplaceInFrontier(State newState, Queue<State> frontier, Set<State> exploredState) {
        List<State> backUpList = new ArrayList<>();
        State compareState = new State();
        if (!exploredState.contains(newState) && !frontier.contains(newState)) {
            frontier.add(newState);
        } else if (frontier.contains(newState)) {
            while (true) {
                if (frontier.isEmpty()) {
                    break;
                }
                compareState = frontier.poll();
                if (compareState.equals(newState)) {
                    break;
                } else {
                    backUpList.add(compareState);
                }
            }
            if (compareState.estimatedCost() > newState.estimatedCost()) {
                frontier.add(newState);
            }
        }
        for (State s : backUpList) {
            frontier.add(s);
        }
    }

    public void setAsGoalState(int noOfBlocks, State goalState) {
        for (int i = 1; i <= noOfBlocks; i++) {
            goalState.currentState.put(i, new BlockPosition(i, BlockPosition.RIGHT_STACK, i));
            goalState.rightStack.add(goalState.currentState.get(i));
        }
        goalState.numberOfBlocks = noOfBlocks;
    }
}

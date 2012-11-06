/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package BlocksWorld;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 *
 * @author arka
 */
public class State implements Comparable<State>,Serializable{

    Map<Integer, BlockPosition> currentState = new HashMap<>();
    

    //int leftStackSize = 0;
    //int midStackSize = 0;
    //int rightStackSize = 0;

    int numberOfBlocks;

    List<BlockPosition> rightStack = new ArrayList<>();
    List<BlockPosition> leftStack = new ArrayList<>();
    List<BlockPosition> midStack = new ArrayList<>();

    State parent;
    State goal;
    int pathCost;
    int estimatedCost;

    public boolean isGoal() {
        Set<Integer> blockSet = currentState.keySet();
        for (int i : blockSet) {
            if (!currentState.get(i).equals(goal.currentState.get(i))) {
                return false;
            }
        }
        return true;
    }

    public void setAsStartState() {
        currentState.put(1, new BlockPosition(1, BlockPosition.RIGHT_STACK, 4));
        currentState.put(2, new BlockPosition(2, BlockPosition.LEFT_STACK, 2));
        currentState.put(3, new BlockPosition(3, BlockPosition.LEFT_STACK, 1));
        currentState.put(4, new BlockPosition(4, BlockPosition.MIDDLE_STACK, 1));
        currentState.put(5, new BlockPosition(5, BlockPosition.RIGHT_STACK, 5));
        currentState.put(6, new BlockPosition(6, BlockPosition.RIGHT_STACK, 3));
        currentState.put(7, new BlockPosition(7, BlockPosition.RIGHT_STACK, 2));
        currentState.put(8, new BlockPosition(8, BlockPosition.RIGHT_STACK, 1));

        rightStack.add(currentState.get(8));
        rightStack.add(currentState.get(7));
        rightStack.add(currentState.get(6));
        rightStack.add(currentState.get(1));
        rightStack.add(currentState.get(5));

        leftStack.add(currentState.get(3));
        leftStack.add(currentState.get(2));

        midStack.add(currentState.get(4));

        //leftStackSize = 2;
        //midStackSize = 1;
        //rightStackSize = 5;
        pathCost=0;
    }

    public int computeStateHeuristic() {
        int stateHeuristic = 0;
        Set<Integer> blockSet = currentState.keySet();
        for (int i : blockSet) {
            stateHeuristic += computeBlockHeuristic(i);
        }
        estimatedCost=stateHeuristic;
        return stateHeuristic;
    }

    
    /* any block will take at most 3 moves to get to goal state.
     * case 1. block on different stack.
     * case 1a. block on top. (1 or 2 moves depending on number of blocks in right stack
     * case 1b. block not on top. (same as 1a)
     * case 2. block on same stack at different depth than goal.
     * case 2a: greater depth and on top. take it out, put required number of blocks to reach
     * goal depth, replace block. 3 moves
     * case 2b: greater depth and not on top. same as case 2a
     * case 2c: lesser depth (at a higher position than required) and on top, take out
     * a number such that when replacing back the top block it sets at goal. 2 moves
     * case 2d: lesser depth and not on top. 2c+1 3 moves
     */
    public int computeBlockHeuristic(int blockNumber) {
        int moves = 0;
        if (currentState.get(blockNumber).equals(goal.currentState.get(blockNumber))) {
            moves = 0;
        } else {
            if (currentState.get(blockNumber).getStackNumber() != goal.currentState.get(blockNumber).getStackNumber()) { //there are blocks on top
                moves++;
                if (goal.currentState.get(blockNumber).getBlockDepth() + this.rightStack.size() != this.numberOfBlocks) {
                    moves++;
                }
            } else { //block on same stack as goal, i.e. right stack
                if (currentState.get(blockNumber).getBlockDepth() < goal.currentState.get(blockNumber).getBlockDepth() && currentState.get(blockNumber).getBlockDepth() == 1) {
                    moves = moves + 2;
                } else {
                    moves = moves + 3;
                }
            }
        }
        return moves;
    }

    public int estimatedCost(){
        return pathCost+computeStateHeuristic();
    }

    @Override
    public int compareTo(State o) {
        if(o.estimatedCost()>this.estimatedCost()) {
            return -1;
        }
        else if(o.estimatedCost()<this.estimatedCost()) {
            return 1;
        }
        else {
            return 0;
        }
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final State other = (State) obj;

        if(this.leftStack.size()==other.leftStack.size()){
            for(int i=0,j=0;i<this.leftStack.size();) {
                if(this.leftStack.get(i++)!=other.leftStack.get(j++)) {
                    return false;
                }
            }
        }
        else {
            return false;
        }

        if (this.rightStack.size() == other.rightStack.size()){
            for (int i = 0, j = 0; i < this.rightStack.size();) {
                if (this.rightStack.get(i++) != other.rightStack.get(j++)) {
                    return false;
                }
            }
        }
        else {
            return false;
        }

        if (this.midStack.size() == other.midStack.size()){
            for (int i = 0, j = 0; i < this.midStack.size();) {
                if (this.midStack.get(i++) != other.midStack.get(j++)) {
                    return false;
                }
            }
        }
        else {
            return false;
        }
        
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 59 * hash + (this.rightStack != null ? this.rightStack.hashCode() : 0);
        hash = 59 * hash + (this.leftStack != null ? this.leftStack.hashCode() : 0);
        hash = 59 * hash + (this.midStack != null ? this.midStack.hashCode() : 0);
        hash = 59 * hash + this.pathCost;
        return hash;
    }

    public void setParent(State parent) {
        this.parent = parent;
    }

    public void setPathCost(int pathCost) {
        this.pathCost = pathCost;
    }

    public State getParent() {
        return parent;
    }

    public int getPathCost() {
        return pathCost;
    }

    @Override
    public String toString() {
        return "State{" + "\nrightStack=" + rightStack + "\nleftStack=" + leftStack + "\nmidStack=" + midStack + '}';
    }

    public void setGoalState(State goalState) {
        this.goal = goalState;
    }

    public int getNumberOfBlocks() {
        return numberOfBlocks;
    }

    public void setNumberOfBlocks(int numberOfBlocks) {
        this.numberOfBlocks = numberOfBlocks;
    }

    public void setStart(int numBlocks){
        currentState.put(1, new BlockPosition(1, BlockPosition.RIGHT_STACK, 1));
        currentState.put(2, new BlockPosition(2, BlockPosition.LEFT_STACK, 1));
        currentState.put(3, new BlockPosition(3, BlockPosition.MIDDLE_STACK, 1));

        rightStack.add(currentState.get(1));
        leftStack.add(currentState.get(2));
        midStack.add(currentState.get(3));
    }
}

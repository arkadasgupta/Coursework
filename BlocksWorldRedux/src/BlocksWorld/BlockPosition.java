/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package BlocksWorld;

import java.io.Serializable;

/**
 *
 * @author arka
 */
public class BlockPosition implements Serializable{
    int blockId;
    int stackNumber;    //1-left stack, 2-middle, 3-right
    int blockDepth;     //depth of the block from top in that stack

    public BlockPosition(int blockId, int stackNumber, int blockDepth) {
        this.blockId = blockId;
        this.stackNumber = stackNumber;
        this.blockDepth = blockDepth;
    }

    public int getBlockDepth() {
        return blockDepth;
    }

    public int getStackNumber() {
        return stackNumber;
    }

    public void setBlockDepth(int blockDepth) {
        this.blockDepth = blockDepth;
    }

    public void setBlockId(int blockId) {
        this.blockId = blockId;
    }

    public void setStackNumber(int stackNumber) {
        this.stackNumber = stackNumber;
    }

    public int getBlockId() {
        return blockId;
    }

    
    
    
    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final BlockPosition other = (BlockPosition) obj;
        if (this.stackNumber != other.stackNumber) {
            return false;
        }
        if (this.blockDepth != other.blockDepth) {
            return false;
        }
        return true;
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 73 * hash + this.stackNumber;
        hash = 73 * hash + this.blockDepth;
        return hash;
    }

    @Override
    public String toString() {
        return "BlockPosition{" + "blockId=" + blockId + "stackNumber=" + stackNumber + "blockDepth=" + blockDepth + '}';
    }

    

    public static final int LEFT_STACK = 1;
    public static final int MIDDLE_STACK = 2;
    public static final int RIGHT_STACK = 3;
}

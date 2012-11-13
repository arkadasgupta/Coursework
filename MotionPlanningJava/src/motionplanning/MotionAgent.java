/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package motionplanning;

/**
 *
 * @author arka
 */
public class MotionAgent implements Runnable{
    Coordinate agentCoordinate;
    
    public MotionAgent(Coordinate agentCoordinate) {
        this.agentCoordinate = agentCoordinate;
    }
    
    
    @Override
    public void run() {
        //MotionPlanning.activeThreadCount++;
        moveAgent();
        MotionPlanning.activeThreadCount--;
    }
    
    private void moveAgent() {
        //Coordinate tempCoordinate;
        
        //tempCoordinate=new Coordinate(agentCoordinate.getXValue()-1, agentCoordinate.getYValue());
        processCoordinate(new Coordinate(agentCoordinate.getXValue()-1, agentCoordinate.getYValue()));
        //tempCoordinate=new Coordinate(agentCoordinate.getXValue()+1, agentCoordinate.getYValue());
        processCoordinate(new Coordinate(agentCoordinate.getXValue()+1, agentCoordinate.getYValue()));
        //tempCoordinate=new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()-1);
        processCoordinate(new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()-1));
        //tempCoordinate=new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()+1);
        processCoordinate(new Coordinate(agentCoordinate.getXValue(), agentCoordinate.getYValue()+1));
        
    }
    
    private void processCoordinate(Coordinate newCoordinate) {
        if(MotionPlanning.problemGrid.isValidCoordinate(newCoordinate)){
            newCoordinate.setParent(agentCoordinate);
            newCoordinate.setgCost(agentCoordinate.getgCost()+1);
            newCoordinate.calculateFCost(MotionPlanning.goalCoordinate);
            MotionPlanning.frontier.add(newCoordinate);
        }
    }

}

package e223770;

import java.util.ArrayList;
import java.util.TreeMap;
import java.util.Map;
import java.util.Set;
import java.util.Iterator; 
import java.util.*;

/* the most important design choice in this assignment was knowing that tree map keeps its keys sorted,
 * after running sessions, just adding results into the map and printing them in order gives us the sorted times
 * look at timeToCarNoMap, type is given List<Integer> as if two cars have exactly same lap time,
 * second driver overwrites first one's time.
 */

public class Session {

  private Track track;
  private ArrayList<Team> teamList;
  private int totalLaps;
  private TreeMap< Double , List<Integer> > timeToCarNoMap;

  public Session() {
    // Fill this method
    this.track = null;
    this.teamList = null;
    this.totalLaps = -1;
    this.timeToCarNoMap = new TreeMap<Double, List<Integer> >(); 
  }

  public Session(Track track, ArrayList<Team> teamList, int totalLaps) {
    // Fill this method
    this.track = track;
    this.teamList = teamList;
    this.totalLaps = totalLaps;
    this.timeToCarNoMap = new TreeMap<Double, List<Integer> >(); 
  }

  public Track getTrack() {
    return track;
  }

  public void setTrack(Track track) {
    this.track = track;
  }

  public ArrayList<Team> getTeamList() {
    return teamList;
  }

  public void setTeamList(ArrayList<Team> teamList) {
    this.teamList = teamList;
  }

  public int getTotalLaps() {
    return totalLaps;
  }

  public void setTotalLaps(int totalLaps) {
    this.totalLaps = totalLaps;
  }

  public void simulate() {
    boolean isTrackValid = track.isValidTrack();
    if(isTrackValid){
      System.out.printf("Track is valid.Starting simulation on %s for %d laps.%n",track.getTrackName(),totalLaps);
      for(int j = 0 ; j < teamList.size() ; j++){
        //for each team
        Team currentTeam = teamList.get(j);
        for(int k = 0 ; k < currentTeam.getCarList().size() ; k++ ){
          //get each car
          Car currentCar = currentTeam.getCarList().get(k);
          for(int i = 0 ; i < getTotalLaps() ; i++){
            //get number of laps
            for(int p = 0 ; p < track.getTrackLength();p++){
              //get number of features
              currentCar.tick(track.getNextFeature());
            }
          }
          List<Integer> carNosArray;
          if(timeToCarNoMap.containsKey(currentCar.getTotalTime())){
            carNosArray = timeToCarNoMap.get(currentCar.getTotalTime());
          }
          else{
            carNosArray = new ArrayList<Integer>();
          }
          carNosArray.add(currentCar.getCarNo());
          timeToCarNoMap.put(currentCar.getTotalTime(),carNosArray);
        }
      }
    printWinnerTeam();
    printTimingTable();
    }
    else{
      System.out.println("Track is invalid.Simulation aborted!");
    }
    // Fill this method
  }

  public String printWinnerTeam() {
    // Fill this method
    Set s = timeToCarNoMap.entrySet(); 
  
        Iterator i = s.iterator(); 

            Map.Entry m = (Map.Entry)i.next(); 

            List<Integer> winnerCarNos = (List<Integer>) m.getValue();
            int winnerCarNo = winnerCarNos.get(0);

            Team winnerTeam = carNoToTeamFinder(winnerCarNo);

            StringBuilder sb = new StringBuilder();

            sb.append("Team ");
            sb.append(winnerTeam.getName());
            sb.append(" wins.");

            String [] winningColors = winnerTeam.getTeamColors();
            if(winningColors.length >1){
              sb.append(winningColors[0]);
              sb.append(" and ");
              sb.append(winningColors[1]);
            }
            else{
              sb.append(winningColors[0]);
            }
            sb.append(" flags are waving everywhere.");

    System.out.println(sb.toString());

    return sb.toString();
  }
  private Team carNoToTeamFinder(int carNo){
    for(int i = 0 ; i < teamList.size() ; i++){
      Team currentTeam = teamList.get(i);
      for(int j = 0 ; j < currentTeam.getCarList().size(); j++){
        Car currentCar = currentTeam.getCarList().get(j);
        if(currentCar.getCarNo() == carNo){
          return currentTeam;
        }
      }
    }
    return null;
  }
    private String carNoToDriverNameFinder(int carNo){
    for(int i = 0 ; i < teamList.size() ; i++){
      Team currentTeam = teamList.get(i);
      for(int j = 0 ; j < currentTeam.getCarList().size(); j++){
        Car currentCar = currentTeam.getCarList().get(j);
        if(currentCar.getCarNo() == carNo){
          return currentCar.getDriverName();
        }
      }
    }
    return null;
  }

  private String printTimingTable() {
    // Fill this method
    Set s = timeToCarNoMap.entrySet(); 
        Iterator i = s.iterator(); 

        while (i.hasNext()) 
        { 
            Map.Entry m = (Map.Entry)i.next(); 
            double totalTime = (Double)m.getKey(); 
            List<Integer> carNos = (List<Integer>)m.getValue();
            for(int j = 0 ; j < carNos.size() ; j++){
              int carNo = carNos.get(j);
              String driverName = carNoToDriverNameFinder(carNo);
              System.out.println(driverName + "(" + carNo + "):   " + ResultUtils.formattedTime(totalTime)); 
            }
        } 
    return null;
  }
}

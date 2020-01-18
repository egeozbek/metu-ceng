package e223770;

import java.util.ArrayList;

public class Track {

  private String trackName;
  private ArrayList<TrackFeature> featureList;
  private boolean isClockwise;
  private static int currentTrackFeatureNo = -1;

  public Track() {
    // Fill this method
    this.trackName = null;
    this.featureList = null;
    this.isClockwise = false;
  }

  public Track(String trackName, ArrayList<TrackFeature> featureList, boolean isClockwise) {
    // Fill this method
    this.trackName = trackName;
    this.featureList = featureList;
    this.isClockwise = isClockwise;
  }

  public String getTrackName() {
    return trackName;
  }

  public void setTrackName(String trackName) {
    this.trackName = trackName;
  }

  public ArrayList<TrackFeature> getFeatureList() {
    return featureList;
  }

  public void setFeatureList(ArrayList<TrackFeature> featureList) {
    this.featureList = featureList;
  }

  public boolean isClockwise() {
    return isClockwise;
  }

  public void setClockwise(boolean clockwise) {
    isClockwise = clockwise;
  }

  public int getTrackLength() {
    // Fill this method
    return featureList.size();
  }

  public TrackFeature getNextFeature() {
    // Fill this method
    currentTrackFeatureNo++;
    currentTrackFeatureNo = currentTrackFeatureNo % getTrackLength();
    return featureList.get(currentTrackFeatureNo);
    
  }

  public void addFeature(TrackFeature feature) {
    // Fill this method
    featureList.add(feature);
  }

  public boolean isValidTrack() {
    // Fill this method
    int leftTurns = 0;
    int rightTurns = 0;
    int trackLength = getTrackLength();
    if(getNthFeature(0).getTurnDirection() != TurnDirection.STRAIGHT){
      return false;
    }
    if(getNthFeature(trackLength-1).getTurnDirection() != TurnDirection.STRAIGHT){
      return false;
    }
    for(int i = 0 ; i < trackLength ; i++){
      TrackFeature currentFeature = getNthFeature(i);
      if(currentFeature.getTurnDirection() == TurnDirection.LEFT){
        leftTurns++;
      }
      else if(currentFeature.getTurnDirection() == TurnDirection.RIGHT){
        rightTurns++;
      }
    }
    if(isClockwise){
      return rightTurns == leftTurns + 4 ? true : false;
    }
    else{
      return leftTurns == rightTurns + 4 ? true : false;
    }
  }
  
  private TrackFeature getNthFeature(int index){
    return featureList.get(index);
  }
}

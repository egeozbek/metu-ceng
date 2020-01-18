package e223770;

public abstract class Tire {

  protected double speed;
  protected double degradation;
  protected double degradationFactor;

  public double getSpeed() {
    return speed;
  }

  public double getDegradation() {
    return degradation;
  }

  public double getDegradationFactor() {
    return degradationFactor;
  }
  public double getFeatureTypeMultiplier(TrackFeature f){
    if(f instanceof HighSpeedTurn){
      return 1.55;
    }
    else if (f instanceof LowSpeedTurn){
      return 1.3;
    }
    else if (f instanceof Straight){
      return 1.0;
    }
    else{
      throw new IllegalArgumentException("Track feature not recognized");
    }
  }

  abstract public void tick(TrackFeature f);
  abstract public void updateDegradation(TrackFeature f);
  abstract public void updateSpeed();

}

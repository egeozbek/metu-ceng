package e223770;

public class HardTire extends Tire {

  public HardTire() {
    this.speed = 275;
    this.degradation = 0;
    this.degradationFactor = 1.0;

  }
      @Override
  public void tick(TrackFeature f){
    updateDegradation(f);
    updateSpeed();
  }

    @Override
  public void updateDegradation(TrackFeature f){
    degradation += getFeatureTypeMultiplier(f) * f.getRoughness() * getDegradationFactor();
  }

    @Override
  public void updateSpeed(){
    //speed shouldnt decrease even further , if already below 100
    if(speed>100){
      speed -= Math.min(75,degradation)*0.25;
    }
  }
}

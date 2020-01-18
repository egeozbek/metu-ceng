package e223770;

public class MediumTire extends Tire {

  public MediumTire() {
    this.speed = 310;
    this.degradation = 0;
    this.degradationFactor = 1.1;
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

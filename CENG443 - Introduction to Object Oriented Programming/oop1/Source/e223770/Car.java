package e223770;

public class Car {

  private int carNo;
  private String driverName;
  private double totalTime;
  private Tire tire;

  public Car() {
    // Fill this method
  }

  public Car(String driverName, int carNo, Tire tire) {
    // Fill this method
    this.driverName = driverName;
    this.carNo = carNo;
    this.tire = tire;
    this.totalTime  = 0.0;
  }

  public Tire getTire() {
    return tire;
  }

  public void setTire(Tire tire) {
    this.tire = tire;
  }
  public void switchToNextTireAndPitStop(){
    totalTime += 25.0;
    Tire currentTire = getTire();
    if ( currentTire instanceof SoftTire){
      setTire( new MediumTire());
    }
    else if ( currentTire instanceof MediumTire){
      setTire( new SoftTire());
    }
    else if ( currentTire instanceof HardTire){
      setTire( new SoftTire());
    }
    else{
      throw new IllegalArgumentException("Illegal tire type found");
    }
  }

  public String getDriverName() {
    return driverName;
  }

  public void setDriverName(String driverName) {
    this.driverName = driverName;
  }

  public int getCarNo() {
    return carNo;
  }

  public void setCarNo(int carNo) {
    this.carNo = carNo;
  }

  public double getTotalTime() {
    return totalTime;
  }

  public void tick(TrackFeature feature) {
    // Fill this method
    double timeSpentAtFeature = ( feature.getDistance()/getTire().getSpeed() ) + Math.random();
    //update degradation
    totalTime += timeSpentAtFeature;

    Tire currentTire = getTire();
    currentTire.tick(feature);

    if( currentTire.getDegradation() > 70){
      switchToNextTireAndPitStop();
    }
  }

}

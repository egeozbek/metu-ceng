package hardware;
public class CPU {
    private String type;
    private String brand;
    private String model;
    private int coreCount;
    private double clockSpeed;
    private double price;
    private double totalPower;

    public CPU(String type,String brand,String model,int coreCount,double clockSpeed,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.coreCount = coreCount;
        this.clockSpeed = clockSpeed;
        this.price = price;
        this.totalPower = clockSpeed * coreCount;
    }
    public String getType() {
        return type;
    }
    public void setType(String type){
        this.type = type;
    }

    public String getBrand() {
        return brand;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public String getModel() {
        return model;
    }

    public void setModel(String model) {
        this.model = model;
    }

    public int getCoreCount() {
        return coreCount;
    }

    public void setCoreCount(int coreCount) {
        this.coreCount = coreCount;
    }

    public double getClockSpeed() {
        return clockSpeed;
    }

    public void setClockSpeed(double clockSpeed) {
        this.clockSpeed = clockSpeed;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }
    public double getTotalPower(){
        return totalPower;
    }
    @Override
    public String toString() {
        String priceString = String.format("%.2f", this.price);
        return "CPU,"+this.brand+","+this.model+","+this.coreCount+","+this.clockSpeed+"GHz,"+priceString+" USD";
    } 
}
package hardware;
public class GPU {
    private String type;
    private String brand;
    private String model;
    private String chipset;
    private int capacity;
    private double clockSpeed;
    private double price;

    public GPU(String type,String brand,String model,String chipset,int capacity,double clockSpeed,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.chipset = chipset;
        this.capacity = capacity;
        this.clockSpeed = clockSpeed;
        this.price = price;
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

    public String getChipset() {
        return chipset;
    }

    public void setChipset(String chipset) {
        this.chipset = chipset;
    }

    public int getCapacity() {
        return capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
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
    @Override
    public String toString() { 
        String priceString = String.format("%.2f", this.price);
        return "GPU,"+this.brand+","+this.model+","+this.chipset+","+this.capacity+"GB,"+this.clockSpeed+"MHz,"+priceString+" USD";
    } 
}
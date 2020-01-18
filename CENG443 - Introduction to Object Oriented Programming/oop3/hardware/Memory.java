package hardware;
public class Memory {
    private String type;
    private String brand;
    private String model;
    private String socket;
    private int capacity;
    private double clockSpeed;
    private double price;

    public Memory(String type,String brand,String model,String socket,int capacity , double clockSpeed,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.socket = socket;
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

    public String getSocket() {
        return socket;
    }

    public void setSocket(String socket) {
        this.socket = socket;
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
        return "Memory,"+this.brand+","+this.model+","+this.socket+","+this.capacity+"GB,"+this.clockSpeed+"MHz,"+priceString+" USD";
    } 
}
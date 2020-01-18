package hardware;
public class HardDrive {
    private String type;
    private String brand;
    private String model;
    private int capacity;
    private double price;

    public HardDrive(String type,String brand,String model,int capacity,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.capacity = capacity;
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

    public int getCapacity() {
        return capacity;
    }

    public void setCapacity(int capacity) {
        this.capacity = capacity;
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
        return "Hard Drive,"+this.brand+","+this.model+","+this.capacity+"GB,"+priceString+" USD";
    } 
}
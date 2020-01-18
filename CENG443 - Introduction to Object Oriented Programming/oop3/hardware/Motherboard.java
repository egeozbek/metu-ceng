package hardware;
public class Motherboard {
    private String type;
    private String brand;
    private String model;
    private String socket;
    private int ramSlots;
    private double price;

    public Motherboard(String type,String brand,String model,String socket ,int ramSlots,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.socket = socket;
        this.ramSlots = ramSlots;
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

    public int getRamSlots() {
        return ramSlots;
    }

    public void setRamSlots(int ramSlots) {
        this.ramSlots = ramSlots;
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
        return "Motherboard,"+this.brand+","+this.model+","+this.socket+","+this.ramSlots+","+priceString+" USD";
    } 
}
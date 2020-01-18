package hardware;
public class Keyboard {
    private String type;
    private String brand;
    private String model;
    private String connectionType;
    private double price;

    public Keyboard(String type,String brand,String model,String connectionType,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.connectionType = connectionType;
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

    public String getConnectionType() {
        return connectionType;
    }

    public void setConnectionType(String connectionType) {
        this.connectionType = connectionType;
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
        return "Keyboard,"+this.brand+","+this.model+","+this.connectionType+","+priceString+" USD";
    } 
}
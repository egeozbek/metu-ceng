package hardware;
public class PSU {
    private String type;
    private String brand;
    private String model;
    private String formFactor;
    private int wattage;
    private double price;

    public PSU(String type,String brand,String model,String formFactor ,int wattage,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.formFactor = formFactor;
        this.wattage = wattage;
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

    public String getFormFactor() {
        return formFactor;
    }

    public void setFormFactor(String formFactor) {
        this.formFactor = formFactor;
    }
    public int getWattage() {
        return wattage;
    }

    public void setWattage(int wattage) {
        this.wattage = wattage;
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
        return "PSU,"+this.brand+","+this.model+","+this.formFactor+","+this.wattage+","+priceString+" USD";
    } 
}
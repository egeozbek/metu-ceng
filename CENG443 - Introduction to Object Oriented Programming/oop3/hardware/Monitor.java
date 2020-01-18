package hardware;
public class Monitor {
    private String type;
    private String brand;
    private String model;
    private String aspectRatio;
    private double size;
    private double price;

    public Monitor(String type,String brand,String model,String aspectRatio,double size,double price){
        this.type = type;
        this.brand = brand;
        this.model = model;
        this.aspectRatio = aspectRatio;
        this.size = size;
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

    public String getAspectRatio() {
        return aspectRatio;
    }

    public void setAspectRatio(String aspectRatio) {
        this.aspectRatio = aspectRatio;
    }

    public double getSize() {
        return size;
    }

    public void setSize(double size) {
        this.size = size;
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
        return "Monitor,"+this.brand+","+this.model+","+this.aspectRatio+","+this.size+","+priceString+" USD";
    } 
}
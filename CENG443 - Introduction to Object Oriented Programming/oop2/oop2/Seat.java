import java.util.concurrent.locks.*;
public class Seat{
    private String name;
    private String bookedBy;
    private boolean isFree = true;

    public Seat(String name){
        this.name = name;
    }
    public String getSeatName(){
        return this.name;
    }
    public boolean isSeatFree(){
        return isFree;
    }
    public String getBookedBy(){
        return bookedBy;
    }
    public void setSeatTaken(String ownerName){
        isFree = false;
        this.bookedBy = ownerName;
    }
}
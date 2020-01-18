import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.concurrent.locks.*;


public class Simulation {

    private int numOfRows;
    private int numOfColumns;
    private int numOfUsers;
    private ArrayList<ArrayList<Seat>> seats = new ArrayList<>() ;
    private List<User> users = new ArrayList<>();
    private List<Thread> userThreads = new ArrayList<>();
    private Set<String> soldSeats = new HashSet<>();
    private Set<String> reservedSeats = new HashSet<>();
    private Map<String,Condition> reservedSeatConditions = new HashMap<>();
    private Lock accessLock = new ReentrantLock();
    
    public Set<String> getSoldSeats(){
        return soldSeats;
    }

    public Set<String> getReservedSeats(){
        return reservedSeats;
    }
    public Map<String,Condition> getReservedSeatConditions(){
        return reservedSeatConditions;
    }

    public void setNumOfRows(int rows){
        this.numOfRows = rows;
    }
    public void setNumOfColumns(int columns){
        this.numOfColumns = columns;
    }
    public void setNumOfUsers(int users){
        this.numOfUsers = users;
    }
    public Lock getAccessLock(){
        return accessLock;
    }
    public void addUser (User user){
        users.add(user);
    }
    public void createSeats(){
        int asciiValueOfA = (int) 'A';
        for(int i  =  0 ; i < numOfRows ; i++){
            String rowCharacter = Character.toString ((char) ((char)asciiValueOfA+(char)i));
            ArrayList<Seat> currentRow = new ArrayList<>();
            for(int j = 0 ; j < numOfColumns ; j++){
                String columnCharacter = Integer.toString(j);
                String totalName = rowCharacter + columnCharacter;
                //System.out.println("Total seat name is : "+totalName);
                Seat currentSeat = new Seat(totalName);
                currentRow.add(currentSeat);
            }
            seats.add(currentRow);
        }
    }
    public void simulate(){
        for(int i = 0 ; i < numOfUsers ; i++){
            userThreads.add(new Thread(users.get(i)));
        }
        for(int i = 0 ; i < numOfUsers ; i++){
            userThreads.get(i).start();
        }
        for(int i = 0 ; i < numOfUsers ; i++){
            try{
                userThreads.get(i).join();
            }
            catch (InterruptedException ex) {
            }
        }
    }
    public void printAllSeats(){
        for(int i = 0 ; i < numOfRows ; i++){
            for(int j = 0 ; j < numOfColumns ; j++){
                Seat currentSeat = seats.get(i).get(j);
                if(currentSeat.isSeatFree()){
                    System.out.print("E: ");
                }
                else{
                    System.out.print("T:"+currentSeat.getBookedBy()+" ");
                }
            }
            System.out.print("\n");
        }
    }

    public Seat getSeat(String seatName){
        char asciiValueOfA = (char)'A';
        char asciiValueOfZero = (char)'0';
        char seatLetter = (char) seatName.charAt(0);
        int seatNumber = seatName.charAt(1) - asciiValueOfZero;
        int rowIndex = seatLetter - asciiValueOfA;
        ArrayList<Seat> seatRow = seats.get(rowIndex);
        Seat wantedSeat = seatRow.get(seatNumber);
        return wantedSeat;
    }

}
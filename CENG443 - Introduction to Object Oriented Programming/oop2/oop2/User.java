import java.util.List;

public class User implements Runnable{

    //TODO move this elsewhere
    public static final double TRANSACTION_FAILURE_PROBABILITY = 0.1;

    private String name;
    private List<String> wantedSeats;
    private Simulation simulation;
    public User(String name, List<String> wantedSeats,Simulation simulation){
        this.name = name;
        this.wantedSeats = wantedSeats;
        this.simulation = simulation;
    }
    public List<String> getWantedSeats (){
        return this.wantedSeats;
    }
    public String getName(){
        return this.name;
    }
    @Override
    public void run(){
        boolean stillTriesBuying = true;
        while(stillTriesBuying){
            simulation.getAccessLock().lock();
            //This boolean keeps track on what goes in the loop
            boolean areAllSeatsAvailable = true;
            for(int i = 0;i<wantedSeats.size();i++){
                String wantedSeatName = wantedSeats.get(i);
                //if at least one of them is already sold
                if(simulation.getSoldSeats().contains(wantedSeatName)){
                    areAllSeatsAvailable = false;
                    stillTriesBuying = false;
                    //REPORT FAILED RESERVATION
                    Logger.LogFailedReservation(this.name,this.wantedSeats.toString(),System.nanoTime());
                    simulation.getAccessLock().unlock();
                    break;
                }
                if(simulation.getReservedSeats().contains(wantedSeatName)){
                    areAllSeatsAvailable = false;
                    simulation.getAccessLock().unlock();
                    try{
                        simulation.getReservedSeatConditions().get(wantedSeatName).await();
                    }
                    catch(InterruptedException ex){

                    }
                    break;
                }
                //TODO reservation...
            }
            if(areAllSeatsAvailable){
                //if there is no problem with seats proceed to buying
                //add the seats to reseved seats
                for(int i = 0;i<wantedSeats.size();i++){
                    String wantedSeatName = wantedSeats.get(i);
                    //if at least one of them is already sold
                    simulation.getReservedSeats().add(wantedSeatName);
                    simulation.getReservedSeatConditions().put(wantedSeatName, simulation.getAccessLock().newCondition());
                }
                //now all reserved seats are in place , check if transaction will work
                double transactionChance = Math.random();
                if(transactionChance< TRANSACTION_FAILURE_PROBABILITY){
                    //TRANSACTION FAILED
                    //remove seats from reserved column
                    //slee
                    for(int i = 0;i<wantedSeats.size();i++){
                        String wantedSeatName = wantedSeats.get(i);
                        //if at least one of them is already sold
                        simulation.getReservedSeats().remove(wantedSeatName);
                        simulation.getReservedSeatConditions().get(wantedSeatName).signalAll();
                    }
                    Logger.LogDatabaseFailiure(this.name,this.wantedSeats.toString(),System.nanoTime());
                    simulation.getAccessLock().unlock();
                    //FAIL SLEEP
                    try{
                        Thread.sleep(100);
                    }
                    catch(InterruptedException ex){

                    }
                }
                else{
                    //TRANSACTION SUCCEEDS
                    //mark sold seats
                    for(int i = 0;i<wantedSeats.size();i++){
                        String wantedSeatName = wantedSeats.get(i);
                        simulation.getReservedSeats().remove(wantedSeatName);
                        simulation.getSoldSeats().add(wantedSeatName);
                        simulation.getReservedSeatConditions().get(wantedSeatName).signalAll();
                        simulation.getSeat(wantedSeatName).setSeatTaken(this.name);
                    }
                    stillTriesBuying = false;
                    simulation.getAccessLock().unlock();
                    //SUCCESS SLEEP
                    try{
                        Thread.sleep(50);
                    }
                    catch(InterruptedException ex){

                    }
                    Logger.LogSuccessfulReservation(this.name,this.wantedSeats.toString(),System.nanoTime());
                }
                //wake waiting threads for those seats
            }
        }
    }
}
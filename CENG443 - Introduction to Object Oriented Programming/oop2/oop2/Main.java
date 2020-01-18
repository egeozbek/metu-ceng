public class Main {

    public static void main(String[] args) {
    Logger.InitLogger();
    Simulation sim = ReservationInputParser.parseInput();
    sim.simulate();
    sim.printAllSeats();
  }

}
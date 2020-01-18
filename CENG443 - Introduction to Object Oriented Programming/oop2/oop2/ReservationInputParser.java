import java.io.*;
import java.util.List;
import java.util.LinkedList;

public class ReservationInputParser {
      public static Simulation parseInput(){
        Simulation sim = new Simulation(); 
        try{
            InputStreamReader input = new InputStreamReader(System.in);
            BufferedReader bufRead = new BufferedReader(input);
            String currentLine = null;

            currentLine = bufRead.readLine();
            String[] rowsAndColumns = currentLine.split(" ");
            
            int numOfRows = Integer.parseInt(rowsAndColumns[0]);
            int numOfColumns = Integer.parseInt(rowsAndColumns[1]);
            int numOfUsers = Integer.parseInt(bufRead.readLine());

            sim.setNumOfRows(numOfRows);
            sim.setNumOfColumns(numOfColumns);

            sim.setNumOfUsers(numOfUsers);

            int currentUser = 0;

            while ( currentUser < numOfUsers)
            {   
                currentLine = bufRead.readLine();
                String[] userInfos = currentLine.split(" ");
                String userName = userInfos[0];
                //System.out.println("User name is : "+ userName);
                int seatIndex = 0;
                List<String> seatNames = new LinkedList<>();
                while(seatIndex < userInfos.length -1){
                    seatNames.add(userInfos[seatIndex+1]);
                    seatIndex++;
                }
                for(int i = 0 ; i < seatNames.size() ; i++){
                    //System.out.println("Seat name is : "+ seatNames.get(i));
                }
                User currentUserObject = new User (userName,seatNames,sim);
                sim.addUser(currentUserObject);
                currentUser++;
            }
            sim.createSeats();
            //System.out.println("Number of Rows : "+ numOfRows);
            //System.out.println("Number of Column : "+ numOfColumns);
            //System.out.println("Number of User : "+ numOfUsers);
        }
        catch(Exception e){
            //System.out.println("Error on reading input file : ");
            e.printStackTrace();
        }
        return sim;
    }
}
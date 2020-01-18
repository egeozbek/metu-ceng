package e223770;

public class ResultUtils{
    public static String formattedTime(double timeInSeconds){
        //reason behind it is that , its already given in seconds with ms after in decimal points
        int miliseconds = (int)(1000 * timeInSeconds) -
                         ((int) timeInSeconds * 1000);
        int seconds = ((int) timeInSeconds) %60;
        int timeInMinutes = ((int)timeInSeconds)/60;
        int minutes = timeInMinutes % 60;
        int timeInHours = timeInMinutes/60;
        int hours = timeInHours%24;

        String formatted = String.format("%02d:%02d:%02d:%03d", hours,minutes,seconds,miliseconds);
        return formatted;

    }
}
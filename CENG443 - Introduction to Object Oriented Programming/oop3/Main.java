
/**
 * This may be the most emberassing code I have written,
 * it has many repetations and unnecessary stream operations,
 * due to lack of time at final examinations period.
 * (whole assignment was completed in 1 hour and submitted at 23.53 at last the day)
 * 
 * Although it is fully functional , 
 * quality of the code has suffered greatly due to
 * a general lack knowledge of lambda functions and streams at that time.
 * 
 * This assignment also proved that there is no white box testing in our department.
 * As many of the students has submitted solutions without using any classes
 * and we both got full points at the end.
 */
public class Main {
    public static void main(String[] args) {
    PartsStore a =new PartsStore();
		PartsStore ps = new PartsStore();
		//example input
		ps.FindPartsWithBrand("Keyboard", "Logitech");
		ps.FindPartsWithBrand(null, "Logitech");
		ps.TotalPrice("GPU", "Asus", "GeForce RTX 2080");
		ps.TotalPrice(null, "Asus", null);
		ps.TotalPrice("GPU", "Asus", null);
		ps.FindCheapestMemory(16);
		ps.FindCheapestMemory(32);
		ps.FindCheapestMemory(64);
		ps.FindFastestCPU();
		ps.UpdateStock();
		ps.FindPartsWithBrand("Keyboard", "Logitech");
		ps.FindPartsWithBrand(null, "Logitech");
		ps.TotalPrice("GPU", "Asus", "GeForce RTX 2080");
		ps.TotalPrice(null, "Asus", null);
		ps.FindCheapestMemory(16);
		ps.FindCheapestMemory(32);
		ps.FindCheapestMemory(64);
		ps.FindFastestCPU();
		ps.UpdateStock();
  }

}
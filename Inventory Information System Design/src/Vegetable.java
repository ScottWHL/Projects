import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class Vegetable extends FoodItem {
	private String farmName;

	/**
	 * Constructor
	 */
	public Vegetable() {

	}

	/**
	 * Convert to a string
	 */
	public String toString() {
		return super.toString() + "farm supplier: " + farmName;
	}

	/**
	 * Write the information to a formatter
	 * 
	 * @param writer The formatter
	 */
	public void outputItem(Formatter writer) {
		writer.format("v\n");
		super.outputItem(writer);
		writer.format("%s\n", farmName);
	}

	/**
	 * Read one item from keyboard
	 * 
	 * @param scanner  The console to be read
	 * @param fromFile Whether read from file
	 * @return True or false
	 */
	public boolean addItem(Scanner scanner, boolean fromFile) {
		super.addItem(scanner, fromFile);
		if (!fromFile)
			System.out.print("Enter the name of the farm supplier: ");
		farmName = scanner.nextLine();
		return true;
	}
}

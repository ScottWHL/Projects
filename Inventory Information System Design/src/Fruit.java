import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class Fruit extends FoodItem {
	private String orchardName;

	/**
	 * Constructor
	 */
	public Fruit() {

	}

	/**
	 * Convert to a string
	 */
	public String toString() {
		return super.toString() + "orchard supplier: " + orchardName;
	}

	/**
	 * Write the information to a formatter
	 * 
	 * @param writer The formatter
	 */
	public void outputItem(Formatter writer) {
		writer.format("f\n");
		super.outputItem(writer);
		writer.format("%s\n", orchardName);
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
			System.out.print("Enter the name of the orchard supplier: ");
		orchardName = scanner.nextLine();
		return true;
	}
}

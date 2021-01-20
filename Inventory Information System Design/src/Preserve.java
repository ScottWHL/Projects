import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class Preserve extends FoodItem
{
	private int jarSize;

	/**
	 * Constructor
	 */
	public Preserve()
	{

	}

	/**
	 * Convert to a string
	 */
	public String toString()
	{
		return super.toString() + "size: " + jarSize + "mL";
	}
		
	/**
	 * Write the information to a formatter
	 * @param writer The formatter
	 */
	public void outputItem(Formatter writer)
	{
		writer.format("p\n");
		super.outputItem(writer);
		writer.format("%d\n", jarSize);
	}

	/**
	 * Read one item from keyboard
	 * 
	 * @param scanner
	 *            The console to be read
	 * @param fromFile Whether read from file
	 * @return True or false
	 */
	public boolean addItem(Scanner scanner, boolean fromFile)
	{
		super.addItem(scanner, fromFile);
		// size
		while (true)
		{
			if(!fromFile) System.out.print("Enter the size of the jar in millilitres: ");
			try
			{
				this.jarSize = scanner.nextInt();
				scanner.nextLine();
			}
			catch (Exception e)
			{
				scanner.nextLine();
				System.out.println("Invalid entry");
				continue;
			}
			if (this.jarSize < 1)
			{
				System.out.println("Invalid entry");
				continue;
			}
			break;
		}
		return true;
	}
}

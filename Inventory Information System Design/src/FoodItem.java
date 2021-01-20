import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class FoodItem implements Comparable<FoodItem> {
	private int itemCode;
	private String itemName;
	private float itemPrice;
	private int itemQuantityInStock;
	private float itemCost;

	/**
	 * Constructor
	 */
	public FoodItem() {

	}

	/**
	 * Get the item code
	 * 
	 * @return item code
	 */
	public int getItemCode() {
		return itemCode;
	}

	/**
	 * Write the information to a formatter
	 * 
	 * @param writer The formatter
	 */
	public void outputItem(Formatter writer) {
		writer.format("%d\n%s\n%d\n%f\n%f\n", itemCode, itemName, itemQuantityInStock, itemCost, itemPrice);
	}

	/**
	 * constructor with parameter
	 * 
	 * @param code The item code
	 */
	public FoodItem(int code) {
		itemCode = code;
	}

	/**
	 * convert to a string
	 */
	public String toString() {
		// Item: <code> <name> <quantity> price: $<price> cost: $<cost>
		String s = "Item: " + itemCode + " " + itemName + " price: $" + String.format("%.2f", itemPrice) + " cost: $"
				+ String.format("%.2f", itemCost) + " ";
		return s;
	}

	/**
	 * change the amount of the item
	 * 
	 * @param amount The number of this item
	 * @return True or false
	 */
	public boolean updateItem(int amount) {
		if (itemQuantityInStock + amount < 0) {
			return false;
		}
		itemQuantityInStock += amount;
		return true;
	}

	/**
	 * Check whether two items are equal
	 * 
	 * @param item Another item.
	 * @return True or false
	 */
	public boolean isEqual(FoodItem item) {
		if (itemCode == item.itemCode)
			return true;
		else
			return false;
	}

	/**
	 * Read one item from keyboard
	 * 
	 * @param scanner  The console to be read
	 * @param fromFile Whether read from file
	 * @return True or false
	 */
	public boolean addItem(Scanner scanner, boolean fromFile) {
		// name
		if (!fromFile)
			System.out.print("Enter the name for the item: ");
		this.itemName = scanner.nextLine();
		// quantity
		/*
		 * while (true) { if(!fromFile)
		 * System.out.print("Enter the quantity for the item: "); try {
		 * this.itemQuantityInStock = scanner.nextInt(); scanner.nextLine(); } catch
		 * (Exception e) { scanner.nextLine(); System.out.println("Invalid entry");
		 * continue; } if (this.itemQuantityInStock < 1) {
		 * System.out.println("Invalid entry"); continue; } break; }
		 */
		// cost
		while (true) {
			if (!fromFile)
				System.out.print("Enter the cost of the item: ");
			try {
				this.itemCost = scanner.nextFloat();
				scanner.nextLine();
			} catch (Exception e) {
				scanner.nextLine();
				System.out.println("Invalid entry");
				continue;
			}
			if (this.itemCost <= 0) {
				System.out.println("Invalid entry");
				continue;
			}
			break;
		}
		// price
		while (true) {
			if (!fromFile)
				System.out.print("Enter the sales price of the item: ");
			try {
				this.itemPrice = scanner.nextFloat();
				scanner.nextLine();
			} catch (Exception e) {
				scanner.nextLine();
				System.out.println("Invalid entry");
				continue;
			}
			if (this.itemPrice <= 0) {
				System.out.println("Invalid entry");
				continue;
			}
			break;
		}
		return true;
	}

	/**
	 * Input the code.
	 * 
	 * @param scanner  The console to be read.
	 * @param fromFile Whether read from file
	 * @return True or false.
	 */
	public boolean inputCode(Scanner scanner, boolean fromFile) {
		while (true) {
			if (!fromFile)
				System.out.print("Enter the code for the item: ");
			try {
				itemCode = scanner.nextInt();
				scanner.nextLine();
			} catch (Exception e) {
				scanner.nextLine();
				System.out.println("Invalid code");
				continue;
			}
			break;
		}
		return true;
	}

	@Override
	public int compareTo(FoodItem o) {
		return itemCode - o.itemCode;
	}
}

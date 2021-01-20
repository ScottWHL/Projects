import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class Inventory {
	private ArrayList<InventoryItem> inventory = new ArrayList<>();

	/**
	 * Constructor
	 */
	public Inventory() {

	}

	/**
	 * Convert to a string
	 */
	public String toString() {
		String s = "Inventory:\n";
		for (int i = 0; i < inventory.size(); i++) {
			s = s + inventory.get(i) + "\n";
		}
		return s;
	}

	/**
	 * Check whether this item is in the list
	 * 
	 * @param item The wanted item
	 * @return -1 indicates can't find it, otherwise return the index
	 */
	public int alreadyExists(InventoryItem item) {
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory.get(i).getItemCode() == item.getItemCode())
				return i;
		}
		return -1;
	}

	public int alreadyExists(int itemCode) {
		for (int i = 0; i < inventory.size(); i++) {
			if (inventory.get(i).getItemCode() == itemCode)
				return i;
		}
		return -1;
	}

	/**
	 * Search one item
	 * 
	 * @param scanner The keyboard from console
	 */
	public void searchForItem(Scanner scanner) {
		System.out.print("Enter the code for the item: ");
		int code = scanner.nextInt();
		scanner.nextLine();
		// iteration
		for (int i = 0; i < inventory.size(); i++) {
			// found it
			if (inventory.get(i).getItemCode() == code) {
				System.out.println(inventory.get(i));
				return;
			}
		}
		// can't find it
		System.out.println("Code not found in inventory...");
	}

	/**
	 * Add one item from keyboard
	 * 
	 * @param scanner  The keyboard from console
	 * @param fromFile Whether read from file
	 * @return True or false
	 */
	public boolean addItem(Scanner scanner) {
		InventoryItem item = new InventoryItem();
		item.addItem(scanner);
		// check code
		if (alreadyExists(item) > -1) {
			System.out.println("Item code already exists");
			return false;
		}
		// insert this item by itemCode
		inventory.add(item);
		// sort it
		Collections.sort(inventory);
		// System.out.println();
		return true;
	}

	/**
	 * Change the quantity of some item
	 * 
	 * @param scanner   The keyboard for console
	 * @param buyOrSell Buy or sell one item
	 * @return True or false
	 */
	public boolean updateQuantity(Scanner scanner, boolean buyOrSell) {
		if (inventory.size() == 0) {
			System.out.println("Error...could not buy item");
			return false;
		}
		//
		int itemCode = 0;
		while (true) {
			System.out.print("Enter valid item code: ");
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
		//
		if (alreadyExists(itemCode) == -1) {
			System.out.println("Code not found in inventory...");
			if (buyOrSell)
				System.out.println("Error...could not buy item");
			else
				System.out.println("Error...could not sell item");
			return false;
		}
		InventoryItem item = inventory.get(alreadyExists(itemCode));
		//
		int amount = 0;
		while (true) {
			if (buyOrSell)
				System.out.print("Enter valid quantity to buy: ");
			else
				System.out.print("Enter valid quantity to sell: ");
			try {
				amount = scanner.nextInt();
				scanner.nextLine();
			} catch (Exception e) {
				scanner.nextLine();
				System.out.println("Invalid quantity...");
				if (buyOrSell)
					System.out.println("Error...could not buy item");
				else
					System.out.println("Error...could not sell item");
				return false;
			}
			if (amount <= 0) {
				System.out.println("Invalid code");
				continue;
			}
			break;
		}
		//
		if (!buyOrSell)
			amount = -amount;
		if (item.updateQuantity(scanner, amount) == false) {
			System.out.println("Insufficient stock in inventory...");
			System.out.println("Error...could not sell item");
			return false;
		}
		// System.out.println();
		return true;
	}

	public void printExpirySummary(Scanner scanner, LocalDate today) {
		int itemCode = 0;
		while (true) {
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
		//
		int i = alreadyExists(itemCode);
		if (i == -1) {
			System.out.println("Code not found in inventory...");
			return;
		}
		inventory.get(i).printExpirySummary(today);
		System.out.println();
	}

	public void removeExpiry(LocalDate today) {
		for (int i = 0; i < inventory.size(); i++) {
			inventory.get(i).removeExpiredItems(today);
		}
		// System.out.println();
	}
}

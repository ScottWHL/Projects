import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class InventoryItem implements Comparable<InventoryItem> {
	private int itemQuantityInStock;
	private FoodItem item;
	private ArrayList<LocalDate> expiries;

	public InventoryItem() {
		itemQuantityInStock = 0;
		this.item = null;
		expiries = new ArrayList<LocalDate>();
	}

	public boolean addItem(Scanner scanner) {
		String choice = "";
		while (true) {
			System.out.print("Do you wish to add a fruit(f), vegetable(v) or a preserve(p)? ");
			choice = scanner.nextLine();
			// ignore the space in user's input
			choice = choice.replace(" ", "");
			if (choice.equals("f") || choice.equals("v") || choice.equals("p"))
				break;
			else
				System.out.println("Invalid entry");
		}
		// create food item
		item = null;
		if (choice.equals("f"))
			item = new Fruit();
		else if (choice.equals("v"))
			item = new Vegetable();
		else if (choice.equals("p"))
			item = new Preserve();
		// input code
		item.inputCode(scanner, false);
		item.addItem(scanner, false);
		// input quantity
		while (true) {
			System.out.print("Enter the quantity for the item: ");
			try {
				itemQuantityInStock = scanner.nextInt();
				scanner.nextLine();
			} catch (Exception e) {
				scanner.nextLine();
				System.out.println("Invalid entry");
				continue;
			}
			if (itemQuantityInStock < 1) {
				System.out.println("Invalid entry");
				continue;
			}
			break;
		}
		//
		LocalDate d = null;
		while (true) {
			try {
				System.out.print("Enter the expiry date of the item (yyyy-mm-dd or none): ");
				d = LocalDate.parse(scanner.nextLine());
			} catch (Exception e) {
				System.out.println("Could not create date from input, please use format yyyy-mm-dd");
				continue;
			}
			break;
		}
		//
		for (int i = 0; i < itemQuantityInStock; i++) {
			expiries.add(d);
		}
		//
		return true;
	}

	public FoodItem getFoodItem() {
		return item;
	}

	public int getItemCode() {
		return item.getItemCode();
	}

	public boolean inputCode(Scanner scanner) {
		return item.inputCode(scanner, false);
	}

	public void printExpirySummary(LocalDate today) {
		System.out.print(item);
		System.out.println(" qty: " + itemQuantityInStock);
		System.out.println("Expiry Details:");
		//
		ArrayList<LocalDate> dates = new ArrayList<>();
		ArrayList<Integer> nums = new ArrayList<>();
		for (int i = 0; i <= expiries.size() - 1; i++) {
			if (/* expiries.get(i).isBefore(today) && */dates.size() == 0) {
				dates.add(expiries.get(i));
				nums.add(1);
			} else if (/* expiries.get(i).isBefore(today) && */dates.get(dates.size() - 1).equals(expiries.get(i))) {
				nums.set(nums.size() - 1, nums.get(nums.size() - 1) + 1);
			} else if (/* expiries.get(i).isBefore(today) && */ !dates.get(dates.size() - 1).equals(expiries.get(i))) {
				dates.add(expiries.get(i));
				nums.add(1);
			}
		}
		//
		for (int i = 0; i < dates.size(); i++) {
			System.out.println(dates.get(i) + ": " + nums.get(i));
		}
	}

	public void removeExpiredItems(LocalDate today) {
		for (int i = expiries.size() - 1; i >= 0; i--) {
			if (expiries.get(i).isBefore(today)) {
				expiries.remove(i);
				itemQuantityInStock--;
			}
		}
	}

	// add more food items
	public boolean updateQuantity(Scanner scanner, int amount) {
		if (amount > 0) {
			LocalDate d = null;
			while (true) {
				System.out.print("Enter the expiry date of the item (yyyy-mm-dd or none): ");
				try {
					d = LocalDate.parse(scanner.nextLine());
					// item.
					break;
				} catch (Exception e) {
					System.out.println("Could not create date from input, please use format yyyy-mm-dd");
					System.out.println(e.getMessage());
					continue;
				}
			}
			//
			itemQuantityInStock += amount;
			for (int i = 0; i < amount; i++)
				expiries.add(d);
			return true;
		} else {
			// sell
			if (itemQuantityInStock + amount < 0)
				return false;
			itemQuantityInStock += amount;
			for (int i = 0; i < -amount; i++)
				expiries.remove(0);
			return true;
		}
	}

	public String toString() {
		return item.toString() + " qty: " + itemQuantityInStock;
	}

	@Override
	public int compareTo(InventoryItem another) {
		return item.compareTo(another.item);
	}
}

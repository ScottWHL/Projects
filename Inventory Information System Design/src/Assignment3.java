import java.time.LocalDate;
import java.util.Formatter;
import java.util.Scanner;

/**
 * Name: Wenhao Li 
 * Student number: 040941575 
 * Assignment number: 3 
 * Date: April 01
 * Purpose of class: Using a dynamically allocated data structure to create a basic inventory system for a produce stand.
 */

public class Assignment3 {
	private static LocalDate changeDate(Scanner scanner) {
		LocalDate d = null;
		while (true) {
			System.out.print("Enter the expiry date of the item (yyyy-mm-dd or none): ");
			try {
				d = LocalDate.parse(scanner.nextLine());
				break;
			} catch (Exception e) {
				System.out.println("Could not create date from input, please use format yyyy-mm-dd");
				continue;
			}
		}
		// System.out.println();
		return d;
	}

	/**
	 * The main function
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		LocalDate today = LocalDate.now();
		Scanner scanner = new Scanner(System.in);
		// create the inventory
		Inventory store = new Inventory();
		// start the loop
		while (true) {
			// show the menu
			displayMenu();
			// the choice
			String choice = scanner.nextLine();
			// ignore the space in user's input
			choice = choice.replace(" ", "");
			if (choice.equals("1"))
				store.addItem(scanner);
			else if (choice.equals("2"))
				System.out.println(store);
			else if (choice.equals("3"))
				store.updateQuantity(scanner, true);
			else if (choice.equals("4"))
				store.updateQuantity(scanner, false);
			else if (choice.equals("5"))
				store.searchForItem(scanner);
			else if (choice.equals("6"))
				store.removeExpiry(today);
			else if (choice.equals("7"))
				store.printExpirySummary(scanner, today);
			else if (choice.equals("8"))
				today = changeDate(scanner);
			else if (choice.equals("9")) {
				System.out.println("Exiting...");
				break;
			} else
				System.out.println("Incorrect value entered");
		}
	}

	/**
	 * Show the menu.
	 */
	public static void displayMenu() {
		System.out.println("Please select one of the following:");
		System.out.println("1: Add Item to Inventory");
		System.out.println("2: Display Current Inventory");
		System.out.println("3: Buy Item(s)");
		System.out.println("4: Sell Item(s)");
		System.out.println("5: Search for Item");
		System.out.println("6: Remove Expired Items");
		System.out.println("7: Print Expiry");
		System.out.println("8: Change Today's Date");
		System.out.println("9: To Exit");
	}

}

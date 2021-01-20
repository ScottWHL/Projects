package curves;

public class MonkeySaddle implements Curve {
	/**
	 * Return the minimum X value of this curve
	 * 
	 * @return minimum X value
	 */
	public double getMinX() {
		return -20;
	}

	/**
	 * Return the minimum Y value of this curve
	 * 
	 * @return minimum Y value
	 */
	public double getMinY() {
		return -10;
	}

	/**
	 * Return the maximum X value of this curve
	 * 
	 * @return maximum X value
	 */
	public double getMaxX() {
		return 20;
	}

	/**
	 * Return the maximum Y value of this curve
	 * 
	 * @return maximum Y value
	 */
	public double getMaxY() {
		return 10;
	}

	/**
	 * Return the step size for the grid used to plot this grid.
	 * 
	 * @return step size
	 */
	public double getStepSize() {
		return 1;
	}

	/**
	 * Create the Z value for Monkey Saddle
	 * 
	 * @param x the X value
	 * @param y the Y value
	 * @return The Z value
	 */
	public double getZ(double i, double j) {
		double x = i / 10;
		double y = j / 10;
		return x * x * x - 3 * x * y * y + 3 + 5.5;
	}

	public String getName() {
		return "MonkeySaddle";
	}

}
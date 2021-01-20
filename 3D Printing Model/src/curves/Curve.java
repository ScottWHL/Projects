package curves;

public interface Curve {
	public abstract double getMinX();

	public abstract double getMinY();

	public abstract double getMaxX();

	public abstract double getMaxY();

	public abstract double getStepSize();

	public abstract double getZ(double x, double y);

}
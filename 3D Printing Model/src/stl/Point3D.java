package stl;

public class Point3D {
	private double x;
	private double y;
	private double z;

	public Point3D(double x, double y, double z) {
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
	}

	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
	}

	public double getZ() {
		return z;
	}

	public void setZ(double z) {
		this.z = z;
	}

	public String toString() {
		return x + " " + y + " " + z;
	}

	public static Point3D calcNormal(Point3D v1, Point3D v2, Point3D v3) {
		double i1 = v1.x - v2.x;
		double j1 = v1.y - v2.y;
		double k1 = v1.z - v2.z;

		double i2 = v1.x - v3.x;
		double j2 = v1.y - v3.y;
		double k2 = v1.z - v3.z;

		double x = j1 * k2 - k1 * j2;
		double y = k1 * i2 - i1 * k2;
		double z = i1 * j2 - j1 * i2;

		double len = Math.sqrt(x * x + y * y + z * z);

		return new Point3D(x / len, y / len, z / len);
	}
}
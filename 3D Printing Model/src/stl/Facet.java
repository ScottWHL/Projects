package stl;

public class Facet {
	private Point3D normal;
	private Point3D v1;
	private Point3D v2;
	private Point3D v3;

	public Facet(Point3D v1, Point3D v2, Point3D v3) {
		this.v1 = v1;
		this.v2 = v2;
		this.v3 = v3;
		normal = Point3D.calcNormal(v1, v2, v3);
	}

	public String toString() {
		String output = new String();
		output += "facet normal " + normal + "\n";
		output += " outer loop\n";
		output += "  vertex " + v1 + "\n";
		output += "  vertex " + v2 + "\n";
		output += "  vertex " + v3 + "\n";
		output += " endloop\n";
		output += "endfacet\n";

		return output;
	}
}
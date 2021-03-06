package stl;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import curves.ConeCurve;
import curves.Curve;

public class CreateStlCurve {
	private List<List<Point3D>> list;

	public void createMatrix(Curve curve) {
		double ss = curve.getStepSize();
		double minX = curve.getMinX();
		double minY = curve.getMinY();
		double maxX = curve.getMaxX();
		double maxY = curve.getMaxY();

		int xSize = (int) ((maxX - minX) / ss) + 1;
		int ySize = (int) ((maxY - minY) / ss) + 1;
		list = new ArrayList<List<Point3D>>(ySize);

		double xValue, yValue = 0;
		for (int y = 0; y < ySize; y++) {
			List<Point3D> row = new ArrayList<Point3D>();
			list.add(row);
			xValue = 0;
			for (int x = 0; x < xSize; x++) {
				row.add(new Point3D(xValue, yValue, curve.getZ(xValue + minX, yValue + minY)));
				xValue += ss;
			}
			yValue += ss;
		}
	}

	public Solid createSolid(String name) {
		Solid solid = new Solid(name);

		int width = list.size() - 1;
		int height = list.get(0).size() - 1;

		// Create curve
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				// as viewed from above
				solid.addFacet(list.get(w).get(h), // bottom left
						list.get(w + 1).get(h), // bottom right
						list.get(w + 1).get(h + 1), // top right
						list.get(w).get(h + 1)); // top left
			}
		}

		// Create bottom edge (Y=0)
		for (int w = 0; w < width; w++) {
			Point3D a = new Point3D(list.get(w).get(0).getX(), list.get(w).get(0).getY(), 0);
			Point3D b = new Point3D(list.get(w + 1).get(0).getX(), list.get(w + 1).get(0).getY(), 0);
			solid.addFacet(a, list.get(w).get(0), list.get(w + 1).get(0), b);
		}

		// Create top edge
		for (int w = 0; w < width; w++) {
			Point3D a = new Point3D(list.get(w).get(height).getX(), list.get(w).get(height).getY(), 0);
			Point3D b = new Point3D(list.get(w + 1).get(height).getX(), list.get(w + 1).get(height).getY(), 0);
			solid.addFacet(a, list.get(w).get(height), list.get(w + 1).get(height), b);
		}

		// Create left edge
		for (int h = 0; h < height; h++) {
			Point3D a = new Point3D(list.get(0).get(h).getX(), list.get(0).get(h).getY(), 0);
			Point3D b = new Point3D(list.get(0).get(h + 1).getX(), list.get(0).get(h + 1).getY(), 0);
			solid.addFacet(a, list.get(0).get(h), list.get(0).get(h + 1), b);
		}

		// Create right edge
		for (int h = 0; h < height; h++) {
			Point3D a = new Point3D(list.get(width).get(h).getX(), list.get(width).get(h).getY(), 0);
			Point3D b = new Point3D(list.get(width).get(h + 1).getX(), list.get(width).get(h + 1).getY(), 0);
			solid.addFacet(a, list.get(width).get(h), list.get(width).get(h + 1), b);
		}

		// Create base
		solid.addFacet(new Point3D(list.get(0).get(0).getX(), list.get(0).get(0).getY(), 0),
				new Point3D(list.get(width).get(0).getX(), list.get(width).get(0).getY(), 0),
				new Point3D(list.get(width).get(height).getX(), list.get(width).get(height).getY(), 0),
				new Point3D(list.get(0).get(height).getX(), list.get(0).get(height).getY(), 0));

		return solid;
	}

	public void printMatrix() {
		for (List<Point3D> row : list) {
			for (Point3D value : row)
				System.out.print("(" + value + ") ");
			System.out.println();
		}
	}

	public static void main(String[] arg) {
		CreateStlCurve curve = new CreateStlCurve();
		ConeCurve c = new ConeCurve();
		curve.createMatrix(c);
		Solid solid = curve.createSolid("ConeCurve");

		String name = "c:\\temp\\ConeCurve.stl";
		Path path = Paths.get(name);

		try {
			solid.toTextFile(path);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
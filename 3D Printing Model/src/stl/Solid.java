package stl;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

public class Solid {
	private final String name;
	private final List<Facet> facets = new ArrayList<>();

	public Solid() {
		name = "";
	}

	public Solid(String name) {
		this.name = name;
	}

	public boolean addFacet(Facet facet) {
		return facets.add(facet);
	}

	public boolean addFacet(Point3D... vertex) {
		int len = vertex.length;
		if (len < 3) {
			return false;
		}

		Facet facet;
		boolean rtrn = true;
		for (int i = 2; i < len; i++) {
			facet = new Facet(vertex[0], vertex[i - 1], vertex[i]);
			facets.add(facet);
		}

		return rtrn;
	}

	public void toTextFile(Path path) throws IOException {
		Files.write(path, toString().getBytes());
	}

	public String toString() {
		String output = new String();

		output += "solid " + name + '\n';
		for (Facet facet : facets) {
			output += facet;
		}
		output += "endsolid " + name + '\n';

		return output;
	}

}
package stl;

import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import curves.ConeCurve;
import curves.Curve;
import curves.HalfSphere;
import curves.MonkeySaddle;
import curves.MyCurve;

public class StlCurves {

	public static void main(String[] arg) {
		int option = 0;
		String name = null;
		int number = 0;
		List<Solid> curves = new ArrayList<>();
		Scanner input = new Scanner(System.in);

//		ArrayList<Curve> list = new ArrayList<Curve>();
//		list.add(new ConeCurve());
//		list.add(new MonkeySaddle());
//		list.add(new HalfSphere(20));

		CreateStlCurve curve1 = new CreateStlCurve();
		ConeCurve c = new ConeCurve();
		curve1.createMatrix(c);
		Solid solid1 = curve1.createSolid(c.getName());
		curves.add(solid1);

		CreateStlCurve curve2 = new CreateStlCurve();
		MonkeySaddle h = new MonkeySaddle();
		curve2.createMatrix(h);
		Solid solid2 = curve2.createSolid(h.getName());
		curves.add(solid2);

		CreateStlCurve curve3 = new CreateStlCurve();
		HalfSphere m = new HalfSphere(20);
		curve3.createMatrix(m);
		Solid solid3 = curve3.createSolid(m.getName());
		curves.add(solid3);
		
		CreateStlCurve curve4 = new CreateStlCurve();
		MyCurve myCurve = new MyCurve();
		curve4.createMatrix(myCurve);
		Solid solid4 = curve4.createSolid(m.getName());
		curves.add(solid4);

		System.out.println("1 ConeCurve");
		System.out.println("2 MonkeySphere");
		System.out.println("3 HaftSphere");
		System.out.println("4 MyCurve");

		System.out.print("Which curve number do you want? ");
		option = input.nextInt();

		switch (option) {
		case 1:
			number = 0;
			name = "c:\\temp\\ConeCurve.stl";
			break;
		case 2:
			number = 1;
			name = "c:\\temp\\MonkeySaddle.stl";
			break;
		case 3:
			number = 2;
			name = "c:\\temp\\HaftSphere20.0.stl";
			break;
		case 4:
			number = 3;
			name = "c:\\temp\\MyCurve.stl";
			break;
		default:
			System.err.println("Invalid number, no curve created");
		}

		Path path = Paths.get(name);
		System.out.println(name + " has been created");
		try {
			curves.get(number).toTextFile(path);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
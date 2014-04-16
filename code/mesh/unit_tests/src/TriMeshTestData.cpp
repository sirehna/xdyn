#include "TriMeshTestData.hpp"

Point3dTriplet generate_a_single_triangle()
{
	Point3dTriplet P;
	P.p1.x = 1.0;
	P.p1.y = 2.1;
	P.p1.z = 3.2;
	P.p2.x = 2.1;
	P.p2.y = 3.7;
	P.p2.z = 4.5;
	P.p3.x = 3.1;
	P.p3.y = 4.5;
	P.p3.z = 6.7;
	return P;
}

Point3dTriplet generate_a_degenerated_triangle()
{
	Point3dTriplet P;
	P.p1.x = 1.0;
	P.p1.y = 2.1;
	P.p1.z = 3.2;
	P.p2.x = 2.1;
	P.p2.y = 3.7;
	P.p2.z = 4.5;
	P.p3.x = 2.1;
	P.p3.y = 3.7;
	P.p3.z = 4.5;
	return P;
}

VectorOfPoint3dTriplet generate_a_unit_cube()
{
	VectorOfPoint3dTriplet V;
	Point3dTriplet P;
	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=-0.5; P.p2.z=+0.5;
	P.p3.x=+0.5; P.p3.y=+0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=+0.5; P.p2.z=+0.5;
	P.p3.x=-0.5; P.p3.y=+0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=+0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=-0.5; P.p2.y=-0.5; P.p2.z=-0.5;
	P.p3.x=-0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);

	P.p1.x=+0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=-0.5; P.p2.y=+0.5; P.p2.z=-0.5;
	P.p3.x=+0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=-0.5; P.p2.y=-0.5; P.p2.z=+0.5;
	P.p3.x=-0.5; P.p3.y=+0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=-0.5; P.p2.y=+0.5; P.p2.z=+0.5;
	P.p3.x=-0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);

	P.p1.x=+0.5; P.p1.y=-0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=-0.5; P.p2.z=-0.5;
	P.p3.x=+0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);

	P.p1.x=+0.5; P.p1.y=-0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=+0.5; P.p2.z=-0.5;
	P.p3.x=+0.5; P.p3.y=+0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=+0.5; P.p2.y=-0.5; P.p2.z=-0.5;
	P.p3.x=+0.5; P.p3.y=-0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=-0.5; P.p1.z=-0.5;
	P.p2.x=+0.5; P.p2.y=-0.5; P.p2.z=+0.5;
	P.p3.x=-0.5; P.p3.y=-0.5; P.p3.z=+0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=+0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=+0.5; P.p2.z=+0.5;
	P.p3.x=+0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);

	P.p1.x=-0.5; P.p1.y=+0.5; P.p1.z=+0.5;
	P.p2.x=+0.5; P.p2.y=+0.5; P.p2.z=-0.5;
	P.p3.x=-0.5; P.p3.y=+0.5; P.p3.z=-0.5;
	V.push_back(P);
	return V;
}

#include "TriMeshTestData.hpp"

Point3dTriplet one_triangle()
{
	Point3dTriplet P;
	P.p1(0) = 1.0;
	P.p1(1) = 2.1;
	P.p1(2) = 3.2;
	P.p2(0) = 2.1;
	P.p2(1) = 3.7;
	P.p2(2) = 4.5;
	P.p3(0) = 3.1;
	P.p3(1) = 4.5;
	P.p3(2) = 6.7;
	return P;
}

Point3dTriplet degenerated_triangle()
{
	Point3dTriplet P;
	P.p1(0) = 1.0;
	P.p1(1) = 2.1;
	P.p1(2) = 3.2;
	P.p2(0) = 2.1;
	P.p2(1) = 3.7;
	P.p2(2) = 4.5;
	P.p3(0) = 2.1;
	P.p3(1) = 3.7;
	P.p3(2) = 4.5;
	return P;
}

VectorOfPoint3dTriplet unit_cube()
{
	VectorOfPoint3dTriplet V;
	Point3dTriplet P;
	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=-0.5; P.p2(2)=+0.5;
	P.p3(0)=+0.5; P.p3(1)=+0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=+0.5; P.p2(2)=+0.5;
	P.p3(0)=-0.5; P.p3(1)=+0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=+0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=-0.5; P.p2(1)=-0.5; P.p2(2)=-0.5;
	P.p3(0)=-0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);

	P.p1(0)=+0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=-0.5; P.p2(1)=+0.5; P.p2(2)=-0.5;
	P.p3(0)=+0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=-0.5; P.p2(1)=-0.5; P.p2(2)=+0.5;
	P.p3(0)=-0.5; P.p3(1)=+0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=-0.5; P.p2(1)=+0.5; P.p2(2)=+0.5;
	P.p3(0)=-0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);

	P.p1(0)=+0.5; P.p1(1)=-0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=-0.5; P.p2(2)=-0.5;
	P.p3(0)=+0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);

	P.p1(0)=+0.5; P.p1(1)=-0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=+0.5; P.p2(2)=-0.5;
	P.p3(0)=+0.5; P.p3(1)=+0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=+0.5; P.p2(1)=-0.5; P.p2(2)=-0.5;
	P.p3(0)=+0.5; P.p3(1)=-0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=-0.5; P.p1(2)=-0.5;
	P.p2(0)=+0.5; P.p2(1)=-0.5; P.p2(2)=+0.5;
	P.p3(0)=-0.5; P.p3(1)=-0.5; P.p3(2)=+0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=+0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=+0.5; P.p2(2)=+0.5;
	P.p3(0)=+0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);

	P.p1(0)=-0.5; P.p1(1)=+0.5; P.p1(2)=+0.5;
	P.p2(0)=+0.5; P.p2(1)=+0.5; P.p2(2)=-0.5;
	P.p3(0)=-0.5; P.p3(1)=+0.5; P.p3(2)=-0.5;
	V.push_back(P);
	return V;
}

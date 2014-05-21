#include "TriMeshTestData.hpp"

VectorOfPoints one_triangle()
{
    VectorOfPoints P;
    EPoint p1, p2, p3;
	p1(0) = 1.0;
	p1(1) = 2.1;
	p1(2) = 3.2;
	p2(0) = 2.1;
	p2(1) = 3.7;
	p2(2) = 4.5;
	p3(0) = 3.1;
	p3(1) = 4.5;
	p3(2) = 6.7;
	P.push_back(p1);
	P.push_back(p2);
	P.push_back(p3);
	return P;
}

VectorOfPoints degenerated_triangle()
{
    VectorOfPoints P;
    EPoint p1, p2, p3;
	p1(0) = 1.0;
	p1(1) = 2.1;
	p1(2) = 3.2;
	p2(0) = 2.1;
	p2(1) = 3.7;
	p2(2) = 4.5;
	p3(0) = 2.1;
	p3(1) = 3.7;
	p3(2) = 4.5;
	P.push_back(p1);
    P.push_back(p2);
    P.push_back(p3);
	return P;
}

VectorOfVectorOfPoints unit_cube()
{
	VectorOfVectorOfPoints V;
	VectorOfPoints P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12;
	EPoint p1, p2, p3;
	p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
	p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
	P1.push_back(p1);
    P1.push_back(p2);
    P1.push_back(p3);
	V.push_back(P1);

	p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
	p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
	P2.push_back(p1);
    P2.push_back(p2);
    P2.push_back(p3);
    V.push_back(P2);

	p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=-0.5; p2(1)=-0.5; p2(2)=-0.5;
	p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
	P3.push_back(p1);
    P3.push_back(p2);
    P3.push_back(p3);
    V.push_back(P3);

	p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=-0.5; p2(1)=+0.5; p2(2)=-0.5;
	p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
	P4.push_back(p1);
    P4.push_back(p2);
    P4.push_back(p3);
    V.push_back(P4);

	p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=-0.5; p2(1)=-0.5; p2(2)=+0.5;
	p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
	P5.push_back(p1);
    P5.push_back(p2);
    P5.push_back(p3);
    V.push_back(P5);

	p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=-0.5; p2(1)=+0.5; p2(2)=+0.5;
	p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
	P6.push_back(p1);
    P6.push_back(p2);
    P6.push_back(p3);
    V.push_back(P6);

	p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
	p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
	P7.push_back(p1);
    P7.push_back(p2);
    P7.push_back(p3);
    V.push_back(P7);

	p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
	p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
	P8.push_back(p1);
    P8.push_back(p2);
    P8.push_back(p3);
    V.push_back(P8);

	p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
	p3(0)=+0.5; p3(1)=-0.5; p3(2)=+0.5;
	P9.push_back(p1);
    P9.push_back(p2);
    P9.push_back(p3);
    V.push_back(P9);

	p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
	p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
	p3(0)=-0.5; p3(1)=-0.5; p3(2)=+0.5;
	P10.push_back(p1);
    P10.push_back(p2);
    P10.push_back(p3);
    V.push_back(P10);

	p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
	p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
	P11.push_back(p1);
    P11.push_back(p2);
    P11.push_back(p3);
    V.push_back(P11);

	p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
	p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
	p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
	P12.push_back(p1);
    P12.push_back(p2);
    P12.push_back(p3);
    V.push_back(P12);
	return V;
}

VectorOfVectorOfPoints two_triangles()
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2;
    facet_1.push_back(EPoint(0,0,0));
    facet_1.push_back(EPoint(4,0,0));
    facet_1.push_back(EPoint(2,2,0));

    facet_2.push_back(EPoint(0,0,0));
    facet_2.push_back(EPoint(2,-1,0));
    facet_2.push_back(EPoint(4,0,0));

    mesh.push_back(facet_1);
    mesh.push_back(facet_2);

    return mesh;
}

VectorOfVectorOfPoints trapezium()
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet;
    facet.push_back(EPoint(0,0,0));
    facet.push_back(EPoint(3,0,0));
    facet.push_back(EPoint(2,1,0));
    facet.push_back(EPoint(1,1,0));
    mesh.push_back(facet);
    return mesh;
}

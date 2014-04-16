#include "Mesh.hpp"
#include "MeshTest.hpp"

Point3dTriplet generate_a_single_triangle();
Point3dTriplet generate_a_single_triangle()
{
	Point3dTriplet P;
	P.p1.x= 1.0;
	P.p1.y= 2.1;
	P.p1.z= 3.2;
	P.p2.x= 2.1;
	P.p2.y= 3.7;
	P.p2.z= 4.5;
	P.p3.x= 3.1;
	P.p3.y= 4.5;
	P.p3.z= 6.7;
	return P;
}

TEST_F(MeshTest, should_be_able_to_build_a_mesh_from_a_single_triangle)
{
	VectorOfPoint3dTriplet facets;
	facets.push_back(generate_a_single_triangle());
	TriMesh m(facets);
}

TEST_F(MeshTest, should_be_able_to_build_a_mesh_from_a_single_triangle22)
{
	VectorOfPoint3dTriplet facets;
	facets.push_back(generate_a_single_triangle());
	TriMesh m = TriMesh(facets);
}

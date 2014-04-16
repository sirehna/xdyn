#include <math.h>
#include "MeshException.hpp"
#include "TriMesh.hpp"
#include "TriMeshTest.hpp"
#include "TriMeshTestData.hpp"

TEST_F(TriMeshTest, should_be_able_to_build_a_mesh_from_a_single_triangle)
{
	VectorOfPoint3dTriplet triangle;
	triangle.push_back(generate_a_single_triangle());
	TriMesh m(triangle);
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.size());
}

TEST_F(TriMeshTest, adding_twice_a_triangle_should_not_change_the_number_of_nodes)
{
	VectorOfPoint3dTriplet triangles;
	triangles.push_back(generate_a_single_triangle());
	triangles.push_back(generate_a_single_triangle());
	TriMesh m(triangles);
	ASSERT_EQ(2,m.facets.size());
	ASSERT_EQ(3,m.nodes.size());
}

TEST_F(TriMeshTest, should_be_able_to_evaluate_the_barycenter_of_a_single_triangle)
{
	VectorOfPoint3dTriplet facets;
	facets.push_back(generate_a_single_triangle());
	TriMesh m(facets);
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.size());
	ASSERT_DOUBLE_EQ((1.0+2.1+3.1)/3.0,m.facets.at(0).barycenter.x);
	ASSERT_DOUBLE_EQ((2.1+3.7+4.5)/3.0,m.facets.at(0).barycenter.y);
	ASSERT_DOUBLE_EQ((3.2+4.5+6.7)/3.0,m.facets.at(0).barycenter.z);
}

TEST_F(TriMeshTest, should_be_able_to_evaluate_the_area_of_a_single_triangle)
{
	VectorOfPoint3dTriplet facets;
	facets.push_back(generate_a_single_triangle());
	TriMesh m(facets);
	Xyz n1,n2,v;
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.size());
	n1.x= 2.1-1.0;
	n1.y= 3.7-2.1;
	n1.z= 4.5-3.2;
	n2.x= 3.1-1.0;
	n2.y= 4.5-2.1;
	n2.z= 6.7-3.2;
	v.x = n1.y*n2.z-n1.z*n2.y;
	v.y = n1.z*n2.x-n1.x*n2.z;
	v.z = n1.x*n2.y-n1.y*n2.x;
	ASSERT_DOUBLE_EQ(0.5*sqrt(v.x*v.x+v.y*v.y+v.z*v.z),m.facets.at(0).area);
}

TEST_F(TriMeshTest, should_throw_an_exception_for_a_degenerated_triangle)
{
	ASSERT_THROW(TriMesh m(generate_a_degenerated_triangle()),MeshException);
}

TEST_F(TriMeshTest, should_be_able_to_evaluate_the_normal_of_a_single_triangle)
{
	VectorOfPoint3dTriplet facets;
	facets.push_back(generate_a_single_triangle());
	TriMesh m(facets);
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.size());
	ASSERT_DOUBLE_EQ(+2.48/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal.x);
	ASSERT_DOUBLE_EQ(-1.12/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal.y);
	ASSERT_DOUBLE_EQ(-0.72/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal.z);
}

TEST_F(TriMeshTest, should_be_able_to_represent_a_cube)
{
	const VectorOfPoint3dTriplet C = generate_a_unit_cube();
	TriMesh m(C);
	ASSERT_EQ(12,m.facets.size());
	ASSERT_EQ(8,m.nodes.size());
	for (size_t i=0;i<12;++i)
		ASSERT_DOUBLE_EQ(0.5,m.facets.at(i).area)<<i;
}


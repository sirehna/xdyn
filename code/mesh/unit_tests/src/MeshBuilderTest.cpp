#include <cmath>

#include "MeshException.hpp"
#include "Mesh.hpp"
#include "MeshBuilderTest.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"

TEST_F(MeshBuilderTest, should_be_able_to_build_a_mesh_from_a_single_triangle)
{
	const Mesh m = MeshBuilder(one_triangle()).build();
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.cols());
}

TEST_F(MeshBuilderTest, adding_twice_a_triangle_should_not_change_the_number_of_nodes)
{
	VectorOfVectorOfPoints triangles;
	triangles.push_back(one_triangle());
	triangles.push_back(one_triangle());
	Mesh m = MeshBuilder(triangles).build();
	ASSERT_EQ(2,m.facets.size());
	ASSERT_EQ(3,m.nodes.cols());
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_barycenter_of_a_single_triangle)
{
	const Mesh m = MeshBuilder(one_triangle()).build();
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.cols());
	ASSERT_DOUBLE_EQ((1.0+2.1+3.1)/3.0,m.facets.at(0).barycenter(0));
	ASSERT_DOUBLE_EQ((2.1+3.7+4.5)/3.0,m.facets.at(0).barycenter(1));
	ASSERT_DOUBLE_EQ((3.2+4.5+6.7)/3.0,m.facets.at(0).barycenter(2));
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_area_of_a_single_triangle)
{
	const Mesh m = MeshBuilder(one_triangle()).build();
	Eigen::Vector3d n1,n2,v;
	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.cols());
	n1(0)= 2.1-1.0;
	n1(1)= 3.7-2.1;
	n1(2)= 4.5-3.2;
	n2(0)= 3.1-1.0;
	n2(1)= 4.5-2.1;
	n2(2)= 6.7-3.2;
	v(0) = n1(1)*n2(2)-n1(2)*n2(1);
	v(1) = n1(2)*n2(0)-n1(0)*n2(2);
	v(2) = n1(0)*n2(1)-n1(1)*n2(0);
	ASSERT_DOUBLE_EQ(0.5*sqrt(v(0)*v(0)+v(1)*v(1)+v(2)*v(2)),m.facets.at(0).area);
}

TEST_F(MeshBuilderTest, should_throw_if_a_triangle_is_degenerated)
{
    MeshBuilder builder(degenerated_triangle());
    ASSERT_THROW(builder.build(), MeshException);
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_normal_of_a_single_triangle)
{
	const Mesh m = MeshBuilder(one_triangle()).build();

	ASSERT_EQ(1,m.facets.size());
	ASSERT_EQ(3,m.nodes.cols());
	ASSERT_DOUBLE_EQ(+2.48/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal(0));
	ASSERT_DOUBLE_EQ(-1.12/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal(1));
	ASSERT_DOUBLE_EQ(-0.72/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),m.facets.at(0).unit_normal(2));
}

TEST_F(MeshBuilderTest, should_be_able_to_represent_a_cube)
{
	const Mesh m = MeshBuilder(unit_cube()).build();
	ASSERT_EQ(12,m.facets.size());
	ASSERT_EQ(8,m.nodes.cols());
	for (size_t i=0;i<12;++i)
		ASSERT_DOUBLE_EQ(0.5,m.facets.at(i).area)<<i;
}

TEST_F(MeshBuilderTest, two_triangles)
{
    const Mesh m = MeshBuilder(two_triangles()).build();
    ASSERT_EQ(2, m.facets.size());
    ASSERT_EQ(4, m.nodes.cols());
    // Check first point
    ASSERT_DOUBLE_EQ(0, m.nodes(0,0));
    ASSERT_DOUBLE_EQ(0, m.nodes(1,0));
    ASSERT_DOUBLE_EQ(0, m.nodes(2,0));
    // Check second point
    ASSERT_DOUBLE_EQ(4, m.nodes(0,1));
    ASSERT_DOUBLE_EQ(0, m.nodes(1,1));
    ASSERT_DOUBLE_EQ(0, m.nodes(2,1));
    // Check third point
    ASSERT_DOUBLE_EQ(2, m.nodes(0,2));
    ASSERT_DOUBLE_EQ(2, m.nodes(1,2));
    ASSERT_DOUBLE_EQ(0, m.nodes(2,2));
    // Check fourth point
    ASSERT_DOUBLE_EQ(2, m.nodes(0,3));
    ASSERT_DOUBLE_EQ(-1, m.nodes(1,3));
    ASSERT_DOUBLE_EQ(0, m.nodes(2,3));
    // Check first facet
    ASSERT_EQ(3,m.facets.at(0).index.size());
    ASSERT_EQ(0,m.facets.at(0).index.at(0));
    ASSERT_EQ(1,m.facets.at(0).index.at(1));
    ASSERT_EQ(2,m.facets.at(0).index.at(2));
    // Check second facet
    ASSERT_EQ(3,m.facets.at(1).index.size());
    ASSERT_EQ(0,m.facets.at(1).index.at(0));
    ASSERT_EQ(3,m.facets.at(1).index.at(1));
    ASSERT_EQ(1,m.facets.at(1).index.at(2));
}

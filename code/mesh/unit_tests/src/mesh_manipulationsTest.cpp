/*
 * mesh_manipulationsTest.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cady
 */

#include "base91x.hpp"
#include "mesh_manipulationsTest.hpp"
#include "GeometricTypes3d.hpp"
#include "mesh_manipulations.hpp"
#include "StlReader.hpp"
#include "TriMeshTestData.hpp"
#include "STL_data.hpp"
#include "MeshException.hpp"

#include <ssc/macros.hpp>

mesh_manipulationsTest::mesh_manipulationsTest() : a(ssc::random_data_generator::DataGenerator(321))
{
}

mesh_manipulationsTest::~mesh_manipulationsTest()
{
}

void mesh_manipulationsTest::SetUp()
{
}

void mesh_manipulationsTest::TearDown()
{
}

TEST_F(mesh_manipulationsTest, unit_normal)
{
//! [mesh_manipulationsTest unit_normal_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d n = unit_normal(M);
    ASSERT_DOUBLE_EQ(1, n.norm());
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(0));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(1));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(2));
//! [mesh_manipulationsTest unit_normal_example]
}

TEST_F(mesh_manipulationsTest, barycenter)
{
//! [mesh_manipulationsTest barycentre_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d G = barycenter(M);
    ASSERT_DOUBLE_EQ(1/3., (double)G(0));
    ASSERT_DOUBLE_EQ(1/3., (double)G(1));
    ASSERT_DOUBLE_EQ(1/3., (double)G(2));
//! [mesh_manipulationsTest barycentre_example]
}

TEST_F(mesh_manipulationsTest, area)
{
//! [mesh_manipulationsTest area_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    ASSERT_DOUBLE_EQ(1, area(M));
//! [mesh_manipulationsTest area_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity)
{
//! [mesh_manipulationsTest centre_of_gravity_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 4; M(0,2) = 3; M(0,3) = 1;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(2,(double)G(0));
    ASSERT_DOUBLE_EQ(4./9.,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
//! [mesh_manipulationsTest centre_of_gravity_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity_2)
{
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(0.5,(double)G(0));
    ASSERT_DOUBLE_EQ(0.5,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
}

TEST_F(mesh_manipulationsTest, can_read_and_write_binary_stl)
{
    const VectorOfVectorOfPoints mesh = unit_cube();
    std::stringstream ss;

    write_binary_stl(mesh, ss);
    const VectorOfVectorOfPoints parsed_mesh = read_binary_stl(ss);

    ASSERT_EQ(mesh.size(), parsed_mesh.size());
    for (size_t i = 0 ; i < mesh.size() ; ++i)
    {
        ASSERT_EQ(mesh[i].size(), parsed_mesh[i].size()) << " (for i = " << i << ")";
        for (size_t j = 0 ; j < mesh[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)mesh[i][j](0), (double)parsed_mesh[i][j](0)) << " (for i = " << i << " and j = " << j << ", x-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](1), (double)parsed_mesh[i][j](1)) << " (for i = " << i << " and j = " << j << ", y-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](2), (double)parsed_mesh[i][j](2)) << " (for i = " << i << " and j = " << j << ", z-coordinate)";
        }
    }
}

TEST_F(mesh_manipulationsTest, can_read_binary_stl_from_string)
{
    const VectorOfVectorOfPoints from_ascii = read_stl(test_data::cube());
    const std::string s = ";C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$AMAAAAAAAAAAAAAAA4}BAC'EBAA1hAAT|DAAA#AAA1hAAT|DAAA#AAAHLAAT|DAAAAAAAAAAAAAk_HAAASEAAcsBA+\\PAAA+CAA4}AAv(fAAA.QAAT|BABt#AAAAAAAAAAAAAC'JCAAOWAA+\\XAAAAAAAP(AA+\\XAAAAAAAP(AA+\\HAAAAAAAAAAAAAAAAAAATEAAcsAAv(vAAAAAAAcsBAv(PAAAAAAAcsAAv(PAAAAAAAAA4}FAAAAAAAAAAA4}CABtEBAAAAAAT|FAC'EBAAPLAAk_LAAA#AAAPLAAAA+\\vAAAAAAAAAAA+\\XAAASEAAAAAA+\\XAAAjBAA8sAAv(vAAA+CAAAAAAAABt#AAAAAAAAAAABtfAAA.QAA4}BAC'fAAA.QAAAAAABtfAAAxFAAAAAAAAC'#AAAuWAAAAAAC'fAAA.QAA4}BAC'fAAAxFAAAAAABtfAAAxFAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAAAAAC'fAAA.QAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAT|DAAAJCAAP(AAk_HAAAAAXLAA4}BAAAAAAA.QAAT|BABt#AAAHLAAk_DAC'#AAAHLAAk_DAAAAAAAAAAAAAk_HAAAAAAAP(AA+\\HAAAkBAAcsAAv(PAAAAAAAcsBAv(PAAAAAAA";
    const VectorOfVectorOfPoints from_binary = read_binary_stl(base<91>::decode(s));

    ASSERT_EQ(from_ascii.size(),from_binary.size());
    for (size_t i = 0 ; i < from_ascii.size() ; ++i)
    {
        ASSERT_EQ(from_ascii[i].size(),from_binary[i].size());
        for (size_t j = 0 ; j < from_ascii[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](0),(double)from_binary[i][j](0));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](1),(double)from_binary[i][j](1));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](2),(double)from_binary[i][j](2));
        }
    }
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_single_point)
{
    const VectorOfPoints single_point(1,a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_clockwise(single_point, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_segment)
{
    VectorOfPoints segment;
    segment.push_back(a.random<EPoint>());
    segment.push_back(a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_clockwise(segment, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_degenerated_triangle)
{
    // Doesn't much matter because there is no surface (so we don't know what "inside" means)
    ASSERT_TRUE(oriented_clockwise(degenerated_triangle(), a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_triangle)
{
    ASSERT_TRUE(oriented_clockwise(one_triangle(), EPoint(1,2,3)));
}

TEST_F(mesh_manipulationsTest, throws_if_not_all_facets_have_the_same_orientation)
{
    ASSERT_THROW(oriented_clockwise(unit_cube_with_incorrect_orientation(), a.random<EPoint>()), MeshException);
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_tetrahedron)
{
    ASSERT_FALSE(oriented_clockwise(tetrahedron(1,4,5,6), EPoint(4,5,6+sqrt(6.)/4.)));
    ASSERT_TRUE(oriented_clockwise(tetrahedron_clockwise(1,2,3,4), EPoint(2,3,4+sqrt(6.)/4.)));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_cube)
{
    ASSERT_FALSE(oriented_clockwise(unit_cube(), EPoint(0,0,0)));
    ASSERT_TRUE(oriented_clockwise(unit_cube_clockwise(), EPoint(0,0,0)));
}

#include "stl_data.hpp"
#include "stl_reader.hpp"
#include "stl_io_hdf5.hpp"
#include "stl_io_hdf5Test.hpp"

TEST_F(stl_io_hdf5Test, should_be_able_to_create_the_correct_number_of_points_and_triangles)
{
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    const SVectorOfVectorOfPoints sv(facets);
    ASSERT_EQ(3,sv.nPoints);
    ASSERT_EQ(1,sv.nTriangles);
}

TEST_F(stl_io_hdf5Test, should_be_able_to_export_triangulated_mesh)
{
    const std::string fileName("should_be_able_to_export_triangulated_mesh.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    {
        writeMeshToHdf5File(fileName, "mesh", facets);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(stl_io_hdf5Test, should_be_able_to_read_triangulated_mesh_with_one_facet)
{
    const std::string fileName("should_be_able_to_read_triangulated_mesh.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    {
        writeMeshToHdf5File(fileName, "mesh", facets);
    }
    {
        VectorOfVectorOfPoints facetsRead = readMeshFromHdf5File(fileName, "mesh");
        ASSERT_EQ(1, facetsRead.size());
        ASSERT_DOUBLE_EQ(1.0, facetsRead[0][0][0]);
        ASSERT_DOUBLE_EQ(2.1, facetsRead[0][0][1]);
        ASSERT_DOUBLE_EQ(3.2, facetsRead[0][0][2]);
        ASSERT_DOUBLE_EQ(2.1, facetsRead[0][1][0]);
        ASSERT_DOUBLE_EQ(3.7, facetsRead[0][1][1]);
        ASSERT_DOUBLE_EQ(4.5, facetsRead[0][1][2]);
        ASSERT_DOUBLE_EQ(3.1, facetsRead[0][2][0]);
        ASSERT_DOUBLE_EQ(4.5, facetsRead[0][2][1]);
        ASSERT_DOUBLE_EQ(6.7, facetsRead[0][2][2]);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

TEST_F(stl_io_hdf5Test, should_be_able_to_read_triangulated_mesh_with_several_facets)
{
    const std::string fileName("should_be_able_to_read_triangulated_mesh.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::three_facets());
    {
        writeMeshToHdf5File(fileName, "mesh", facets);
    }
    {
        VectorOfVectorOfPoints facetsRead = readMeshFromHdf5File(fileName, "mesh");
        ASSERT_EQ(3, facetsRead.size());
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[0][0][0]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[0][0][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[0][0][2]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[0][1][0]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[0][1][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[0][1][2]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[0][2][0]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[0][2][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[0][2][2]);

        ASSERT_DOUBLE_EQ(-0.5, facetsRead[1][0][0]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[1][0][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[1][0][2]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[1][1][0]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[1][1][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[1][1][2]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[1][2][0]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[1][2][1]);
        ASSERT_DOUBLE_EQ(+1.0, facetsRead[1][2][2]);

        ASSERT_DOUBLE_EQ(+0.5, facetsRead[2][0][0]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[2][0][1]);
        ASSERT_DOUBLE_EQ(+0.0, facetsRead[2][0][2]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[2][1][0]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[2][1][1]);
        ASSERT_DOUBLE_EQ(+0.0, facetsRead[2][1][2]);
        ASSERT_DOUBLE_EQ(-0.5, facetsRead[2][2][0]);
        ASSERT_DOUBLE_EQ(+0.5, facetsRead[2][2][1]);
        ASSERT_DOUBLE_EQ(+0.0, facetsRead[2][2][2]);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}


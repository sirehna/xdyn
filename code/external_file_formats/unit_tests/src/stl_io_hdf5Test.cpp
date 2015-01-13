#include "STL_data.hpp"
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

TEST_F(stl_io_hdf5Test, should_be_able_to_read_triangulated_mesh)
{
    const std::string fileName("should_be_able_to_read_triangulated_mesh.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    {
        writeMeshToHdf5File(fileName, "mesh", facets);
    }
    {
        VectorOfVectorOfPoints facetsRead = readMeshFromHdf5File(fileName, "mesh");
        ASSERT_EQ(1, facetsRead.size());
        ASSERT_DOUBLE_EQ(1.0, facetsRead.at(0)[0][0]);
        ASSERT_DOUBLE_EQ(2.1, facetsRead.at(0)[0][1]);
        ASSERT_DOUBLE_EQ(3.2, facetsRead.at(0)[0][2]);
        ASSERT_DOUBLE_EQ(2.1, facetsRead.at(0)[1][0]);
        ASSERT_DOUBLE_EQ(3.7, facetsRead.at(0)[1][1]);
        ASSERT_DOUBLE_EQ(4.5, facetsRead.at(0)[1][2]);
        ASSERT_DOUBLE_EQ(3.1, facetsRead.at(0)[2][0]);
        ASSERT_DOUBLE_EQ(4.5, facetsRead.at(0)[2][1]);
        ASSERT_DOUBLE_EQ(6.7, facetsRead.at(0)[2][2]);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}

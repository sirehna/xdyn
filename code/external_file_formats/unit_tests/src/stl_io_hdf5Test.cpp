#include "STL_data.hpp"
#include "stl_reader.hpp"
#include "stl_io_hdf5.hpp"
#include "stl_io_hdf5Test.hpp"

TEST_F(stl_io_hdf5Test, should_be_able_to_create_the_correct_number_of_points_and_triangles)
{
    const std::string fileName("writeMeshToHdf5File.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    const SVectorOfVectorOfPoints sv(facets);
    ASSERT_EQ(3,sv.nPoints);
    ASSERT_EQ(1,sv.nTriangles);
}

TEST_F(stl_io_hdf5Test, should_be_able_to_export_triangulated_mesh)
{
    const std::string fileName("writeMeshToHdf5File.h5");
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    {
        writeMeshToHdf5File(fileName, "mesh", facets);
    }
    EXPECT_EQ(0,remove(fileName.c_str()));
}


#include "StlReader.hpp"
#include "STL_data.hpp"
#include "StlReaderTest.hpp"

TEST_F(StlReaderTest, number_of_facets_should_be_correct_for_a_single_facet_file)
{
	const VectorOfPoint3dTriplet facets = read_stl(stl_single_facet());
	ASSERT_EQ(1, facets.size());
}

TEST_F(StlReaderTest, number_of_facets_should_be_correct_for_a_three_facet_file)
{
	const VectorOfPoint3dTriplet facets = read_stl(stl_three_facets());
	ASSERT_EQ(3, facets.size());
}

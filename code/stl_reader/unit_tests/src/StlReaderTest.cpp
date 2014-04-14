#include "StlReader.hpp"
#include "STL_data.hpp"
#include "StlReaderTest.hpp"

TEST_F(StlReaderTest, example)
{
	const VectorOfPoint3dTriplet res = read_stl(stl_single_facet());
}

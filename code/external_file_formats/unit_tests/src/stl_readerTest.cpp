#include "MeshException.hpp"
#include "stl_reader.hpp"
#include "stl_data.hpp"
#include "stl_readerTest.hpp"

TEST_F(StlReaderTest, should_be_able_to_detect_ascii_file)
{
    ASSERT_FALSE(is_stl_data_binary(test_data::single_facet()));
}

TEST_F(StlReaderTest, should_be_able_to_detect_ascii_file_even_with_starting_spaces)
{
    ASSERT_FALSE(is_stl_data_binary("  " + test_data::single_facet()));
    ASSERT_FALSE(is_stl_data_binary("\t" + test_data::single_facet()));
    ASSERT_FALSE(is_stl_data_binary("   \t  " + test_data::single_facet()));
    ASSERT_TRUE(is_stl_data_binary("   \t  dsq"));
    ASSERT_TRUE(is_stl_data_binary("dummy"));
}

TEST_F(StlReaderTest, number_of_facets_should_be_correct_for_a_single_facet_file)
{
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    ASSERT_EQ(1, facets.size());
}

TEST_F(StlReaderTest, number_of_facets_should_be_correct_for_a_three_facet_file)
{
    const VectorOfVectorOfPoints facets = read_stl(test_data::three_facets());
    ASSERT_EQ(3, facets.size());
}

TEST_F(StlReaderTest, should_be_able_to_read_a_single_facet_properly)
{
    const VectorOfVectorOfPoints facets = read_stl(test_data::single_facet());
    ASSERT_EQ(1, facets.size());
    ASSERT_DOUBLE_EQ(1.0, facets.at(0)[0][0]);
    ASSERT_DOUBLE_EQ(2.1, facets.at(0)[0][1]);
    ASSERT_DOUBLE_EQ(3.2, facets.at(0)[0][2]);
    ASSERT_DOUBLE_EQ(2.1, facets.at(0)[1][0]);
    ASSERT_DOUBLE_EQ(3.7, facets.at(0)[1][1]);
    ASSERT_DOUBLE_EQ(4.5, facets.at(0)[1][2]);
    ASSERT_DOUBLE_EQ(3.1, facets.at(0)[2][0]);
    ASSERT_DOUBLE_EQ(4.5, facets.at(0)[2][1]);
    ASSERT_DOUBLE_EQ(6.7, facets.at(0)[2][2]);
}

TEST_F(StlReaderTest, should_be_able_to_read_several_facets_properly)
{
    const VectorOfVectorOfPoints facets = read_stl(test_data::three_facets());
    ASSERT_EQ(3, facets.size());
    ASSERT_DOUBLE_EQ(-0.50, facets.at(0)[0][0]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(0)[0][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(0)[0][2]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(0)[1][0]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(0)[1][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(0)[1][2]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(0)[2][0]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(0)[2][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(0)[2][2]);

    ASSERT_DOUBLE_EQ(-0.50, facets.at(1)[0][0]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(1)[0][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(1)[0][2]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(1)[1][0]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(1)[1][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(1)[1][2]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(1)[2][0]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(1)[2][1]);
    ASSERT_DOUBLE_EQ(  1.0, facets.at(1)[2][2]);

    ASSERT_DOUBLE_EQ( 0.50, facets.at(2)[0][0]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(2)[0][1]);
    ASSERT_DOUBLE_EQ( 0.00, facets.at(2)[0][2]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(2)[1][0]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(2)[1][1]);
    ASSERT_DOUBLE_EQ( 0.00, facets.at(2)[1][2]);
    ASSERT_DOUBLE_EQ(-0.50, facets.at(2)[2][0]);
    ASSERT_DOUBLE_EQ( 0.50, facets.at(2)[2][1]);
    ASSERT_DOUBLE_EQ( 0.00, facets.at(2)[2][2]);
}

TEST_F(StlReaderTest, should_throw_an_exception_if_endsolid_keyword_not_found_before_end_of_file_is_reached)
{
    const std::string data("solid MYSOLID\nfacet normal 0.4 0.4 0.2\nouterloop\n");
    ASSERT_THROW(read_stl(data), MeshException);
}

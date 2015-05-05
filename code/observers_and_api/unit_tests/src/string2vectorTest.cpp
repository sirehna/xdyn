/*
 * vector2stringTest.cpp
 *
 *  Created on: May 5, 2015
 *      Author: cady
 */


#include "string2vectorTest.hpp"
#include "string2vector.hpp"

string2vectorTest::string2vectorTest() : a(ssc::random_data_generator::DataGenerator(8542222))
{
}

string2vectorTest::~string2vectorTest()
{
}

void string2vectorTest::SetUp()
{
}

void string2vectorTest::TearDown()
{
}

TEST_F(string2vectorTest, should_be_able_to_convert_vector_of_doubles_to_string_and_back)
{
    const size_t n=3;
    std::vector<double> v(n,0.0);
    v[0]=1.0;v[1]=2.0;v[2]=3.0;
    const std::string s = convert_vector_to_string(v);
    ASSERT_EQ(3*8,s.size());
    std::vector<double> v2=convert_string_to_vector<double>(s);
    ASSERT_EQ(n,v2.size());
    for (size_t i=0;i<n;++i)
    {
        ASSERT_EQ(v[i],v2[i]);
    }
}

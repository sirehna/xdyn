/*
 * maneuvering_DataSource_builderTest.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: cady
 */


#include "maneuvering_DataSource_builderTest.hpp"
#include "maneuvering_DataSource_builder.hpp"
#include "maneuvering_compiler.hpp"

maneuvering_DataSource_builderTest::maneuvering_DataSource_builderTest() : a(ssc::random_data_generator::DataGenerator(123545))
{
}

maneuvering_DataSource_builderTest::~maneuvering_DataSource_builderTest()
{
}

void maneuvering_DataSource_builderTest::SetUp()
{
}

void maneuvering_DataSource_builderTest::TearDown()
{
}

TEST_F(maneuvering_DataSource_builderTest, example)
{
    std::map<std::string, maneuvering::NodePtr> m;

    m["X"] = maneuvering::compile("2*Y+sqrt(x(t))", YamlRotation());
    m["Y"] = maneuvering::compile("y(t)^2", YamlRotation());

    ssc::data_source::DataSource ds;
    maneuvering::build_ds(ds, m);
    ds.check_in(__PRETTY_FUNCTION__);
    BodyStates states;
    states.x.record(10, 1024);
    states.y.record(10, 400);

    ds.set("t", 10.);
    ds.set("states", states);

    ASSERT_DOUBLE_EQ(160000, ds.get<double>("Y"));
    ASSERT_DOUBLE_EQ(320032, ds.get<double>("X"));
    ds.check_out();
}




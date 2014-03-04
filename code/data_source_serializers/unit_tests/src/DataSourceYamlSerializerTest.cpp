/*
 * DataSourceYamlSerializerTest.cpp
 *
 *  Created on: 26 févr. 2014
 *      Author: maroff
 */

#include "DataSourceYamlSerializerTest.hpp"
#include "DataSource.hpp"
#include "DataSourceModule.hpp"
#include "DataSourceYamlSerializer.hpp"
#include "common_serialization_constants.hpp"
#include "serialize_basic_types.hpp"

#include <sstream>

DataSourceYamlSerializerTest::DataSourceYamlSerializerTest() : a(DataGenerator(4012))
{
}

DataSourceYamlSerializerTest::~DataSourceYamlSerializerTest()
{
}

void DataSourceYamlSerializerTest::SetUp()
{
}

void DataSourceYamlSerializerTest::TearDown()
{
}

MODULE(M1,ds->set<double>("s1",123))
MODULE(M2,const double s1 = ds->get<double>("s1");\
          ds->set<double>("s2", s1))
MODULE(M3,ds->get<double>("s2"))
MODULE(OneInputTwoOutputs,const double x = ds->get<double>("x");\
          ds->set<double>("y1", 2*x);\
          ds->set<double>("y2", 3*x))

TEST_F(DataSourceYamlSerializerTest, can_serialize_empty_DataSource)
{
    std::stringstream actual;
    DataSourceYamlSerializer serializer(actual);
    DataSourceYamlSerializer serializer2(std::cout);
    DataSource ds;
    serializer.serialize(ds);
    std::stringstream expected;
    expected << common_serialization_constants::compiler_key << ": " << common_serialization_constants::compiler_name << std::endl
             << common_serialization_constants::data_source_key << ":" << std::endl
             << "    " << common_serialization_constants::modules_key << ":" << std::endl
             << "        []" << std::endl
             << "    " << common_serialization_constants::signals_key << ":" << std::endl
             << "        []";
    ASSERT_EQ(expected.str(), actual.str());
}

TEST_F(DataSourceYamlSerializerTest, example)
{
//! [DataSourceYamlSerializerTest example]
    DataSource ds;
    const std::string n1 = "M1";//a.random<std::string>();
    const std::string n2 = "M2";a.random<std::string>();
    const std::string n3 = "M3";a.random<std::string>();
    const std::string n4 = "M4";a.random<std::string>();
    ds.add<M1>(n1);
    ds.add<M2>(n2);
    ds.add<M3>(n3);
    ds.add<OneInputTwoOutputs>(n4);
    std::stringstream actual;
    DataSourceYamlSerializer serializer(actual);
//! [DataSourceYamlSerializerTest example]

//! [DataSourceYamlSerializerTest expected output]
    serializer.serialize(ds);
    std::stringstream expected;
    expected << common_serialization_constants::compiler_key << ": " << common_serialization_constants::compiler_name << std::endl
             << common_serialization_constants::data_source_key << ":" << std::endl
             << "    " << common_serialization_constants::modules_key << ":" << std::endl
             << "        -   name: " << n1 << std::endl
             << "            type: " << typeid(M1).name() << std::endl
             << "        -   name: " << n2 << std::endl
             << "            type: " << typeid(M2).name() << std::endl
             << "        -   name: " << n3 << std::endl
             << "            type: " << typeid(M3).name() << std::endl
             << "        -   name: " << n4 << std::endl
             << "            type: " << typeid(OneInputTwoOutputs).name() << std::endl
             << "    " << common_serialization_constants::signals_key << ":" << std::endl
             << "        []";

     ASSERT_EQ(expected.str(), actual.str());
//! [DataSourceYamlSerializerTest expected output]
}

TEST_F(DataSourceYamlSerializerTest, signals_should_be_properly_serialized)
{
    DataSource ds;
    const double d1 = a.random<double>().between(-10,10);
    const double d2 = a.random<double>().between(-10,10);
    const double d3 = a.random<double>().between(-10,10);
    ds.set("d1", d1);
    ds.set("d2", d2);
    ds.set("d3", d3);
    std::stringstream actual;
    DataSourceYamlSerializer serializer(actual);
    serializer.can_serialize<double>();
    serializer.serialize(ds);
    std::stringstream expected;
    expected << std::setprecision(15)
             << common_serialization_constants::compiler_key << ": " << common_serialization_constants::compiler_name << std::endl
             << common_serialization_constants::data_source_key << ":" << std::endl
             << "    " << common_serialization_constants::modules_key << ":" << std::endl
             << "        []" << std::endl
             << "    " << common_serialization_constants::signals_key << ":" << std::endl
             << "        -   name: d1" << std::endl
             << "            type: " << typeid(double).name() << std::endl
             << "            value: " << d1 << std::endl
             << "        -   name: d2" << std::endl
             << "            type: " << typeid(double).name() << std::endl
             << "            value: " << d2 << std::endl
             << "        -   name: d3" << std::endl
             << "            type: " << typeid(double).name() << std::endl
             << "            value: " << d3;
    ASSERT_EQ(expected.str(), actual.str());
}

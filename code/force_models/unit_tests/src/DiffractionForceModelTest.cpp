/*
 * DiffractionForceModelTest.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */


#include "DiffractionForceModelTest.hpp"
#include "DiffractionForceModel.hpp"
#include "yaml_data.hpp"

DiffractionForceModelTest::DiffractionForceModelTest() : a(ssc::random_data_generator::DataGenerator(545454))
{
}

DiffractionForceModelTest::~DiffractionForceModelTest()
{
}

void DiffractionForceModelTest::SetUp()
{
}

void DiffractionForceModelTest::TearDown()
{
}

TEST_F(DiffractionForceModelTest, parser)
{
    const YamlDiffraction r = DiffractionForceModel::parse(test_data::diffraction());
    ASSERT_EQ("test_ship.hdb", r.hdb_filename);
    ASSERT_EQ(0.696, r.calculation_point.x);
    ASSERT_EQ(0, r.calculation_point.y);
    ASSERT_EQ(1.418, r.calculation_point.z);
    ASSERT_TRUE(r.mirror);
}



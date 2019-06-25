/*
 * GRPCForceModelTest.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

//#include "BodyBuilder.hpp"
//#include "EnvironmentAndFrames.hpp"
#include "GRPCForceModel.hpp"
#include "GRPCForceModelTest.hpp"
#include "yaml_data.hpp"


GRPCForceModelTest::GRPCForceModelTest()
{
}

GRPCForceModelTest::~GRPCForceModelTest()
{
}

void GRPCForceModelTest::SetUp()
{
}

void GRPCForceModelTest::TearDown()
{
}

TEST_F(GRPCForceModelTest, can_parse_reference_frame)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("force-model:9002", input.url);
}

TEST_F(GRPCForceModelTest, can_parse_name)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("parametric oscillator", input.name);
}

TEST_F(GRPCForceModelTest, can_parse_rest_of_yaml_parameters)
{
    const auto input = GRPCForceModel::parse(test_data::gRPC_force_model());
    ASSERT_EQ("c: 1\n"
              "k: 60\n"
              "model: grpc\n"
              "name: parametric oscillator\n"
              "url: force-model:9002"
              , input.yaml);
}

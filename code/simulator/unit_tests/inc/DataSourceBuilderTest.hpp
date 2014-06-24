#ifndef DATASOURCEBUILDERTEST_HPP_
#define DATASOURCEBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "DataSource.hpp"
#include "GeometricTypes3d.hpp"
#include "YamlSimulatorInput.hpp"

class DataSourceBuilderTest : public ::testing::Test
{
    protected:
        DataSourceBuilderTest();
        virtual ~DataSourceBuilderTest();
        virtual void SetUp();
        virtual void TearDown();

        static const YamlSimulatorInput input;
        static const VectorOfVectorOfPoints mesh_data;

        static DataSource build_ds();

        DataGenerator a;
        DataSource ds;
};

#endif  /* DATASOURCEBUILDERTEST_HPP_ */

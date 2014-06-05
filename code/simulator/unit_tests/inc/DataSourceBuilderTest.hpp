#ifndef DATASOURCEBUILDERTEST_HPP_
#define DATASOURCEBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "DataSource.hpp"
#include "GeometricTypes3d.hpp"

class DataSourceBuilderTest : public ::testing::Test
{
    protected:
        DataSourceBuilderTest();
        virtual ~DataSourceBuilderTest();
        virtual void SetUp();
        virtual void TearDown();

		DataGenerator a;
		DataSource ds;
		VectorOfVectorOfPoints mesh_data;
};

#endif  /* DATASOURCEBUILDERTEST_HPP_ */

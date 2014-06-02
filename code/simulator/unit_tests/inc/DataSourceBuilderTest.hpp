#ifndef DATASOURCEBUILDERTEST_HPP_
#define DATASOURCEBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "DataSource.hpp"

class DataSourceBuilderTest : public ::testing::Test
{
    protected:
        DataSourceBuilderTest();
	    virtual ~DataSourceBuilderTest();
        virtual void SetUp();
        virtual void TearDown();

		DataGenerator a;
		DataSource ds;
};

#endif  /* DATASOURCEBUILDERTEST_HPP_ */

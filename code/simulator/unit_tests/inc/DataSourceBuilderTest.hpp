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
        void make_stl_file(const std::string& data, const std::string& filename) const;

		DataGenerator a;
		DataSource ds;
};

#endif  /* DATASOURCEBUILDERTEST_HPP_ */

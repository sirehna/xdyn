#ifndef DATASOURCEYAMLSERIALIZERTEST_HPP_
#define DATASOURCEYAMLSERIALIZERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DataSourceYamlSerializerTest : public ::testing::Test
{
    protected:
        DataSourceYamlSerializerTest();
	    virtual ~DataSourceYamlSerializerTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* DATASOURCEYAMLSERIALIZERTEST_HPP_ */

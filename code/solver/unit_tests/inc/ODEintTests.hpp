#ifndef ODEINTTESTS_HPP_
#define ODEINTTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class ODEintTests : public ::testing::Test
{
    protected:
        ODEintTests();
	    virtual ~ODEintTests();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* ODEINTTESTS_HPP_ */

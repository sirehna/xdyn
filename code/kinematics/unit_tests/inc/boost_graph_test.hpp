#ifndef BOOST_GRAPH_TEST_HPP_
#define BOOST_GRAPH_TEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class boost_graph_test : public ::testing::Test
{
    protected:
        boost_graph_test();
	    virtual ~boost_graph_test();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* BOOST_GRAPH_TEST_HPP_ */

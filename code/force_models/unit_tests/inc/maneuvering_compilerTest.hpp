/*
 * maneuvering_compilerTest.hpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#ifndef MANEUVERING_COMPILERTEST_HPP_
#define MANEUVERING_COMPILERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include <ssc/data_source.hpp>

#include "BodyStates.hpp"
#include "maneuvering_grammar.hpp"


class maneuvering_compilerTest : public ::testing::Test
{
    protected:
        maneuvering_compilerTest();
        virtual ~maneuvering_compilerTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        BodyStates states;
        ssc::data_source::DataSource ds;
        double t;
        maneuvering::grammar g;
};

#endif  /* MANEUVERING_COMPILERTEST_HPP_ */

/*
 * EverythingObserverTest.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_UNIT_TESTS_INC_EVERYTHINGOBSERVERTEST_HPP_
#define OBSERVERS_AND_API_UNIT_TESTS_INC_EVERYTHINGOBSERVERTEST_HPP_

#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class ListOfObservers;

class EverythingObserverTest : public ::testing::Test
{
    protected:
        EverythingObserverTest();
        virtual ~EverythingObserverTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

std::vector<Res> get_results(const ListOfObservers& observers);


#endif /* OBSERVERS_AND_API_UNIT_TESTS_INC_EVERYTHINGOBSERVERTEST_HPP_ */

/*
 * ListOfWaveModelsTest.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef LISTOFWAVEMODELSTEST_HPP_
#define LISTOFWAVEMODELSTEST_HPP_

#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class ListOfWaveModelsTest : public ::testing::Test
{
    protected:
        ListOfWaveModelsTest();
        virtual ~ListOfWaveModelsTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
        std::pair<std::vector<double>,std::vector<double> > spectrum(const std::vector<double>& x, const double fs) const;

};

#endif  /* LISTOFWAVEMODELSTEST_HPP_ */

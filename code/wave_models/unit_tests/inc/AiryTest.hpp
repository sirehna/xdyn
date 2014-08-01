/*
 * AiryTest.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef AIRYTEST_HPP_
#define AIRYTEST_HPP_

#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class AiryTest : public ::testing::Test
{
    protected:
        AiryTest();
        virtual ~AiryTest();
        virtual void SetUp();
        virtual void TearDown();
        std::pair<std::vector<double>,std::vector<double> > spectrum(const std::vector<double>& x, const double fs) const;
        DataGenerator a;

};

#endif  /* AIRYTEST_HPP_ */

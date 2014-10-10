/*
 * OutputTransformerTest.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMERRTEST_HPP_
#define OUTPUTTRANSFORMERRTEST_HPP_

#include <map>

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlSimulatorInput.hpp"

class OutputTransformerTest : public ::testing::Test
{
    protected:
        OutputTransformerTest();
        virtual ~OutputTransformerTest();
        virtual void SetUp();
        virtual void TearDown();

        double get(const std::vector<std::map<std::string,double> >& results, const size_t body_idx, const std::string& variable_name) const;

        static std::vector<std::map<std::string,double> > get_results(const std::string& yaml, const std::string& mesh_file="");

        ssc::random_data_generator::DataGenerator a;
        static const std::vector<std::map<std::string,double> > falling_ball;
        static const std::vector<std::map<std::string,double> > full_example;
        static const std::vector<std::map<std::string,double> > rolling_cube;
        static const std::vector<std::map<std::string,double> > falling_cube;

};

#endif  /* OUTPUTTRANSFORMERRTEST_HPP_ */

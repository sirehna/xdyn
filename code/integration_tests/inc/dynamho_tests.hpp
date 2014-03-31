#ifndef DYNAMHO_TESTS_HPP_
#define DYNAMHO_TESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "DataSource.hpp"

class SimulationStartStopParameters;
class dynamho_tests : public ::testing::Test
{
    protected:
        dynamho_tests();
        virtual ~dynamho_tests();
        virtual void SetUp();
        virtual void TearDown();

        DataGenerator a;
        std::string yaml;
        DataSource make_ds(const std::string& yaml) const;
        void initialize_DS_with_yaml_start_parameters(DataSource& ds, const SimulationStartStopParameters& ss) const;
};

#endif  /* DYNAMHO_TESTS_HPP_ */

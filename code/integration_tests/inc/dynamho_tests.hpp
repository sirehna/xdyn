#ifndef DYNAMHO_TESTS_HPP_
#define DYNAMHO_TESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "DataSource.hpp"

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
};

#endif  /* DYNAMHO_TESTS_HPP_ */

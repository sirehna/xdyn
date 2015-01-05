/*
 * mesh_manipulationsTest.hpp
 *
 *  Created on: May 20, 2014
 *      Author: cady
 */

#ifndef MESH_MANIPULATIONSTEST_HPP_
#define MESH_MANIPULATIONSTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "GeometricTypes3d.hpp"

class mesh_manipulationsTest : public ::testing::Test
{
    protected:
        mesh_manipulationsTest();
        virtual ~mesh_manipulationsTest();
        virtual void SetUp();
        virtual void TearDown();

        Matrix3x get_rectangle(const double w, const double h) const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* MESH_MANIPULATIONSTEST_HPP_ */

#ifndef KINEMATICSTESTS_HPP_
#define KINEMATICSTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class Kinematics;
class KinematicsTests : public ::testing::Test
{
    protected:
        KinematicsTests();
        virtual ~KinematicsTests();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* KINEMATICSTESTS_HPP_ */

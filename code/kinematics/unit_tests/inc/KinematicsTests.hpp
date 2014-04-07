#ifndef KINEMATICSTESTS_HPP_
#define KINEMATICSTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "AbstractFrame.hpp"

class KinematicsTests : public ::testing::Test
{
    protected:
        KinematicsTests();
        virtual ~KinematicsTests();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
        kinematics::FramePtr base;

        kinematics::FramePtr get_random_frame(const std::string& name);
};

#endif  /* KINEMATICSTESTS_HPP_ */

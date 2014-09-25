#ifndef MESH_INTERSECTOR_TEST_HPP
#define MESH_INTERSECTOR_TEST_HPP

#include "gtest/gtest.h"
#include "MeshIntersector.hpp"
#include <ssc/random_data_generator.hpp>

class MeshIntersectorTest : public ::testing::Test
{
    protected:
        MeshIntersectorTest();
        virtual ~MeshIntersectorTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif

#ifndef MESH_INTERSECTOR_TEST_HPP
#define MESH_INTERSECTOR_TEST_HPP

#include "gtest/gtest.h"
#include "MeshIntersector.hpp"
#include "DataGenerator.hpp"

class MeshIntersectorTest : public ::testing::Test
{
    protected:
        MeshIntersectorTest();
        virtual ~MeshIntersectorTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
};

#endif

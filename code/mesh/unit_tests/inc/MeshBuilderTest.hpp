#ifndef MESHBUILDER_TEST_HPP
#define MESHBUILDER_TEST_HPP

#include "gtest/gtest.h"
#include "Mesh.hpp"

class MeshBuilderTest : public ::testing::Test
{
    protected:
        MeshBuilderTest();
        virtual void SetUp(){}
        virtual void TearDown(){}
};

size_t get_oriented_edge_index(const size_t &e);

#endif

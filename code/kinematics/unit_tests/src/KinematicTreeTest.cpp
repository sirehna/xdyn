/*
 * KinematicTreeTest.cpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */


#include "KinematicTreeTest.hpp"
#include "KinematicTree.hpp"

KinematicTreeTest::KinematicTreeTest() : a(DataGenerator(78784))
{
}

KinematicTreeTest::~KinematicTreeTest()
{
}

void KinematicTreeTest::SetUp()
{
}

void KinematicTreeTest::TearDown()
{
}

TEST_F(KinematicTreeTest, example)
{
//! [KinematicTreeTest example]
    KinematicTree t;
    t.add("a","b");
    t.add("a","c");
    t.add("b","d");
    t.add("c","e");
    t.add("c","f");
    t.add("d","g");
    t.add("d","h");
    t.add("d","i");
//! [KinematicTreeTest example]
//! [KinematicTreeTest expected output]
//! [KinematicTreeTest expected output]
}

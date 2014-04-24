/*
 * KinematicTreeTest.cpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */


#include "KinematicTreeTest.hpp"
#include "KinematicTree.hpp"
#include "test_macros.hpp"

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
    const std::vector<std::pair<std::string,std::string> > path_from_c_to_d = t.get_path("c", "d");
    ASSERT_EQ(3, path_from_c_to_d.size());
    ASSERT_EQ("c", path_from_c_to_d.at(0).first);
    ASSERT_EQ("a", path_from_c_to_d.at(0).second);
    ASSERT_EQ("a", path_from_c_to_d.at(1).first);
    ASSERT_EQ("b", path_from_c_to_d.at(1).second);
    ASSERT_EQ("b", path_from_c_to_d.at(2).first);
    ASSERT_EQ("d", path_from_c_to_d.at(2).second);
    const std::vector<std::pair<std::string,std::string> > path_from_e_to_h = t.get_path("e", "h");
    ASSERT_EQ(5, path_from_e_to_h.size());
    ASSERT_EQ("e", path_from_e_to_h.at(0).first);
    ASSERT_EQ("c", path_from_e_to_h.at(0).second);
    ASSERT_EQ("c", path_from_e_to_h.at(1).first);
    ASSERT_EQ("a", path_from_e_to_h.at(1).second);
    ASSERT_EQ("a", path_from_e_to_h.at(2).first);
    ASSERT_EQ("b", path_from_e_to_h.at(2).second);
    ASSERT_EQ("b", path_from_e_to_h.at(3).first);
    ASSERT_EQ("d", path_from_e_to_h.at(3).second);
    ASSERT_EQ("d", path_from_e_to_h.at(4).first);
    ASSERT_EQ("h", path_from_e_to_h.at(4).second);
//! [KinematicTreeTest expected output]
}

TEST_F(KinematicTreeTest, case_with_more_than_one_solution)
{
    KinematicTree t;
    t.add("a","b");
    t.add("a","c");
    t.add("b","d");
    t.add("c","e");
    t.add("c","f");
    t.add("d","g");
    t.add("d","h");
    t.add("d","i");
    t.add("e","d");
    const std::vector<std::pair<std::string,std::string> > path_from_c_to_d = t.get_path("c", "d");
    ASSERT_EQ(2, path_from_c_to_d.size());
    ASSERT_EQ("c", path_from_c_to_d.at(0).first);
    ASSERT_EQ("e", path_from_c_to_d.at(0).second);
    ASSERT_EQ("e", path_from_c_to_d.at(1).first);
    ASSERT_EQ("d", path_from_c_to_d.at(1).second);
    const std::vector<std::pair<std::string,std::string> > path_from_e_to_h = t.get_path("e", "h");
    ASSERT_EQ(2, path_from_e_to_h.size());
    ASSERT_EQ("e", path_from_e_to_h.at(0).first);
    ASSERT_EQ("d", path_from_e_to_h.at(0).second);
    ASSERT_EQ("d", path_from_e_to_h.at(1).first);
    ASSERT_EQ("h", path_from_e_to_h.at(1).second);
}

TEST_F(KinematicTreeTest, can_retrieve_path_to_self)
{
    KinematicTree t;
    t.add("a","b");
    const std::vector<std::pair<std::string,std::string> > path_from_a_to_a = t.get_path("a", "a");
    ASSERT_EQ(1, path_from_a_to_a.size());
    ASSERT_EQ("a", path_from_a_to_a.at(0).first);
    ASSERT_EQ("a", path_from_a_to_a.at(0).second);
}

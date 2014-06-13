/*
 * KinematicsTree.hpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */

#ifndef KINEMATICTREE_HPP_
#define KINEMATICTREE_HPP_

#include <string>
#include "tr1_macros.hpp"
#include TR1INC(memory)
#include <vector>
#include <utility> // std::pair

typedef std::vector<std::pair<std::string,std::string> > PathType;

/** \author cec
 *  \date 23 avr. 2014, 14:50:11
 *  \brief Computes the shortest Transform path between two reference frames.
 *  \details Uses boost::graph's implementation of Dijkstra's algorithm. The tree is an undirected graph.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/KinematicsTreeTest.cpp KinematicsTreeTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/KinematicsTreeTest.cpp KinematicsTreeTest expected output
 */
class KinematicTree
{
    public:
        KinematicTree();
        /**  \author cec
          *  \date 23 avr. 2014, 14:53:58
          *  \brief Adds a connection between two (existing or non-existing) nodes in the kinematic tree
          *  \snippet kinematics/unit_tests/src/KinematicsTreeTest.cpp KinematicsTreeTest add_example
          */
        void add(const std::string& frame_A, //!< First frame to connect
                 const std::string& frame_B  //!< Second frame to connect
                 );

        /** \author cec
          *  \date 23 avr. 2014, 15:10:30
          *  \brief Computes the shortest path between two reference frames
          *  \details Uses Dijkstra's algorithm
          *  \returns Vector of links (pairs of nodes) from frame_A to frame_B.
          *  \snippet kinematics/unit_tests/src/KinematicTreeTest.cpp KinematicTreeTest get_path_example
          */
        PathType get_path(const std::string& frame_A, //!< First node in the path
                          const std::string& frame_B  //!< Last node in the path
                         );

    private:
        class Impl;
        TR1(shared_ptr<Impl>) pimpl;
};

#endif /* KINEMATICTREE_HPP_ */

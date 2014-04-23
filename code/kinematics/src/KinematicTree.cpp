/*
 * KinematicsTree.cpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */

#include "KinematicTree.hpp"

class KinematicTree::Impl
{
    public:
        Impl()  {}

        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
            }
            return *this;
        }

        Impl(const Impl& )
        {
        }
};


KinematicTree::KinematicTree() : pimpl(new Impl())
{

}

void KinematicTree::add(const std::string& frame_A, const std::string& frame_B)
{
    (void) frame_A;
    (void) frame_B;
}

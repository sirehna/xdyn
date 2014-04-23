/*
 * Kinematics.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef KINEMATICS_HPP_
#define KINEMATICS_HPP_

#include <string>
#include <tr1/memory>

namespace kinematics
{
    class Transform;
}

/** \author cec
 *  \date 23 avr. 2014, 10:34:05
 *  \brief Lazily computes the optimal transform between two reference frames.
 *  \details When user requests a transform between two frames:
 *           - Looks for it in the underlying internal DataSource object
 *           - If the corresponding transform wasn't found, see if it's the inverse of a known transform
 *           - If it isn't, see if it can be computed using a PathComputer object.
 *           - Otherwise throw a KinematicsException.
 *  \ingroup kinematics
 *  \section ex1 Example
 *  \snippet kinematics/unit_tests/src/KinematicsTest.cpp KinematicsTest example
 *  \section ex2 Expected output
 *  \snippet kinematics/unit_tests/src/KinematicsTest.cpp KinematicsTest expected output
 */
class Kinematics
{
    public:
        /** \author cec
          *  \date 23 avr. 2014, 10:45:40
          *  \author cec
          *  \brief Adds a transform between two reference frames to *this
          *  \snippet kinematics/unit_tests/src/KinematicsTest.cpp KinematicsTest add_example
          */
        void add(const kinematics::Transform& transform_to_add //!< Transform to add
                );

    private:
};

#endif /* KINEMATICS_HPP_ */

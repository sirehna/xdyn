/*
 * transform_double_equal.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef TRANSFORM_DOUBLE_EQUAL_HPP_
#define TRANSFORM_DOUBLE_EQUAL_HPP_

namespace kinematics
{
	class Transform;
}

bool double_equal(const kinematics::Transform& t1, const kinematics::Transform& t2, const double eps=0);

#endif  /* TRANSFORM_DOUBLE_EQUAL_HPP_ */

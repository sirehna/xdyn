/*
 * KinematicsTests.cpp
 *
 *  Created on: 1 avr. 2014
 *      Author: maroff
 */

#include "KinematicsTests.hpp"
#include "Kinematics.hpp"
#include "random_kinematics.hpp"
#include "test_macros.hpp"
#include "almost_equal.hpp"

bool double_equal(const kinematics::Transform& t1, const kinematics::Transform& t2, const double eps=0);
bool double_equal(const kinematics::Transform& t1, const kinematics::Transform& t2, const double eps)
{
    if (t1.get_from_frame() != t2.get_from_frame())
    {
        std::cerr << "'from' frames don't match." << std::endl;
        return false;
    }
    if (t1.get_to_frame() != t2.get_to_frame())
    {
        std::cerr << "'to' frames don't match." << std::endl;
        return false;
    }
    const Point u1(t1.get_from_frame(), 1, 0, 0);
    const Point u2(t1.get_from_frame(), 0, 1, 0);
    const Point u3(t1.get_from_frame(), 0, 0, 1);

    const Point u11 = t1*u1;
    const Point u12 = t1*u2;
    const Point u13 = t1*u3;
    const Point u21 = t2*u1;
    const Point u22 = t2*u2;
    const Point u23 = t2*u3;

    const std::vector<double> std_u11(&u11.x, &u11.x + 3);
    const std::vector<double> std_u12(&u12.x, &u12.x + 3);
    const std::vector<double> std_u13(&u13.x, &u13.x + 3);
    const std::vector<double> std_u21(&u21.x, &u21.x + 3);
    const std::vector<double> std_u22(&u22.x, &u22.x + 3);
    const std::vector<double> std_u23(&u23.x, &u23.x + 3);

    if (not(almost_equal(std_u11, std_u21, eps)))
    {
        std::cerr << "t1*(1,0,0) = " << u11.v.transpose() << std::endl;
        std::cerr << "t2*(1,0,0) = " << u21.v.transpose() << std::endl;
        return false;
    }

    if (not(almost_equal(std_u12, std_u22, eps)))
    {
        std::cerr << "t1*(0,1,0) = " << u12.v.transpose() << std::endl;
        std::cerr << "t2*(0,1,0) = " << u22.v.transpose() << std::endl;
        return false;
    }
    if (not(almost_equal(std_u13, std_u23, eps)))
    {
        std::cerr << "t1*(0,0,1) = " << u13.v.transpose() << std::endl;
        std::cerr << "t2*(0,0,1) = " << u23.v.transpose() << std::endl;
        return false;
    }

    return true;
}

KinematicsTests::KinematicsTests() : a(DataGenerator(122))
{
}

KinematicsTests::~KinematicsTests()
{
}

void KinematicsTests::SetUp()
{
}

void KinematicsTests::TearDown()
{
}

kinematics::Transform identity(const std::string& frame);
kinematics::Transform identity(const std::string& frame)
{
    return kinematics::Transform(Point(frame,0,0,0), frame);
}

TEST_F(KinematicsTests, can_add_a_transform_to_a_kinematics_object)
{
    Kinematics k;
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
		const auto bTa = random_transform(a, a.random<std::string>(), a.random<std::string>());
		k.add(bTa);
    }
}

TEST_F(KinematicsTests, can_retrieve_a_transform)
{
	Kinematics k;
	const std::string from_frame = a.random<std::string>();
	const std::string to_frame = a.random<std::string>();
	const auto bTa = random_transform(a, from_frame, to_frame);
	k.add(bTa);
	const auto transform = k.get(from_frame, to_frame);
	ASSERT_TRUE(double_equal(transform, bTa));
}

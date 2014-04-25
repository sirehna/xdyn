/*
 * WrenchTest.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: cady
 */

#include "WrenchTest.hpp"
#include "random_kinematics.hpp"
#include "KinematicsException.hpp"
#include "Wrench.hpp"
#include "rotation_matrix_builders.hpp"

#include <cmath>

#define PI (4.*atan(1.))

WrenchTest::WrenchTest() : a(DataGenerator(1215))
{
}

WrenchTest::~WrenchTest()
{
}

void WrenchTest::SetUp()
{
}

void WrenchTest::TearDown()
{
}

TEST_F(WrenchTest, can_change_reference_point)
{
	const std::string frame = a.random<std::string>();
	const Point A(frame,4,-2,9);
	Wrench wA(A);
	wA.X = 11;
	wA.Y = -20;
	wA.Z = 89;
	wA.K = 1;
	wA.M = 2;
	wA.N = 3;
	const Point B(frame, 0, 3, -9);
	const Wrench wB = wA.change_point_of_application(B);
	ASSERT_DOUBLE_EQ(wA.X, wB.X);
	ASSERT_DOUBLE_EQ(wA.Y, wB.Y);
	ASSERT_DOUBLE_EQ(wA.Z, wB.Z);
	ASSERT_DOUBLE_EQ(-84, wB.K);
	ASSERT_DOUBLE_EQ(-156,wB.M);
	ASSERT_DOUBLE_EQ(-22, wB.N);
}

TEST_F(WrenchTest, cannot_change_reference_point_if_ref_frames_dont_match)
{
	for (size_t i = 0 ; i < 20 ; ++i)
	{
		Wrench w(random_point(a));
		ASSERT_THROW(w.change_point_of_application(random_point(a)), KinematicsException);
	}
}

TEST_F(WrenchTest, can_add_two_wrenches)
{
	for (size_t i = 0 ; i < 20 ; ++i)
	{
		const Point P = random_point(a);
		Wrench w1(P);
		Wrench w2(P);
		w1.X = a.random<double>();
		w1.Y = a.random<double>();
		w1.Z = a.random<double>();
		w1.K = a.random<double>();
		w1.M = a.random<double>();
		w1.N = a.random<double>();
		w2.X = a.random<double>();
		w2.Y = a.random<double>();
		w2.Z = a.random<double>();
		w2.K = a.random<double>();
		w2.M = a.random<double>();
		w2.N = a.random<double>();
		const Wrench w = w1 + w2;
		ASSERT_DOUBLE_EQ(w.X, w1.X+w2.X);
		ASSERT_DOUBLE_EQ(w.Y, w1.Y+w2.Y);
		ASSERT_DOUBLE_EQ(w.Z, w1.Z+w2.Z);
		ASSERT_DOUBLE_EQ(w.K, w1.K+w2.K);
		ASSERT_DOUBLE_EQ(w.M, w1.M+w2.M);
		ASSERT_DOUBLE_EQ(w.N, w1.N+w2.N);
	}
}

TEST_F(WrenchTest, cannot_add_two_wrenches_if_the_reference_points_dont_match)
{
	for (size_t i = 0 ; i < 20 ; ++i)
	{
		const Wrench w1(random_point(a));
		const Wrench w2(random_point(a));
		ASSERT_THROW(w1+w2, KinematicsException);
	}
}

TEST_F(WrenchTest, can_substract_two_wrenches)
{
	for (size_t i = 0 ; i < 20 ; ++i)
	{
		const Point P = random_point(a);
		Wrench w1(P);
		Wrench w2(P);
		w1.X = a.random<double>();
		w1.Y = a.random<double>();
		w1.Z = a.random<double>();
		w1.K = a.random<double>();
		w1.M = a.random<double>();
		w1.N = a.random<double>();
		w2.X = a.random<double>();
		w2.Y = a.random<double>();
		w2.Z = a.random<double>();
		w2.K = a.random<double>();
		w2.M = a.random<double>();
		w2.N = a.random<double>();
		const Wrench w = w1 - w2;
		ASSERT_DOUBLE_EQ(w.X, w1.X-w2.X);
		ASSERT_DOUBLE_EQ(w.Y, w1.Y-w2.Y);
		ASSERT_DOUBLE_EQ(w.Z, w1.Z-w2.Z);
		ASSERT_DOUBLE_EQ(w.K, w1.K-w2.K);
		ASSERT_DOUBLE_EQ(w.M, w1.M-w2.M);
		ASSERT_DOUBLE_EQ(w.N, w1.N-w2.N);
	}
}

TEST_F(WrenchTest, cannot_substract_two_wrenches_if_the_reference_points_dont_match)
{
	for (size_t i = 0 ; i < 20 ; ++i)
	{
		const Wrench w1(random_point(a));
		const Wrench w2(random_point(a));
		ASSERT_THROW(w1-w2, KinematicsException);
	}
}

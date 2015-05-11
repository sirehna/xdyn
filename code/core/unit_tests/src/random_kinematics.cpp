/*
 * random_kinematics.cpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */


#include "random_kinematics.hpp"
#include "ssc/random_data_generator/DataGenerator.hpp"
#include "ssc/kinematics/rotation_matrix_builders.hpp"

#include <cmath> // For atan

#define PI (4.*atan(1.))

using namespace ssc::kinematics;

namespace ssc
{
    namespace random_data_generator
    {
        template <> RotationMatrix ssc::random_data_generator::TypedScalarDataGenerator<RotationMatrix>::get() const
        {
            double lambda1 = random<double>().between(-1,1);
            double lambda2 = random<double>().between(-1,1);
            double lambda3 = random<double>().between(-1,1);
            const double D = sqrt(lambda1*lambda1+lambda2*lambda2+lambda3*lambda3);
            lambda1 = lambda1/D;
            lambda2 = lambda2/D;
            lambda3 = lambda3/D;
            const double beta = random<double>().between(-PI,PI);
            return rot(lambda1, lambda2, lambda3, beta);
        }

        template <> Point TypedScalarDataGenerator<Point>::get() const
        {
            const double x = random<double>().between(-10,10);
            const double y = random<double>().between(-10,10);
            const double z = random<double>().between(-10,10);
            return Point(random<std::string>(), x, y, z);
        }


    }
}

Point random_point_in_frame(const ssc::random_data_generator::DataGenerator& a, const std::string& frame)
{
    const double x = a.random<double>().between(-10,10);
    const double y = a.random<double>().between(-10,10);
    const double z = a.random<double>().between(-10,10);
    return Point(frame, x, y, z);
}

Point random_point(const ssc::random_data_generator::DataGenerator& a)
{
    return random_point_in_frame(a, a.random<std::string>());
}

PointMatrix random_point_matrix(const ssc::random_data_generator::DataGenerator& a)
{
    return random_point_matrix_in_frame(a, a.random<std::string>());
}

PointMatrix random_point_matrix_in_frame(const ssc::random_data_generator::DataGenerator& a, const std::string& frame)
{
    PointMatrix p(frame, 100);
    for (size_t i=0;i<100;++i)
    {
        p.m(0,(long)i)=a.random<double>().between(-10,10);
        p.m(1,(long)i)=a.random<double>().between(-10,10);
        p.m(2,(long)i)=a.random<double>().between(-10,10);
    }
    return p;
}

Transform random_transform(const ssc::random_data_generator::DataGenerator& a, const std::string& from_frame, const std::string& to_frame)
{
    const RotationMatrix R = a.random<RotationMatrix>();
    return Transform(random_point_in_frame(a, from_frame), R, to_frame);
}


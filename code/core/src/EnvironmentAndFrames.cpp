/*
 * EnvironmentAndFrames.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include <ssc/exception_handling.hpp>

#include "SurfaceElevationInterface.hpp"
#include "EnvironmentAndFrames.hpp"
#include "Observer.hpp"

using namespace ssc::kinematics;


class EnvironmentAndFramesException: public ::ssc::exception_handling::Exception
{
    public:
    EnvironmentAndFramesException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};


EnvironmentAndFrames::EnvironmentAndFrames() : w(),
                                               k(KinematicsPtr(new Kinematics())),
                                               rho(0),
                                               g(0),
                                               rot()
{
}

void EnvironmentAndFrames::feed(Observer& observer, double t) const
{
    (void) observer;
    (void) t;
    /*
    std::vector<std::string> address;
    address.push_back("waveElevation");
    const auto kk = w->get_waves_on_mesh_as_a_grid(k, t);
    if(kk.z.size()!=0)
    {
        observer.write(
                std::vector<double>(kk.x.data(), kk.x.data() + kk.x.size()),
                address+"/x", std::vector<std::string>{},"wave");
    }
    */
}

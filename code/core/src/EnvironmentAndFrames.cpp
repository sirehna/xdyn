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
void EnvironmentAndFrames::feed(
        Observer& observer, double t,
        const std::vector<BodyPtr>& bodies, const StateType& state) const
{
    try
    {
        if (w.get())
        {
            for (size_t i = 0 ; i < bodies.size() ; ++i)
            {
                bodies[i]->update_kinematics(state,k);
            }
            const auto kk = w->get_waves_on_mesh_as_a_grid(k, t);
            if(kk.z.size()!=0)
            {
                const auto address = DataAddressing(std::vector<std::string>{"waveElevation"},"wave");
                observer.write(kk, address);
            }
        }
    }
    catch (const ssc::kinematics::KinematicsException& e)
    {
        std::stringstream ss;
        ss << "Error when calculating waves on mesh: the output reference frame does not exist (caught the following exception: " << e.what() << ")";
        THROW(__PRETTY_FUNCTION__, EnvironmentAndFramesException, ss.str());
    }
}

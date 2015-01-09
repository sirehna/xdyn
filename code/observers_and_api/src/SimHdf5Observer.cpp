/*
 * SimHdf5Observer.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#include <vector>
#include <string>

#include "SimHdf5Observer.hpp"
#include "SimHdf5ObserverBuilder.hpp"
#include "SimHdf5WaveObserver.hpp"
#include "Sim.hpp"
#include "h5_interface.hpp"
#include "h5_version.hpp"

typedef std::map<std::string, std::map< std::string,ssc::kinematics::Vector6d > > OuputtedForces;

class SimHdf5Observer::Impl
{
    public:
        Impl(const std::string& fileName_,
             const std::string& baseName_,
             const VectorOfStringModelForEachBody& s,
             const std::pair<std::size_t,std::size_t>& waves_mesh_size) :
            fileName(fileName_),h5File(H5::H5File(fileName_, H5F_ACC_TRUNC)),
            sStates(h5File, baseName_.empty()?"states":baseName_+"/states", H5_CreateIdStates(s)),
            sEfforts(h5File, baseName_.empty()?"efforts":baseName_+"/efforts", H5_CreateIdEfforts(s)),
            sWaves(h5File, baseName_.empty()?"waveElevation":baseName_+"/waveElevation", waves_mesh_size.first, waves_mesh_size.second)
        {
            h5_writeFileDescription(h5File);
        }

        void observe_states(const double t, const Sim& s);
        void observe_efforts(const double t, const Sim& s);
        void observe_waves(const double t, const Sim& s);
        std::string fileName;
        H5::H5File h5File;
        H5_Serialize<H5Res> sStates;
        H5_Serialize<H5Res> sEfforts;
        SimHdf5WaveObserver sWaves;
};

#include "yaml2eigen.hpp"
ssc::kinematics::EulerAngles convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations);
ssc::kinematics::EulerAngles convert(const ssc::kinematics::RotationMatrix& R, const YamlRotation& rotations)
{
    using namespace ssc::kinematics;
    if (rotations.order_by == "angle")
    {
        if (match(rotations.convention, "z", "y'", "x''"))
            return euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
        std::stringstream ss;
        ss << "Rotation convention '" << rotations.convention.at(0) << "," << rotations.convention.at(1) << "," << rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, std::string("Ordering rotations by '") + rotations.order_by + "' is not currently supported");
    }
    return EulerAngles();
}
// ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x) const;

void SimHdf5Observer::Impl::observe_states(const double t, const Sim& s)
{
    const size_t nbody = s.get_names_of_bodies().size();
    std::vector<double> x;
    for (size_t i=0;i<nbody;++i)
    {
        ssc::kinematics::EulerAngles e;
        x.push_back(*_X(s.state,i));
        x.push_back(*_Y(s.state,i));
        x.push_back(*_Z(s.state,i));
        x.push_back(*_U(s.state,i));
        x.push_back(*_V(s.state,i));
        x.push_back(*_W(s.state,i));
        x.push_back(*_P(s.state,i));
        x.push_back(*_Q(s.state,i));
        x.push_back(*_R(s.state,i));
        x.push_back(*_QR(s.state,i));
        x.push_back(*_QI(s.state,i));
        x.push_back(*_QJ(s.state,i));
        x.push_back(*_QK(s.state,i));
        x.push_back(e.phi);
        x.push_back(e.theta);
        x.push_back(e.psi);
    }
    sStates << H5Res(t, x);
}

void SimHdf5Observer::Impl::observe_efforts(const double t, const Sim& s)
{
    const H5Res res(t, s.get_forces_as_a_vector_of_doubles());
    sEfforts << res;
}

void SimHdf5Observer::Impl::observe_waves(const double t, const Sim& s)
{
    const auto w = s.get_waves_as_a_grid(t);
    if(w.z.size()!=0)
    {
        sWaves << w;
    }
}

SimHdf5Observer::SimHdf5Observer(const std::string& fileName, const Sim& s) :
        pimpl(new Impl(fileName, "simu01", s.get_vector_of_string_model_for_each_body(), s.get_waves_mesh_size()))
{
}

SimHdf5Observer::SimHdf5Observer(const std::string& fileName, const std::string& baseName, const Sim& s) :
        pimpl(new Impl(fileName, baseName, s.get_vector_of_string_model_for_each_body(), s.get_waves_mesh_size()))
{
}

void SimHdf5Observer::observe(const Sim& sys, const double t)
{
    pimpl->observe_states(t, sys);
    pimpl->observe_efforts(t, sys);
    pimpl->observe_waves(t, sys);
}

void SimHdf5Observer::observe_states(const Sim& sys, const double t)
{
    pimpl->observe_states(t, sys);
}

void SimHdf5Observer::observe_efforts(const Sim& sys, const double t)
{
    pimpl->observe_efforts(t, sys);
}

void SimHdf5Observer::observe_waves(const Sim& sys, const double t)
{
    pimpl->observe_waves(t, sys);
}

/*
 * SimHdf5Observer.cpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#include <vector>
#include <string>

#include "SimHdf5Observer.hpp"
#include "Sim.hpp"
#include "h5_interface.hpp"

struct H5Res
{
    double t; //<! Instant at which the states correspond
    std::vector<double> v; //<! Values of the states of the system
    H5Res() : t(0), v(){}
    H5Res(const double t_, const std::vector<double>& v_):t(t_),v(v_){}
};

H5::CompType H5_InterfaceH5ResCreateId(const std::vector<std::string>& v);
H5::CompType H5_InterfaceH5ResCreateId(const VectorOfStringModelForEachBody& v);

template<class T> std::string t_to_string(T i);
template<class T> std::string t_to_string(T i)
{
    std::ostringstream ss;
    std::string s;
    ss << i;
    s = ss.str();
    return s;
}
template<size_t> std::string t_to_string(size_t x);

H5::CompType H5_InterfaceH5ResCreateId(const std::vector<std::string>& v)
{
    const size_t n = v.size();
    H5::CompType mtype = H5::CompType(sizeof(double)+n*NB_OF_STATES_PER_BODY*sizeof(double));
    mtype.insertMember("t", 0, H5::PredType::NATIVE_DOUBLE);
    for (size_t i=0;i<n;++i)
    {
        H5::CompType statei = H5::CompType(NB_OF_STATES_PER_BODY*sizeof(double));
        statei.insertMember("X", XIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Y", YIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Z", ZIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("U", UIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("V", VIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("W", WIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("P", PIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Q", QIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("R", RIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        H5::CompType quaternionType = H5::CompType(4*sizeof(double));
        quaternionType.insertMember("Qr", 0*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        quaternionType.insertMember("Qi", 1*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        quaternionType.insertMember("Qj", 2*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        quaternionType.insertMember("Qk", 3*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Quat",QRIDX(0)*sizeof(double), quaternionType);
        mtype.insertMember(v.at(i), offsetof(H5Res, v) + i*NB_OF_STATES_PER_BODY*sizeof(double), statei);
    }
    return mtype;
}

template <> void H5_Serialize<H5Res>::write(H5Res const * const data)
{
    const hsize_t dims[1] = {(hsize_t)1};
    double * dataV = new double[1+data->v.size()];
    hsize_t offset[1];
    hsize_t size[1];
    offset[0] = n;
    size[0] = ++n;
    dataset.extend(size);
    H5::DataSpace fspace = dataset.getSpace();
    fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
    dataV[0] = data->t;
    memcpy(dataV+1,&data->v.at(0),data->v.size()*sizeof(double));
    dataset.write(dataV, this->get_type(), this->get_space(), fspace);
    delete dataV;
}

H5::CompType h5_createWrenchType();
H5::CompType h5_createWrenchType()
{
    H5::CompType wrenchType = H5::CompType(6*sizeof(double));
    wrenchType.insertMember("Fx", 0*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    wrenchType.insertMember("Fy", 1*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    wrenchType.insertMember("Fz", 2*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    wrenchType.insertMember("Mx", 3*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    wrenchType.insertMember("My", 4*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    wrenchType.insertMember("Mz", 5*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    return wrenchType;
}

H5::CompType H5_InterfaceH5ResCreateId(const VectorOfStringModelForEachBody& v)
{
    size_t nModels = 0;
    for (auto it = v.begin() ; it != v.end() ; ++it)
    {
        nModels += it->second.size();
    }
    H5::CompType mtype = H5::CompType(sizeof(double)+nModels*6*sizeof(double));
    mtype.insertMember("t", 0, H5::PredType::NATIVE_DOUBLE);
    size_t iGlobal = 0;
    for (auto itBody = v.begin() ; itBody != v.end() ; ++itBody)
    {
        const auto body_name = itBody->first;
        H5::CompType bodyType = H5::CompType(itBody->second.size()*6*sizeof(double));
        size_t iModel = 0;
        for (auto itModel = itBody->second.begin() ; itModel != itBody->second.end() ; ++itModel)
        {
            bodyType.insertMember(*itModel, iModel*6*sizeof(double), h5_createWrenchType());
            ++iModel;
        }
        mtype.insertMember(body_name, offsetof(H5Res, v)+iGlobal*6*sizeof(double), bodyType);
        iGlobal += iModel;
    }
    return mtype;
}

typedef std::map<std::string, std::map< std::string,ssc::kinematics::Vector6d > > OuputtedForces;

class SimHdf5Observer::Impl
{
    public:
        Impl(const std::string& fileName, const std::string& baseName, const Sim& s) :
            fileName_(fileName),h5File_(H5::H5File(fileName, H5F_ACC_TRUNC)),
            sStates(h5File_, baseName.empty()?"states":baseName+"/states", H5_InterfaceH5ResCreateId(s.get_names_of_bodies())),
            sEfforts(h5File_, baseName.empty()?"efforts":baseName+"/efforts", H5_InterfaceH5ResCreateId(s.get_vector_of_string_model_for_each_body()))
        {
        }

        void observe_states(const double t, const Sim& s);
        void observe_efforts(const double t, const Sim& s);
        std::string fileName_;
        H5::H5File h5File_;
        H5_Serialize<H5Res> sStates;
        H5_Serialize<H5Res> sEfforts;
};

SimHdf5Observer::SimHdf5Observer(const std::string& fileName, const Sim& s) : pimpl(new Impl(fileName, "simu01", s))
{
}

SimHdf5Observer::SimHdf5Observer(const std::string& fileName, const std::string& baseName, const Sim& s) : pimpl(new Impl(fileName, baseName, s))
{
}

void SimHdf5Observer::Impl::observe_states(const double t, const Sim& s)
{
    const H5Res res(t, s.state);
    sStates << res;
}

void SimHdf5Observer::Impl::observe_efforts(const double t, const Sim& s)
{
    const H5Res res(t, s.get_forces_as_a_vector_of_doubles());
    sEfforts << res;
}

void SimHdf5Observer::observe(const Sim& sys, const double t)
{
    pimpl->observe_states(t, sys);
    pimpl->observe_efforts(t, sys);
}

void SimHdf5Observer::observe_states(const Sim& sys, const double t)
{
    pimpl->observe_states(t, sys);
}

void SimHdf5Observer::observe_efforts(const Sim& sys, const double t)
{
    pimpl->observe_efforts(t, sys);
}

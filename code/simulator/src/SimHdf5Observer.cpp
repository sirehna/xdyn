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
#include "Res.hpp"
#include "h5_interface.hpp"

H5::CompType H5_InterfaceResCreateId(const std::vector<std::string>& v);
H5::CompType H5_InterfaceResCreateId(const std::size_t& n);
H5::CompType H5_InterfaceResCreateId(const Sim& s);

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

H5::CompType H5_InterfaceResCreateId(const std::vector<std::string>& v)
{
    const size_t n = v.size();
    H5::CompType mtype = H5::CompType(sizeof(double)+n*NB_OF_STATES_PER_BODY*sizeof(double));
    mtype.insertMember("t", 0, H5::PredType::NATIVE_DOUBLE);
    for (size_t i=0;i<n;++i)
    {
        const std::string str = v.at(i);
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
        mtype.insertMember("S"+str, offsetof(Res, x) + i*NB_OF_STATES_PER_BODY*sizeof(double), statei);
    }
    return mtype;
}

H5::CompType H5_InterfaceResCreateId(const std::size_t& n)
{
    std::vector<std::string> v(n);
    for (size_t i=0;i<n;++i)
    {
        v.at(i) = "body" + t_to_string(i);
    }
    return H5_InterfaceResCreateId(v);
}

H5::CompType H5_InterfaceResCreateId(const Sim& s)
{
    return H5_InterfaceResCreateId(s.get_names_of_bodies());
}

template <> void H5_Serialize<Res>::write(Res const * const data)
{
    const hsize_t dims[1] = {(hsize_t)1};
    double * dataV = new double[1+data->x.size()];
    hsize_t offset[1];
    hsize_t size[1];
    offset[0] = n;
    size[0] = ++n;
    dataset.extend(size);
    H5::DataSpace fspace = dataset.getSpace();
    fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
    dataV[0] = data->t;
    memcpy(dataV+1,&data->x.at(0),data->x.size()*sizeof(double));
    dataset.write(dataV, this->get_type(), this->get_space(), fspace);
    delete dataV;
}

class SimHdf5Observer::Impl
{
    public:
        Impl(const std::string& fileName, const std::string& baseName, const Sim& s) :
            sRes(fileName, baseName+"/states", H5_InterfaceResCreateId(s))
        {
        }
        void observe_states(const double t, const Sim& s);
        void observe_states(const Res& res);
        H5_Serialize<Res> sRes;
};

SimHdf5Observer::SimHdf5Observer(const std::string& fileName, const std::string& baseName, const Sim& s) : pimpl(new Impl(fileName, baseName, s))
{
}

void SimHdf5Observer::Impl::observe_states(const double t, const Sim& s)
{
    const Res res(t, s.state);
    sRes << res;
}

void SimHdf5Observer::Impl::observe_states(const Res& res)
{
    sRes << res;
}

void SimHdf5Observer::observe(const Res& res)
{
    pimpl->observe_states(res);
}

void SimHdf5Observer::observe(const Sim& sys, const double t)
{
    pimpl->observe_states(t, sys);
}

#include "SimHdf5ObserverBuilder.hpp"

#define NB_OF_SCALAR_FOR_EULER_ANGLES 3
#define NB_OF_SCALAR_STATES (NB_OF_STATES_PER_BODY+NB_OF_SCALAR_FOR_EULER_ANGLES)

H5::CompType H5_CreateIdQuaternion()
{
    H5::CompType quaternionType = H5::CompType(4*sizeof(double));
    quaternionType.insertMember("Qr", 0*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    quaternionType.insertMember("Qi", 1*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    quaternionType.insertMember("Qj", 2*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    quaternionType.insertMember("Qk", 3*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    return quaternionType;
}

H5::CompType H5_CreateIdEulerAngle()
{
    H5::CompType eulerType = H5::CompType(3*sizeof(double));
    eulerType.insertMember("Phi", 0*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    eulerType.insertMember("Theta", 1*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    eulerType.insertMember("Psi", 2*sizeof(double), H5::PredType::NATIVE_DOUBLE);
    return eulerType;
}

H5::CompType H5_CreateIdStates(const VectorOfStringModelForEachBody& v)
{
    const size_t n = v.size();
    H5::CompType mtype = H5::CompType((1+n*NB_OF_SCALAR_STATES)*sizeof(double));
    mtype.insertMember("t", 0, H5::PredType::NATIVE_DOUBLE);
    for (size_t i=0;i<n;++i)
    {
        H5::CompType statei = H5::CompType(NB_OF_SCALAR_STATES*sizeof(double));
        statei.insertMember("X", XIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Y", YIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Z", ZIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("U", UIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("V", VIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("W", WIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("P", PIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Q", QIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("R", RIDX(0)*sizeof(double), H5::PredType::NATIVE_DOUBLE);
        statei.insertMember("Quat",QRIDX(0)*sizeof(double), H5_CreateIdQuaternion());
        statei.insertMember("Euler",NB_OF_STATES_PER_BODY*sizeof(double), H5_CreateIdEulerAngle());
        mtype.insertMember(v.at(i).first, sizeof(double) + i*NB_OF_SCALAR_STATES*sizeof(double), statei);
    }
    return mtype;
}

H5::CompType H5_CreateIdWrenchType()
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

H5::CompType H5_CreateIdEfforts(const VectorOfStringModelForEachBody& v)
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
            bodyType.insertMember(*itModel, iModel*6*sizeof(double), H5_CreateIdWrenchType());
            ++iModel;
        }
        mtype.insertMember(body_name, sizeof(double) + iGlobal*6*sizeof(double), bodyType);
        iGlobal += iModel;
    }
    return mtype;
}

template <> void H5_Serialize<H5Res>::write(H5Res const * const data)
{
    const hsize_t dims[1] = {(hsize_t)1};
    const hsize_t offset[1] = {n};
    const hsize_t size[1] = {++n};
    dataset.extend(size);
    H5::DataSpace fspace = dataset.getSpace();
    fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
    dataset.write(data->v.data(), this->get_type(), this->get_space(), fspace);
}

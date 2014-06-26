/*
 * StateDerivativesModule.cpp
 *
 *  Created on: May 29, 2014
 *      Author: cady
 */

#include "StateDerivativesModule.hpp"
#include "Wrench.hpp"
#include "DataSource.hpp"
#include "pairwise_sum.hpp"
#include "coriolis_and_centripetal.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"
#include <Eigen/Geometry> // For Eigen::Quaternion
#include "tr1_macros.hpp"
#include TR1INC(memory)

StateDerivativesModule::StateDerivativesModule(DataSource* const data_source, const std::string& body, const std::vector<std::string>& forces_) : SimulatorModuleInterface(data_source, "sum of forces", body), forces(forces_)
{

}

StateDerivativesModule::StateDerivativesModule(const StateDerivativesModule& rhs, DataSource* const data_source) : SimulatorModuleInterface(rhs, data_source), forces(rhs.forces)
{

}

DataSourceModule* StateDerivativesModule::clone() const
{
    return new StateDerivativesModule(*this);
}

DataSourceModule* StateDerivativesModule::clone(DataSource* const data_source) const
{
    return new StateDerivativesModule(*this, data_source);
}

void StateDerivativesModule::update() const
{
    Wrench sum;
    const Matrix6x6 M = ds->get<Matrix6x6>(customize("total inertia"));
    const Matrix6x6 Mrb = ds->get<Matrix6x6>(customize("solid body inertia"));
    const Matrix6x6 Minv = ds->get<Matrix6x6>(customize("inverse of the total inertia"));
    const Point P = ds->get<Point>(customize("G"));
    const TR1(shared_ptr)<Kinematics> k = ds->get<TR1(shared_ptr)<Kinematics> >("kinematics");
    const Eigen::Vector3d uvw_in_body_frame(ds->get<double>(customize("u")),
                                            ds->get<double>(customize("v")),
                                            ds->get<double>(customize("w")));
    const Eigen::Vector3d pqr(ds->get<double>(customize("p")),
                              ds->get<double>(customize("q")),
                              ds->get<double>(customize("r")));
    std::vector<Wrench> f(1, (-1.)*coriolis_and_centripetal(P,&Mrb,uvw_in_body_frame, pqr));
    ds->set<Wrench>(customize("coriolis and centripetal forces"), f.front());
    if (not(ds->read_only()))
    {
        std::vector<std::string>::const_iterator it = forces.begin();
        for (;it != forces.end() ; ++it) f.push_back(ds->get<Wrench>(customize(*it)));
    }
    if (not(ds->read_only())) sum = sum::pairwise(f);
    ds->set(customize("sum of forces"), sum);
    Eigen::Matrix<double,6,1> dstates_dt = ds->read_only() ? Eigen::Matrix<double,6,1>::Constant(0) : Eigen::Matrix<double,6,1>(Minv*sum.to_vector());
    const RotationMatrix R = ds->read_only() ? Eigen::Matrix<double, 3, 3>::Identity() : k->get(body, "NED").get_rot();
    const Eigen::Vector3d uvw_in_ned_frame(R*Point(body,uvw_in_body_frame).v);
    ds->set<double>(customize("dx/dt"), (double)uvw_in_ned_frame(0));
    ds->set<double>(customize("dy/dt"), (double)uvw_in_ned_frame(1));
    ds->set<double>(customize("dz/dt"), (double)uvw_in_ned_frame(2));
    ds->set<double>(customize("du/dt"), (double)dstates_dt(0));
    ds->set<double>(customize("dv/dt"), (double)dstates_dt(1));
    ds->set<double>(customize("dw/dt"), (double)dstates_dt(2));
    ds->set<double>(customize("dp/dt"), (double)dstates_dt(3));
    ds->set<double>(customize("dq/dt"), (double)dstates_dt(4));
    ds->set<double>(customize("dr/dt"), (double)dstates_dt(5));
    const Eigen::Quaternion<double> q1(ds->get<double>(customize("qr")),
                                       ds->get<double>(customize("qi")),
                                       ds->get<double>(customize("qj")),
                                       ds->get<double>(customize("qk"))
                        );
    const Eigen::Quaternion<double> q2(0,pqr(0),pqr(1),pqr(2));
    const Eigen::Quaternion<double> dq_dt(Eigen::Quaternion<double>(q1*q2));
    ds->set<double>(customize("dqr/dt"), 0.5*(double)dq_dt.w());
    ds->set<double>(customize("dqi/dt"), 0.5*(double)dq_dt.x());
    ds->set<double>(customize("dqj/dt"), 0.5*(double)dq_dt.y());
    ds->set<double>(customize("dqk/dt"), 0.5*(double)dq_dt.z());
}

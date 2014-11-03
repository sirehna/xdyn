/*
 * SurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "SurfaceForceModel.hpp"

SurfaceForceModel::SurfaceForceModel(const std::string& name_, const EnvironmentAndFrames& env_) : ForceModel(name_),
        env(env_),
        g_in_NED(ssc::kinematics::Point("NED", 0, 0, env.g))
{
}

SurfaceForceModel::~SurfaceForceModel()
{
}

ssc::kinematics::Wrench SurfaceForceModel::operator()(const Body& body, const double t) const
{
    ssc::kinematics::UnsafeWrench F(body.G);
    const double orientation_factor = body.intersector->mesh->orientation_factor;
    const auto e = end(body.intersector);

    for (auto that_facet = begin(body.intersector) ; that_facet != e ; ++that_facet)
    {
        const DF f = dF(that_facet, env, body, t);
        const double x = (f.C(0)-body.G.v(0));
        const double y = (f.C(1)-body.G.v(1));
        const double z = (f.C(2)-body.G.v(2));
        F.X() += orientation_factor*f.dF(0);
        F.Y() += orientation_factor*f.dF(1);
        F.Z() += orientation_factor*f.dF(2);
        F.K() += orientation_factor*(y*f.dF(2)-z*f.dF(1));
        F.M() += orientation_factor*(z*f.dF(0)-x*f.dF(2));
        F.N() += orientation_factor*(x*f.dF(1)-y*f.dF(0));
    }
    return F;
}

double SurfaceForceModel::potential_energy(const Body& body, const std::vector<double>& x) const
{
    return pe(body, x, env);
}

bool SurfaceForceModel::is_a_surface_force_model() const
{
    return true;
}

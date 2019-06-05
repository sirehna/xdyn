/*
 * SurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "BodyStates.hpp"
#include "SurfaceForceModel.hpp"

SurfaceForceModel::SurfaceForceModel(const std::string& name_, const std::string& body_name_, const EnvironmentAndFrames& env_) : ForceModel(name_, body_name_),
        env(env_),
        g_in_NED(ssc::kinematics::Point("NED", 0, 0, env.g)),
        zg_calculator(new ZGCalculator())
{
}

SurfaceForceModel::~SurfaceForceModel()
{
}

ssc::kinematics::Wrench SurfaceForceModel::operator()(const BodyStates& states, const double t) const
{
    zg_calculator->update_transform(env.k->get("NED", states.name));
    ssc::kinematics::UnsafeWrench F(states.G);
    const double orientation_factor = states.intersector->mesh->orientation_factor;

    const auto b = begin(states.intersector);
    const auto e = end(states.intersector);
    std::function<SurfaceForceModel::DF(const FacetIterator &,
                                        const size_t,
                                        const EnvironmentAndFrames &,
                                        const BodyStates &,
                                        const double)> dF_lambda =
        get_dF(b, e, env, states, t);
    
    size_t facet_index = 0;
    for (auto that_facet = begin(states.intersector) ; that_facet != e ; ++that_facet)
    {
        const DF f = dF_lambda(that_facet, facet_index, env, states, t);
        const double x = (f.C(0)-states.G.v(0));
        const double y = (f.C(1)-states.G.v(1));
        const double z = (f.C(2)-states.G.v(2));
        F.X() += orientation_factor*f.dF(0);
        F.Y() += orientation_factor*f.dF(1);
        F.Z() += orientation_factor*f.dF(2);
        F.K() += orientation_factor*(y*f.dF(2)-z*f.dF(1));
        F.M() += orientation_factor*(z*f.dF(0)-x*f.dF(2));
        F.N() += orientation_factor*(x*f.dF(1)-y*f.dF(0));
        ++facet_index;
    }
    return F;
}

double SurfaceForceModel::potential_energy(const BodyStates& states, const std::vector<double>& x) const
{
    return pe(states, x, env);
}

bool SurfaceForceModel::is_a_surface_force_model() const
{
    return true;
}

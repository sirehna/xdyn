/*
 * FroudeKrylovForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "SurfaceElevationInterface.hpp"

std::string FroudeKrylovForceModel::model_name() {return "non-linear Froude-Krylov";}

FroudeKrylovForceModel::FroudeKrylovForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(model_name(), body_name_, env_)
{
    if (env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << model_name() << "' needs a wave model, even if it's 'no waves'");
    }
}

ssc::kinematics::Wrench FroudeKrylovForceModel::operator()(const BodyStates &states, const double t) const
{
    zg_calculator->update_transform(env.k->get("NED", states.name));
    ssc::kinematics::UnsafeWrench F(states.G);
    const double orientation_factor = states.intersector->mesh->orientation_factor;
    const auto e = end(states.intersector);

    for (auto that_facet = begin(states.intersector); that_facet != e; ++that_facet)
    {
        const DF f = dF(that_facet, env, states, t);
        const double x = (f.C(0) - states.G.v(0));
        const double y = (f.C(1) - states.G.v(1));
        const double z = (f.C(2) - states.G.v(2));
        F.X() += orientation_factor * f.dF(0);
        F.Y() += orientation_factor * f.dF(1);
        F.Z() += orientation_factor * f.dF(2);
        F.K() += orientation_factor * (y * f.dF(2) - z * f.dF(1));
        F.M() += orientation_factor * (z * f.dF(0) - x * f.dF(2));
        F.N() += orientation_factor * (x * f.dF(1) - y * f.dF(0));
    }
    return F;
}

SurfaceForceModel::DF FroudeKrylovForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const BodyStates& states, const double t) const
{
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const ssc::kinematics::Point C(states.M->get_frame(), that_facet->centre_of_gravity);
    double eta = 0;
    for (auto it = that_facet->vertex_index.begin() ; it != that_facet->vertex_index.end() ; ++it)
    {
        eta += states.intersector->all_absolute_wave_elevations.at(*it);
    }
    if (not(that_facet->vertex_index.empty())) eta /= (double)that_facet->vertex_index.size();
    const double pdyn = env.w->get_dynamic_pressure(env.rho, env.g, std::vector<ssc::kinematics::Point>{C}, env.k, std::vector<double>{eta}, t).at(0);
    return DF(-pdyn*dS,C.v);
}

double FroudeKrylovForceModel::pe(const BodyStates& , const std::vector<double>& , const EnvironmentAndFrames& ) const
{
    return 0;
}

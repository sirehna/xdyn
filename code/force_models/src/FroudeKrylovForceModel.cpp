/*
 * FroudeKrylovForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include "InternalErrorException.hpp"
#include <ssc/exception_handling.hpp>

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

    std::vector<EPoint> dS;
    std::vector<ssc::kinematics::Point> C;
    // Calculate facet area and centre of gravity
    for (auto that_facet = begin(states.intersector); that_facet != e; ++that_facet)
    {
        dS.push_back(EPoint(that_facet->area * that_facet->unit_normal));
        C.push_back(ssc::kinematics::Point(states.M->get_frame(), that_facet->centre_of_gravity));
    }
    // Compute average elevation for each facet
    std::vector<double> average_eta_per_facet;
    for (auto that_facet = begin(states.intersector); that_facet != e; ++that_facet)
    {
        double eta_facet = 0;
        for (auto it = that_facet->vertex_index.begin(); it != that_facet->vertex_index.end(); ++it)
        {
            eta_facet += states.intersector->all_absolute_wave_elevations.at(*it);
        }
        if (not(that_facet->vertex_index.empty()))
            eta_facet /= (double)that_facet->vertex_index.size();
        average_eta_per_facet.push_back(eta_facet);
    }

    const std::vector<double> pdyn = env.w->get_dynamic_pressure(env.rho, env.g, C, env.k, average_eta_per_facet, t);

    if (pdyn.size() != C.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
              "Error when getting dynamic pression in Froude-Krylov force model: the points vector and the dynamic pressure vector don't have the same size (size of P: "
                  << C.size() << ", size of pdyn: " << pdyn.size() << ")")
    }
    for (size_t i = 0; i < pdyn.size(); ++i)
    {
        const DF f = DF(-pdyn.at(i) * dS.at(i), C.at(i).v);
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

SurfaceForceModel::DF FroudeKrylovForceModel::dF(const FacetIterator& that_facet, const size_t that_facet_index, const EnvironmentAndFrames& env, const BodyStates& states, const double t) const
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

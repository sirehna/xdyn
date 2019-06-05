/*
 * FroudeKrylovForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "SurfaceElevationInterface.hpp"
#include <ssc/exception_handling.hpp>

std::string FroudeKrylovForceModel::model_name() {return "non-linear Froude-Krylov";}

FroudeKrylovForceModel::FroudeKrylovForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(model_name(), body_name_, env_)
{
    if (env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << model_name() << "' needs a wave model, even if it's 'no waves'");
    }
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

std::function<SurfaceForceModel::DF(const FacetIterator &, const size_t, const EnvironmentAndFrames &, const BodyStates &, const double)>
    FroudeKrylovForceModel::get_dF(const FacetIterator &begin_facet,
                                   const FacetIterator &end_facet,
                                   const EnvironmentAndFrames &env,
                                   const BodyStates &states,
                                   const double t) const
{
    // Compute dynamic pressure for all facets
    std::vector<ssc::kinematics::Point> C;
    std::vector<double> average_eta_per_facet;
    for (auto that_facet = begin_facet; that_facet != end_facet; ++that_facet)
    {
        C.push_back(ssc::kinematics::Point(states.M->get_frame(), that_facet->centre_of_gravity));
        // Compute average elevation for each facet
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

    return [pdyn](const FacetIterator &that_facet,
                  const size_t that_facet_index,
                  const EnvironmentAndFrames &env,
                  const BodyStates &states,
                  const double t)
    {
        // Calculate facet area and centre of gravity
        const EPoint dS = that_facet->area * that_facet->unit_normal;
        const ssc::kinematics::Point C(states.M->get_frame(), that_facet->centre_of_gravity);
        return SurfaceForceModel::DF(-pdyn.at(that_facet_index) * dS, C.v);
    };
}

double FroudeKrylovForceModel::pe(const BodyStates& , const std::vector<double>& , const EnvironmentAndFrames& ) const
{
    return 0;
}

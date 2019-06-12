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

std::function<SurfaceForceModel::DF(const FacetIterator &, const size_t, const EnvironmentAndFrames &, const BodyStates &, const double)>
    FroudeKrylovForceModel::get_dF(const FacetIterator &begin_facet,
                                   const FacetIterator &end_facet,
                                   const EnvironmentAndFrames &env,
                                   const BodyStates &states,
                                   const double t) const
{
    // Compute average elevation for each facet
    std::vector<double> average_eta_per_facet;
    for (auto that_facet = begin_facet; that_facet != end_facet; ++that_facet)
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

    ssc::kinematics::PointMatrix M(states.M->get_frame(), average_eta_per_facet.size());
    size_t that_facet_index = 0;
    for (auto that_facet = begin_facet; that_facet != end_facet; ++that_facet)
    {
        M.m(0, that_facet_index) = that_facet->centre_of_gravity.x();
        M.m(1, that_facet_index) = that_facet->centre_of_gravity.y();
        M.m(2, that_facet_index) = that_facet->centre_of_gravity.z();
        ++that_facet_index;
    }
    // Compute dynamic pressure for all facets
    std::vector<double> pdyn;
    try
    {
        pdyn = env.w->get_dynamic_pressure(env.rho, env.g, M, env.k, average_eta_per_facet, t);
    }
    catch (const ssc::exception_handling::Exception& e)
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the Froude-Krylov force model which uses the dynamic pressures calculated by a wave model. When querying the wave model for these dynamic pressures, the following problem occurred:\n" << e.get_message());
    }

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

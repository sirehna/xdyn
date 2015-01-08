/*
 * HydrostaticForceModel.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef FASTHYDROSTATICFORCEMODEL_HPP_
#define FASTHYDROSTATICFORCEMODEL_HPP_

#include "ImmersedSurfaceForceModel.hpp"

class FastHydrostaticForceModel : public ImmersedSurfaceForceModel
{
    public:
        FastHydrostaticForceModel(const EnvironmentAndFrames& env);
        DF dF(const FacetIterator& that_facet,
              const EnvironmentAndFrames& env,
              const Body& body,
              const double t
              ) const;

        static const std::string model_name;

    private:
        FastHydrostaticForceModel();
        virtual EPoint get_application_point(const FacetIterator& that_facet, const Body& body, const double zG) const;
        double pe(const Body& body, const std::vector<double>& x, const EnvironmentAndFrames& env) const;
};

#endif /* FASTHYDROSTATICFORCEMODEL_HPP_ */

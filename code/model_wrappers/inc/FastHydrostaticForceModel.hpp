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

    private:
        FastHydrostaticForceModel();
};

#endif /* FASTHYDROSTATICFORCEMODEL_HPP_ */

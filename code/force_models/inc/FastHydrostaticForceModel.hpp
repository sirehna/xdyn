/*
 * FastHydrostaticForceModel.hpp
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
        FastHydrostaticForceModel(const std::string& body_name, const EnvironmentAndFrames& env);
        DF dF(const FacetIterator& that_facet,
              const EnvironmentAndFrames& env,
              const BodyStates& states,
              const double t
              ) const;

        std::string get_name() const;
        static std::string model_name();
        double gz() const;

    protected:
        FastHydrostaticForceModel(const std::string& force_name, const std::string& body_name, const EnvironmentAndFrames& env);

    private:
        FastHydrostaticForceModel();
        void extra_observations(Observer& observer) const;
        virtual EPoint get_application_point(const FacetIterator& that_facet, const BodyStates& states, const double zG) const;
        double pe(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const;
};

#endif /* FASTHYDROSTATICFORCEMODEL_HPP_ */

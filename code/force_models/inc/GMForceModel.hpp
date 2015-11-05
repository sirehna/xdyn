/*
 * GMForceModel.hpp
 *
 *  Created on: April 10, 2015
 *      Author: cady
 */

#ifndef GMFORCEMODEL_HPP_
#define GMFORCEMODEL_HPP_

#include "ImmersedSurfaceForceModel.hpp"
#include <ssc/kinematics.hpp>

#include "EnvironmentAndFrames.hpp"

class Body;

/** \brief Provides an interface to QuadraticDampingForceModel & LinearDampingForceModel
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 */
class GMForceModel : public ImmersedSurfaceForceModel
{
    public:
        struct Yaml : public YamlModel
        {
            Yaml();
            std::string name_of_hydrostatic_force_model;
            double roll_step;
            ForceParser try_to_parse;
        };
        GMForceModel(const Yaml& data, const std::string& body_name, const EnvironmentAndFrames& env);
        DF dF(const FacetIterator& that_facet,
                      const EnvironmentAndFrames& env,
                      const BodyStates& states,
                      const double t
                      ) const;
        static Yaml parse(const std::string& yaml);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        void extra_observations(Observer& ) const;
        static const std::string model_name;

    private:
        GMForceModel();
        double get_gz_for_shifted_states(const BodyStates& states, const double t) const;
        BodyStates get_shifted_states(const BodyStates& states,
                const double t) const;
        double pe(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const;

        ForcePtr underlying_hs_force_model;
        double dphi;
        EnvironmentAndFrames env;
        TR1(shared_ptr)<double> GM;
};

#endif /* GMFORCEMODEL_HPP_ */

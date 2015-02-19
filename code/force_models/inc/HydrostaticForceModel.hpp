/*
 * HydrostaticForceModel.hpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICFORCEMODEL_HPP_
#define HYDROSTATICFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include <Eigen/Dense>
#include <ssc/kinematics.hpp>

#include "EnvironmentAndFrames.hpp"

class Body;

/** \brief Provides an interface to QuadraticDampingForceModel & LinearDampingForceModel
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 */
class HydrostaticForceModel : public ForceModel
{
    public:
        HydrostaticForceModel(const std::string& body_name, const EnvironmentAndFrames& env_);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        const static std::string model_name;
        bool is_a_surface_force_model() const;

    private:
        HydrostaticForceModel();
        EnvironmentAndFrames env;
};

#endif /* HYDROSTATICFORCEMODEL_HPP_ */

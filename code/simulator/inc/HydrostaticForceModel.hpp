/*
 * HydrostaticForceModel.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICFORCEMODEL_HPP_
#define HYDROSTATICFORCEMODEL_HPP_

#include "ForceModel.hpp"
#include "Kinematics.hpp"

class WaveModelInterface;
typedef TR1(shared_ptr)<WaveModelInterface> WaveModelPtr;

class Body;
class EnvironmentAndFrames;

class HydrostaticForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            Input(const EnvironmentAndFrames& env);
            double rho;
            double g;
            WaveModelPtr w;
            KinematicsPtr k;
        };

        HydrostaticForceModel(const Input& input);
        Wrench operator()(const Body& body) const;

    private:
        HydrostaticForceModel();
        double rho;
        double g;
        WaveModelPtr w;
        KinematicsPtr k;
};

#endif /* HYDROSTATICFORCEMODEL_HPP_ */

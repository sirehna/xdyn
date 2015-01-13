/*
 * RadiationDampingForceModel.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cec
 */

#ifndef RadiationDampingForceModel_HPP_
#define RadiationDampingForceModel_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "ForceModel.hpp"
#include "YamlRadiationDamping.hpp"

class HDBParser;

class EnvironmentAndFrames;

class RadiationDampingForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input() : hdb(), yaml(){}
            TR1(shared_ptr)<HDBParser> hdb;
            YamlRadiationDamping yaml;
        };
        RadiationDampingForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        static Input parse(const std::string& yaml, const bool parse_hdb=true);
        static const std::string model_name;

    private:
        RadiationDampingForceModel();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* RadiationDampingForceModel_HPP_ */

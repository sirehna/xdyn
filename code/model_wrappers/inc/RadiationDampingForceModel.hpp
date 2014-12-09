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

class HDBParser;
struct YamlRadiationDamping;

class RadiationDampingForceModel : public ForceModel
{
    public:
        RadiationDampingForceModel(const TR1(shared_ptr)<HDBParser>& hdb, const YamlRadiationDamping& yaml);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        RadiationDampingForceModel();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* RadiationDampingForceModel_HPP_ */

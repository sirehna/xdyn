/*
 * RadiationDampingForceModel.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cec
 */

#ifndef RadiationDampingForceModel_HPP_
#define RadiationDampingForceModel_HPP_

#include "ForceModel.hpp"

class HDBParser;

class RadiationDampingForceModel : public ForceModel
{
    public:
        RadiationDampingForceModel(const HDBParser& hdb);

    private:
        RadiationDampingForceModel();
};

#endif /* RadiationDampingForceModel_HPP_ */

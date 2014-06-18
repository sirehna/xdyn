/*
 * GravityForceModel.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef GRAVITYFORCEMODEL_HPP_
#define GRAVITYFORCEMODEL_HPP_

#include "ForceModel.hpp"

class Body;
class EnvironmentAndFrames;

class GravityForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            Input(const EnvironmentAndFrames& env);
            double g;
        };

        GravityForceModel(const Input& input);
        Wrench operator()(const Body& body) const;

    private:
        GravityForceModel();
        double g;
};


#endif /* GRAVITYFORCEMODEL_HPP_ */

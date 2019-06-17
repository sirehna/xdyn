/*
 * GRPCForceModel.hpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_GRPCFORCEMODEL_HPP_
#define GRPC_INC_GRPCFORCEMODEL_HPP_


#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"
#include <ssc/kinematics.hpp>

class GRPCForceModel : public ForceModel
{
    public:
        struct Input
        {
            Input();
            std::string url;
        };

        GRPCForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        GRPCForceModel(); // Disabled
        EnvironmentAndFrames env;
};


#endif /* GRPC_INC_GRPCFORCEMODEL_HPP_ */

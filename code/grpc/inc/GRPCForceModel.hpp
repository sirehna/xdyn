/*
 * GRPCForceModel.hpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_GRPCFORCEMODEL_HPP_
#define GRPC_INC_GRPCFORCEMODEL_HPP_


#include "EnvironmentAndFrames.hpp"
#include "ControllableForceModel.hpp"
#include <ssc/kinematics.hpp>

class GRPCForceModel : public ControllableForceModel
{
    public:
        struct Input
        {
            Input();
            std::string url;
            std::string name;
            std::string yaml;
        };

        GRPCForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, const std::map<std::string,double>& commands) const;
        static Input parse(const std::string& yaml);
        static std::string model_name();

    private:
        GRPCForceModel(); // Disabled
        EnvironmentAndFrames env;
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};


#endif /* GRPC_INC_GRPCFORCEMODEL_HPP_ */

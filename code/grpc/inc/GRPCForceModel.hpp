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
#include "GRPCTypes.hpp"


class GRPCForceModel : public ControllableForceModel
{
    public:
        GRPCForceModel(const Input& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, const std::map<std::string,double>& commands) const;
        static Input parse(const std::string& yaml);
        static std::string model_name();
        double get_Tmax() const;

    private:
        void extra_observations(Observer& observer) const;
        GRPCForceModel(); // Disabled
        EnvironmentAndFrames env;
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};


#endif /* GRPC_INC_GRPCFORCEMODEL_HPP_ */

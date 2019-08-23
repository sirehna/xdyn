/*
 * ToGRPC.hpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_TOGRPC_HPP_
#define GRPC_INC_TOGRPC_HPP_

#include <grpcpp/grpcpp.h>
#include "force.pb.h"
#include "force.grpc.pb.h"

#include "GRPCTypes.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "EnvironmentAndFrames.hpp"

#include "GRPCForceModel.hpp"

class ToGRPC
{
    public:
        ToGRPC(const GRPCForceModel::Input& input_);
        RequiredWaveInformationRequest from_required_wave_information(const double t, const double x, const double y, const double z) const;
        SpectrumResponse* from_discrete_directional_wave_spectra(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra) const;
        WaveInformation* from_wave_information(const WaveRequest& wave_request, const double t, const EnvironmentAndFrames& env) const;
        States* from_state(const BodyStates& state, const double max_history_length, const EnvironmentAndFrames& env) const;
        ForceRequest from_force_request(States* states, const std::map<std::string, double >& commands, WaveInformation* wave_information, const std::string& instance_name) const;
        SetForceParameterRequest from_yaml(const std::string& yaml, const std::string body_name, const std::string& instance_name) const;

    private:
        void copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination) const;
        void copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination) const;
        GRPCForceModel::Input input;
        ToGRPC(); // Disabled
};



#endif /* GRPC_INC_TOGRPC_HPP_ */

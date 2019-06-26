/*
 * FromGRPC.hpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */

#ifndef GRPC_INC_FROMGRPC_HPP_
#define GRPC_INC_FROMGRPC_HPP_

#include <ssc/kinematics.hpp>
#include <grpcpp/grpcpp.h>
#include "force.pb.h"
#include "force.grpc.pb.h"
#include "GRPCTypes.hpp"

class FromGRPC
{
    public:
        WaveRequest to_wave_request(const RequiredWaveInformationResponse& response) const;
        ssc::kinematics::Vector6d to_force(const ForceResponse& response) const;
};


#endif /* GRPC_INC_FROMGRPC_HPP_ */

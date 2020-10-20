/*
 * XdynForCSgrpc.hpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_XDYNFORCSGRPC_HPP_
#define EXECUTABLES_INC_XDYNFORCSGRPC_HPP_

#include <grpcpp/grpcpp.h>
#include "cosimulation.grpc.pb.h"
#include "cosimulation.pb.h"
#include "XdynForCS.hpp"

/*
 *
 */
class CosimulationServiceImpl final : public Cosimulation::Service {
    public:
        explicit CosimulationServiceImpl(const TR1(shared_ptr)<SimServer>& simserver);
        grpc::Status step(grpc::ServerContext* context, const CosimulationRequest* request, CosimulationResponse* response) override;

    private:
        TR1(shared_ptr)<SimServer> simserver;
};

#endif /* EXECUTABLES_INC_XDYNFORCSGRPC_HPP_ */

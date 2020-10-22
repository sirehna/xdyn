/*
 * ModelExchangeServiceImpl.hpp
 *
 *  Created on: Oct 22, 2020
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_
#define EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_

#include <grpcpp/grpcpp.h>
#include "model_exchange.grpc.pb.h"
#include "model_exchange.pb.h"
#include "XdynForME.hpp"

/*
 *
 */
class ModelExchangeServiceImpl final : public ModelExchange::Service {
    public:
        explicit ModelExchangeServiceImpl(const XdynForME& xdyn);
        grpc::Status dx_dt_quaternion(grpc::ServerContext* context, const ModelExchangeRequestQuaternion* request, ModelExchangeResponse* response) override;
        grpc::Status dx_dt_euler_321(grpc::ServerContext* context, const ModelExchangeRequestEuler* request, ModelExchangeResponse* response) override;

    private:
        XdynForME xdyn;
};

#endif /* EXECUTABLES_INC_MODELEXCHANGESERVICEIMPL_HPP_ */

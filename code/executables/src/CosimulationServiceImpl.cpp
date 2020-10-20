/*
 * XdynForCSgrpc.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */
#include <sstream>
#include "CosimulationServiceImpl.hpp"
#include "YamlSimServerInputs.hpp"

CosimulationServiceImpl::CosimulationServiceImpl(const TR1(shared_ptr)<SimServer>& simserver_):
simserver(simserver_)
{}

#define SIZE size()
#define PASTER(x,y) x ## _ ## y
#define EVALUATOR(x,y)  PASTER(x,y)
#define STATE_SIZE(state) EVALUATOR(state, SIZE)
#define CHECK_SIZE(state) if (states.STATE_SIZE(state) != states.t_size())\
{\
    const size_t n1 = states.STATE_SIZE(state);\
    const size_t n2 = states.t_size();\
    msg << "State '" << #state << "' has size " << n1 << ", whereas 't' has size " << n2 << ": this is a problem in the client code (caller of xdyn's gRPC server), not a problem with xdyn. Please ensure that '" << #state << "' and 't' have the same size in CosimulationRequest's 'States' type." << std::endl;\
}

grpc::Status check_states_size(const CosimulationRequest* request);
grpc::Status check_states_size(const CosimulationRequest* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const CosimulationStates states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(qr);
        CHECK_SIZE(qi);
        CHECK_SIZE(qj);
        CHECK_SIZE(qk);
    }
    if (msg.str().empty())
    {
        return grpc::Status::OK;
    }
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, msg.str());
}

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequest* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* , const CosimulationRequest* request)
{
    YamlSimServerInputs server_inputs;
    if (request)
    {

        server_inputs.Dt = request->dt();
        std::map<std::string, double> commands(request->commands().begin(),
                request->commands().end());
        server_inputs.commands = commands;
        const size_t n = request->states().t_size();
        server_inputs.states.resize(n);
        for (size_t i = 0 ; i < n ; ++i)
        {
            server_inputs.states[i].x = request->states().x(i);
            server_inputs.states[i].y = request->states().y(i);
            server_inputs.states[i].z = request->states().z(i);
            server_inputs.states[i].u = request->states().u(i);
            server_inputs.states[i].v = request->states().v(i);
            server_inputs.states[i].w = request->states().w(i);
            server_inputs.states[i].p = request->states().p(i);
            server_inputs.states[i].q = request->states().q(i);
            server_inputs.states[i].r = request->states().r(i);
            server_inputs.states[i].qr = request->states().qr(i);
            server_inputs.states[i].qi = request->states().qi(i);
            server_inputs.states[i].qj = request->states().qj(i);
            server_inputs.states[i].qk = request->states().qk(i);
        }
    }
    return server_inputs;
}

grpc::Status to_grpc(grpc::ServerContext* context, const std::vector<YamlState>& res, CosimulationResponse* response);
grpc::Status to_grpc(grpc::ServerContext* , const std::vector<YamlState>& res, CosimulationResponse* response)
{
    if (res.empty())
    {
        return grpc::Status(grpc::StatusCode::INTERNAL, "We got a zero-size res vector from SimServer::play_one_step. This should never happen and is a big in xdyn's gRPC implementation. Please contact xdyn's support team!");
    }
    const YamlState state = res.back();
    response->set_t(state.t);
    response->set_x(state.x);
    response->set_y(state.y);
    response->set_z(state.z);
    response->set_u(state.u);
    response->set_v(state.v);
    response->set_w(state.w);
    response->set_p(state.p);
    response->set_q(state.q);
    response->set_r(state.r);
    response->set_qr(state.qr);
    response->set_qi(state.qi);
    response->set_qj(state.qj);
    response->set_qk(state.qk);
    response->set_phi(state.phi);
    response->set_theta(state.theta);
    response->set_psi(state.psi);
    return grpc::Status::OK;
}

grpc::Status CosimulationServiceImpl::step(
        grpc::ServerContext* context,
        const CosimulationRequest* request,
        CosimulationResponse* response)
{
    const grpc::Status precond = check_states_size(request);
    if (not precond.ok())
    {
        return precond;
    }
    const YamlSimServerInputs inputs = from_grpc(context, request);
    const std::vector<YamlState> output = simserver->play_one_step(inputs);
    const grpc::Status postcond = to_grpc(context, output, response);
    return postcond;
}

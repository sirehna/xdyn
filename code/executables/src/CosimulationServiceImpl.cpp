/*
 * XdynForCSgrpc.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */
#include <sstream>
#include "BodyStates.hpp"
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

grpc::Status check_states_size(const CosimulationRequestEuler* request);
grpc::Status check_states_size(const CosimulationRequestEuler* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const CosimulationStatesEuler states = request->states();
        CHECK_SIZE(x);
        CHECK_SIZE(y);
        CHECK_SIZE(z);
        CHECK_SIZE(u);
        CHECK_SIZE(v);
        CHECK_SIZE(w);
        CHECK_SIZE(p);
        CHECK_SIZE(q);
        CHECK_SIZE(r);
        CHECK_SIZE(phi);
        CHECK_SIZE(theta);
        CHECK_SIZE(psi);
    }
    if (msg.str().empty())
    {
        return grpc::Status::OK;
    }
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, msg.str());
}

grpc::Status check_states_size(const CosimulationRequestQuaternion* request);
grpc::Status check_states_size(const CosimulationRequestQuaternion* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const CosimulationStatesQuaternion states = request->states();
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

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestEuler* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* , const CosimulationRequestEuler* request)
{
    YamlSimServerInputs server_inputs;
    ssc::kinematics::EulerAngles angles;
    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention = {"z", "y'", "x''"};
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
            angles.phi = request->states().phi(i);
            angles.theta = request->states().theta(i);
            angles.psi = request->states().psi(i);
            const auto quaternion = BodyStates::convert(angles, rot);
            server_inputs.states[i].qr = std::get<0>(quaternion);
            server_inputs.states[i].qi = std::get<1>(quaternion);
            server_inputs.states[i].qj = std::get<2>(quaternion);
            server_inputs.states[i].qk = std::get<3>(quaternion);
        }
    }
    return server_inputs;
}

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const CosimulationRequestQuaternion* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* , const CosimulationRequestQuaternion* request)
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
    CosimulationState* last_state = new CosimulationState();
    CosimulationStates* all_states = new CosimulationStates();
    for (const auto s:res)
    {
        all_states->add_x(s.x);
        all_states->add_y(s.y);
        all_states->add_z(s.z);
        all_states->add_u(s.u);
        all_states->add_v(s.v);
        all_states->add_w(s.w);
        all_states->add_p(s.p);
        all_states->add_q(s.q);
        all_states->add_r(s.r);
        all_states->add_qr(s.qr);
        all_states->add_qi(s.qi);
        all_states->add_qj(s.qj);
        all_states->add_qk(s.qk);
        all_states->add_phi(s.phi);
        all_states->add_theta(s.theta);
        all_states->add_psi(s.psi);
        all_states->add_t(s.t);
    }

    last_state->set_t(state.t);
    last_state->set_x(state.x);
    last_state->set_y(state.y);
    last_state->set_z(state.z);
    last_state->set_u(state.u);
    last_state->set_v(state.v);
    last_state->set_w(state.w);
    last_state->set_p(state.p);
    last_state->set_q(state.q);
    last_state->set_r(state.r);
    last_state->set_qr(state.qr);
    last_state->set_qi(state.qi);
    last_state->set_qj(state.qj);
    last_state->set_qk(state.qk);
    last_state->set_phi(state.phi);
    last_state->set_theta(state.theta);
    last_state->set_psi(state.psi);
    response->set_allocated_last_state(last_state);
    response->set_allocated_all_states(all_states);
    return grpc::Status::OK;
}

grpc::Status CosimulationServiceImpl::step_euler_321(
        grpc::ServerContext* context,
        const CosimulationRequestEuler* request,
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

grpc::Status CosimulationServiceImpl::step_quaternion(
        grpc::ServerContext* context,
        const CosimulationRequestQuaternion* request,
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

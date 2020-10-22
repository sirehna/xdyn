/*
 * XdynForCSgrpc.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: cady
 */
#include <cmath>
#include <sstream>
#include <tuple>
#include "BodyStates.hpp"
#include "ModelExchangeServiceImpl.hpp"
#include "SimServerInputs.hpp"
#include "YamlSimServerInputs.hpp"

ModelExchangeServiceImpl::ModelExchangeServiceImpl(const XdynForME& xdyn_):
xdyn(xdyn_)
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

grpc::Status check_states_size(const ModelExchangeRequestEuler* request);
grpc::Status check_states_size(const ModelExchangeRequestEuler* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const ModelExchangeStatesEuler states = request->states();
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

grpc::Status check_states_size(const ModelExchangeRequestQuaternion* request);
grpc::Status check_states_size(const ModelExchangeRequestQuaternion* request)
{
    std::stringstream msg;
    if (!request)
    {
        msg << "'request' is a NULL pointer in " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": this is an implementation error in xdyn. You should contact xdyn's support team." << std::endl;
    }
    else
    {
        const ModelExchangeStatesQuaternion states = request->states();
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

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestEuler* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* , const ModelExchangeRequestEuler* request)
{
    YamlSimServerInputs server_inputs;
    ssc::kinematics::EulerAngles angles;
    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention = {"z", "y'", "x''"};
    if (request)
    {

        server_inputs.Dt = 0; // Not used by XdynForME anyway
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

YamlSimServerInputs from_grpc(grpc::ServerContext* context, const ModelExchangeRequestQuaternion* request);
YamlSimServerInputs from_grpc(grpc::ServerContext* , const ModelExchangeRequestQuaternion* request)
{
    YamlSimServerInputs server_inputs;
    if (request)
    {

        server_inputs.Dt = 0; // Not used by XdynForME anyway
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

std::tuple<double, double, double> get_euler_derivative(const StateType& state);
std::tuple<double, double, double> get_euler_derivative(const StateType& state)
{
    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention = {"z", "y'", "x''"};
    const ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(state[9],state[10],state[11],state[12]).matrix();
    const ssc::kinematics::EulerAngles angles = BodyStates::convert(R, rot);
    const double cos_phi = std::cos(angles.phi);
    const double sin_phi = std::sin(angles.phi);
    const double cos_theta = std::cos(angles.theta);
    const double tan_theta = std::tan(angles.theta);
    const double p = state[6];
    const double q = state[7];
    const double r = state[8];
    const double dphi_dt = p + sin_phi*tan_theta *q + cos_phi*tan_theta*r;
    const double dtheta_dt = cos_phi*q - sin_phi*r;
    const double dpsi_dt = sin_phi/cos_theta*q + cos_phi/cos_theta*r;
    return std::tuple<double, double, double>(dphi_dt, dtheta_dt, dpsi_dt);
}

grpc::Status to_grpc(grpc::ServerContext* context, const StateType& res, ModelExchangeResponse* response, const SimServerInputs& inputs);
grpc::Status to_grpc(grpc::ServerContext* , const StateType& res, ModelExchangeResponse* response, const SimServerInputs& inputs)
{
    if (res.size() != 13)
    {
        return grpc::Status(grpc::StatusCode::INTERNAL, "We didn't get 13 states back from XdynForME::calculate_dx_dt. This should never happen and is a bug in xdyn's gRPC implementation. Please contact xdyn's support team!");
    }
    ModelExchangeStateDerivatives* d_dt = new ModelExchangeStateDerivatives();
    d_dt->set_x(res[0]);
    d_dt->set_y(res[1]);
    d_dt->set_z(res[2]);
    d_dt->set_u(res[3]);
    d_dt->set_v(res[4]);
    d_dt->set_w(res[5]);
    d_dt->set_p(res[6]);
    d_dt->set_q(res[7]);
    d_dt->set_r(res[8]);
    d_dt->set_qr(res[9]);
    d_dt->set_qi(res[10]);
    d_dt->set_qj(res[11]);
    d_dt->set_qk(res[12]);
    const std::tuple<double, double, double> deuler_dt = get_euler_derivative(inputs.state_at_t);
    d_dt->set_phi(std::get<0>(deuler_dt));
    d_dt->set_theta(std::get<1>(deuler_dt));
    d_dt->set_psi(std::get<2>(deuler_dt));
    d_dt->set_t(inputs.t);
    response->set_allocated_d_dt(d_dt);
    return grpc::Status::OK;
}

grpc::Status ModelExchangeServiceImpl::dx_dt_euler_321(
        grpc::ServerContext* context,
        const ModelExchangeRequestEuler* request,
        ModelExchangeResponse* response)
{
    const grpc::Status precond = check_states_size(request);
    if (not precond.ok())
    {
        return precond;
    }
    const SimServerInputs inputs(from_grpc(context, request), xdyn.get_Tmax());
    const StateType output = xdyn.calculate_dx_dt(inputs);
    const grpc::Status postcond = to_grpc(context, output, response, inputs);
    return postcond;
}

grpc::Status ModelExchangeServiceImpl::dx_dt_quaternion(
        grpc::ServerContext* context,
        const ModelExchangeRequestQuaternion* request,
        ModelExchangeResponse* response)
{
    const grpc::Status precond = check_states_size(request);
    if (not precond.ok())
    {
        return precond;
    }
    const YamlSimServerInputs inputs = from_grpc(context, request);
    if (inputs.states.empty())
    {
        return grpc::Status(grpc::StatusCode::INTERNAL, "We didn't get any states as input (inputs.states is empty): we need at least one to set the initial conditions. This error was detected in ModelExchangeServiceImpl::dx_dt_quaternion");
    }
    const double t = inputs.states.back().t;
    const StateType output = xdyn.calculate_dx_dt(SimServerInputs(inputs, xdyn.get_Tmax()));
    const grpc::Status postcond = to_grpc(context, output, response, t);
    return postcond;
}

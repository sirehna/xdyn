/*
 * GRPCForceModel.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#include <memory> // std::make_shared
#include <vector>
#include <grpcpp/grpcpp.h>
#include "force.pb.h"
#include "force.grpc.pb.h"

#include "yaml.h"
#include <ssc/macros.hpp>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>

#include "Body.hpp"
#include "GRPCError.hpp"
#include "GRPCForceModel.hpp"
#include "GRPCTypes.hpp"
#include "ToGRPC.hpp"
#include "FromGRPC.hpp"


void throw_if_invalid_status(const GRPCForceModel::Input& input, const std::string& rpc_method, const grpc::Status& status);
void throw_if_invalid_status(const GRPCForceModel::Input& input, const std::string& rpc_method, const grpc::Status& status)
{
    const std::string base_error_msg = std::string("an error occurred when using the distant force model '") + input.name + "' defined via gRPC (method '" + rpc_method + "').\n";
    const std::string yaml_msg = "The YAML you provided for this gRPC model is:\n\n" + input.yaml + "\n";
    const std::string contact_team_msg = "support team with the following error code (cf. https://grpc.github.io/grpc/cpp/grpc_2impl_2codegen_2status_8h.html): ";
    const std::string user_error_msg = "Check that the server is running and accessible from the URL defined in the YAML file: " + input.url;
    const std::string dev_error_msg = "This is a problem with xdyn and should never happen: please contact xdyn's " + contact_team_msg;
    const std::string network_error_msg = "We couldn't reach the gRPC force model server at this URL: " + input.url + " Maybe the server is temporarily inaccessible or hasn't started yet? Either wait and give it another try or change the 'url' key in the YAML file. If all else fails, contact xdyn's " + contact_team_msg;
    const std::string server_error_msg = "The gRPC force model server responded with the following error code: " + status.error_message()  + "\nMaybe the parameters you defined in xdyn's YAML file are incorrect? " + yaml_msg + "\nPlease check the force model server's documentation! If the problem persists, try contacting the force model server's " + contact_team_msg;
    const std::string authentication_error_msg = "This force model server requires authentication and xdyn does not support this yet.";
    const std::string unimplemented = "This force model server does not implement gRPC method '" + rpc_method + "': implement it or use another model. Otherwise, contact the force model's " + contact_team_msg;
    switch(status.error_code())
    {
        case grpc::StatusCode::OK:
        {
            return;
            break;
        }
        case grpc::StatusCode::ABORTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "ABORTED");
            break;
        }
        case grpc::StatusCode::ALREADY_EXISTS:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "ALREADY_EXISTS");
            break;
        }
        case grpc::StatusCode::CANCELLED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "CANCELLED");
            break;
        }
        case grpc::StatusCode::DATA_LOSS:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + network_error_msg + "DATA_LOSS");
            break;
        }
        case grpc::StatusCode::DEADLINE_EXCEEDED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + network_error_msg + "DEADLINE_EXCEEDED");
            break;
        }
        case grpc::StatusCode::DO_NOT_USE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + dev_error_msg + "DO_NOT_USE");
            break;
        }
        case grpc::StatusCode::FAILED_PRECONDITION:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "FAILED_PRECONDITION");
            break;
        }
        case grpc::StatusCode::INTERNAL:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "INTERNAL");
            break;
        }
        case grpc::StatusCode::INVALID_ARGUMENT:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "INVALID_ARGUMENT");
            break;
        }
        case grpc::StatusCode::NOT_FOUND:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "NOT_FOUND");
            break;
        }
        case grpc::StatusCode::OUT_OF_RANGE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "OUT_OF_RANGE");
            break;
        }
        case grpc::StatusCode::PERMISSION_DENIED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + authentication_error_msg);
            break;
        }
        case grpc::StatusCode::RESOURCE_EXHAUSTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "RESOURCE_EXHAUSTED");
            break;
        }
        case grpc::StatusCode::UNAUTHENTICATED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + authentication_error_msg);
            break;
        }
        case grpc::StatusCode::UNAVAILABLE:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg  + network_error_msg + "UNAVAILABLE");
            break;
        }
        case grpc::StatusCode::UNIMPLEMENTED:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + unimplemented + "UNIMPLEMENTED");
            break;
        }
        case grpc::StatusCode::UNKNOWN:
        {
            THROW(__PRETTY_FUNCTION__, GRPCError, base_error_msg + server_error_msg + "UNKNOWN");
            break;
        }
        default:
        {
            return; // Do nothing: should never happen
            break;
        }
    }
}

class GRPCForceModel::Impl
{
    public:
        Impl(const GRPCForceModel::Input& input_, const std::vector<std::string>& rotation_convention_, const std::string& body_name)
            : input(input_)
            , stub(Force::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
            , extra_observations()
            , max_history_length()
            , needs_wave_outputs()
            , rotation_convention(rotation_convention_)
            , to_grpc(ToGRPC(input))
            , from_grpc(FromGRPC())
            , commands()
            , force_frame()
        {
            set_parameters(input.yaml, body_name);
        }

        GRPCForceModel::Input get_input() const
        {
            return input;
        }

        void set_parameters(const std::string& yaml, const std::string& body_name)
        {
            SetForceParameterResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->set_parameters(&context, to_grpc.from_yaml(yaml, body_name), &response);
            throw_if_invalid_status(input, "set_parameters", status);
            needs_wave_outputs = response.needs_wave_outputs();
            max_history_length = response.max_history_length();
            commands.reserve(response.commands_size());
            std::copy(response.commands().begin(), response.commands().end(), std::back_inserter(commands));
            force_frame.frame = response.frame();
            force_frame.angle.phi = response.phi();
            force_frame.angle.theta = response.theta();
            force_frame.angle.psi = response.psi();
            force_frame.coordinates.x = response.x();
            force_frame.coordinates.y = response.y();
            force_frame.coordinates.z = response.z();
        }

        WaveRequest required_wave_information(const double t, const double x, const double y, const double z) const
        {
            const RequiredWaveInformationRequest required_wave_information_request = to_grpc.from_required_wave_information(t, x, y, z);
            RequiredWaveInformationResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->required_wave_information(&context, required_wave_information_request, &response);
            throw_if_invalid_status(input, "required_wave_information", status);
            return from_grpc.to_wave_request(response);
        }

        ssc::kinematics::Vector6d force(const double t, const BodyStates& state, const std::map<std::string,double>& commands, const EnvironmentAndFrames& env)
        {
            ForceResponse response;
            grpc::ClientContext context;
            const auto states = to_grpc.from_state(state, max_history_length, env);
            const auto wave_information = get_wave_information(t, state.x(0), state.y(0), state.z(0), env);
            const grpc::Status status = stub->force(&context, to_grpc.from_force_request(states, commands, wave_information), &response);
            throw_if_invalid_status(input, "force", status);
            extra_observations = std::map<std::string,double>(response.extra_observations().begin(),response.extra_observations().end());
            return from_grpc.to_force(response);
        }

        double get_Tmax() const
        {
            return max_history_length;
        }

        std::map<std::string,double> get_extra_observations() const
        {
            return extra_observations;
        }

        std::vector<std::string> get_commands() const
        {
            return commands;
        }

        YamlPosition get_transformation_to_model_frame() const
        {
            return force_frame;
        }

    private:
        Impl(); // Disabled
        WaveInformation* get_wave_information(const double t, const double x, const double y, const double z, const EnvironmentAndFrames& env) const
        {
            if (needs_wave_outputs)
            {
                const WaveRequest wave_request = required_wave_information(t, x, y, z);
                return to_grpc.from_wave_information(wave_request, t, env);
            }
            return new WaveInformation();
        }
        GRPCForceModel::Input input;
        std::unique_ptr<Force::Stub> stub;
        std::map<std::string,double> extra_observations;
        double max_history_length;
        bool needs_wave_outputs;
        std::vector<std::string> rotation_convention;
        ToGRPC to_grpc;
        FromGRPC from_grpc;
        std::vector<std::string> commands;
        YamlPosition force_frame;
};

std::string GRPCForceModel::model_name() {return "grpc";}


GRPCForceModel::Input GRPCForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    GRPCForceModel::Input ret;
    node["url"] >> ret.url;
    node["name"] >> ret.name;
    YAML::Emitter out;
    out << node;
    ret.yaml = out.c_str();
    return ret;
}

GRPCForceModel::GRPCForceModel(const GRPCForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        GRPCForceModel(TR1(shared_ptr)<GRPCForceModel::Impl>(new GRPCForceModel::Impl(input, env_.rot.convention, body_name_)), body_name_, env_)

{
}

GRPCForceModel::GRPCForceModel(const TR1(shared_ptr)<Impl>& pimpl_, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(pimpl_->get_input().name, pimpl_->get_commands(), pimpl_->get_transformation_to_model_frame(), body_name_, env_),
        pimpl(pimpl_),
        env(env_)

{
}

ssc::kinematics::Vector6d GRPCForceModel::get_force(const BodyStates& states, const double t, const std::map<std::string,double>& commands) const
{
    const auto ret = pimpl->force(t, states, commands, env);
    return ret;
}

void GRPCForceModel::extra_observations(Observer& observer) const
{
    const auto extra_observations = pimpl->get_extra_observations();
    for (const auto observation : extra_observations)
    {
        observer.write(observation.second, DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),observation.first},observation.first + std::string("(") + get_body_name() + ")"));
    }
}

double GRPCForceModel::get_Tmax() const
{
    return pimpl->get_Tmax();
}

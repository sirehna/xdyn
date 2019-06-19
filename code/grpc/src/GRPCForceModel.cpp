/*
 * GRPCForceModel.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */


#include <grpcpp/grpcpp.h>
#include "force.grpc.pb.h"

#include "yaml.h"
#include <ssc/macros.hpp>
#include <ssc/yaml_parser.hpp>
#include <ssc/kinematics.hpp>

#include "Body.hpp"
#include "GRPCError.hpp"
#include "GRPCForceModel.hpp"

void throw_if_invalid_status(const GRPCForceModel::Input& input, const std::string& rpc_method, const grpc::Status& status);
void throw_if_invalid_status(const GRPCForceModel::Input& input, const std::string& rpc_method, const grpc::Status& status)
{
    const std::string base_error_msg = std::string("an error occurred when using the distant force model '") + input.name + "' defined via gRPC (method '" + rpc_method + "').\n";
    const std::string yaml_msg = "The YAML you provided for this gRPC model is:\n\n" + input.yaml + "\n";
    const std::string contact_team_msg = "support team with the following error code (cf. https://grpc.github.io/grpc/cpp/grpc_2impl_2codegen_2status_8h.html): ";
    const std::string user_error_msg = "Check that the server is running and accessible from the URL defined in the YAML file: " + input.url;
    const std::string dev_error_msg = "This is a problem with xdyn and should never happen: please contact xdyn's " + contact_team_msg;
    const std::string network_error_msg = "We couldn't reach the gRPC force model server at this URL: " + input.url + " Maybe the server is temporarily inaccessible or hasn't started yet? Either wait and give it another try or change the 'url' key in the YAML file. If all else fails, contact xdyn's " + contact_team_msg;
    const std::string server_error_msg = "The wave model server responded with the following error code: " + status.error_message()  + "\nMaybe the parameters you defined in xdyn's YAML file are incorrect? " + yaml_msg + "\nPlease check the force model server's documentation! If the problem persists, try contacting the force model server's " + contact_team_msg;
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
        Impl(const Input& input_)
            : input(input_)
            , stub(Force::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
        {
            SetParameterRequest request;
            request.set_parameters(input.yaml);
            SetParameterResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->set_parameters(&context, request, &response);
            throw_if_invalid_status(input, "set_parameters", status);
        }

    private:
        Impl(); // Disabled
        Input input;
        std::unique_ptr<Force::Stub> stub;
};

std::string GRPCForceModel::model_name() {return "grpc";}

GRPCForceModel::Input::Input() :
        url(),
        name(),
        yaml()
{
}

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

std::vector<std::string> get_commands_from_grpc(const GRPCForceModel::Input& input);
std::vector<std::string> get_commands_from_grpc(const GRPCForceModel::Input& input)
{
    std::unique_ptr<Force::Stub> stub = Force::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials()));
    CommandsRequest request;
    CommandsResponse response;
    grpc::ClientContext context;
    const grpc::Status status = stub->get_commands(&context, request, &response);
    throw_if_invalid_status(input, "get_commands", status);
    std::vector<std::string> ret;
    ret.reserve(response.commands_size());
    std::copy(response.commands().begin(), response.commands().end(), std::back_inserter(ret));
    return ret;
}

YamlPosition get_origin_of_BODY_frame(const std::string& body_name);
YamlPosition get_origin_of_BODY_frame(const std::string& body_name)
{
    YamlPosition ret;
    ret.frame = body_name;
    return ret;
}

GRPCForceModel::GRPCForceModel(const GRPCForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(model_name(), get_commands_from_grpc(input), get_origin_of_BODY_frame(body_name_), body_name_, env),
        env(env_),
        pimpl(new Impl(input))

{
}

ssc::kinematics::Vector6d GRPCForceModel::get_force(const BodyStates& , const double , const std::map<std::string,double>& ) const
{
    return ssc::kinematics::Vector6d();
}

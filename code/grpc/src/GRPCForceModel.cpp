/*
 * GRPCForceModel.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */


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

struct XYTs
{
    std::vector<double> x;
    std::vector<double> y;
    double t;
};

struct XYZTs
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    double t;
};

struct XYT
{
    double x;
    double y;
    double t;
};

struct WaveRequest
{
    XYTs elevations;
    XYZTs dynamic_pressures;
    XYZTs orbital_velocities;
    XYT spectrum;
    bool angular_frequencies_for_rao;
    bool directions_for_rao;
    bool need_spectrum;
};

class FromGRPC
{
    public:
        WaveRequest to_wave_request(const RequiredWaveInformationResponse& response) const
        {
            WaveRequest ret;
            ret.angular_frequencies_for_rao = response.angular_frequencies_for_rao();
            ret.directions_for_rao = response.directions_for_rao();
            ret.dynamic_pressures.t = response.dynamic_pressures().t();
            ret.dynamic_pressures.x.reserve(response.dynamic_pressures().x_size());
            std::copy(response.dynamic_pressures().x().begin(), response.dynamic_pressures().x().end(), std::back_inserter(ret.dynamic_pressures.x));
            ret.dynamic_pressures.y.reserve(response.dynamic_pressures().y_size());
            std::copy(response.dynamic_pressures().y().begin(), response.dynamic_pressures().y().end(), std::back_inserter(ret.dynamic_pressures.y));
            ret.elevations.t = response.elevations().t();
            ret.elevations.x.reserve(response.elevations().x_size());
            std::copy(response.elevations().x().begin(), response.elevations().x().end(), std::back_inserter(ret.elevations.x));
            ret.elevations.y.reserve(response.elevations().y_size());
            std::copy(response.elevations().y().begin(), response.elevations().y().end(), std::back_inserter(ret.elevations.y));
            ret.need_spectrum = response.need_spectrum();
            ret.orbital_velocities.t = response.orbital_velocities().t();
            ret.orbital_velocities.x.reserve(response.orbital_velocities().x_size());
            std::copy(response.orbital_velocities().x().begin(), response.orbital_velocities().x().end(), std::back_inserter(ret.orbital_velocities.x));
            ret.orbital_velocities.y.reserve(response.orbital_velocities().y_size());
            std::copy(response.orbital_velocities().y().begin(), response.orbital_velocities().y().end(), std::back_inserter(ret.orbital_velocities.y));
            ret.orbital_velocities.z.reserve(response.orbital_velocities().z_size());
            std::copy(response.orbital_velocities().z().begin(), response.orbital_velocities().z().end(), std::back_inserter(ret.orbital_velocities.z));
            ret.spectrum.t = response.spectrum().t();
            ret.spectrum.x = response.spectrum().x();
            ret.spectrum.y = response.spectrum().y();
            return ret;
        }

        ssc::kinematics::Vector6d to_force(const ForceResponse& response) const
        {
            ssc::kinematics::Vector6d ret;
            ret(0) = response.fx();
            ret(1) = response.fy();
            ret(2) = response.fz();
            ret(3) = response.mx();
            ret(4) = response.my();
            ret(5) = response.mz();
            return ret;
        }

};

class ToGRPC
{
    public:
        ToGRPC(const GRPCForceModel::Input& input_) : input(input_) {}
        RequiredWaveInformationRequest from_required_wave_information(const double t, const double x, const double y, const double z) const
        {
            RequiredWaveInformationRequest request;
            request.set_t(t);
            request.set_x(x);
            request.set_y(y);
            request.set_z(z);
            return request;
        }

        SpectrumResponse* from_discrete_directional_wave_spectra(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra) const
        {
            SpectrumResponse* spectrum_response = new SpectrumResponse();
            for (const auto& spectrum:spectra)
            {
                const auto s = spectrum_response->add_spectrum();
                for (const auto& Si:spectrum.Si)
                {
                    s->add_si(Si);
                }
                for (const auto& Dj:spectrum.Dj)
                {
                    s->add_dj(Dj);
                }
                for (const auto& k:spectrum.k)
                {
                    s->add_k(k);
                }
                for (const auto& omega:spectrum.omega)
                {
                    s->add_omega(omega);
                }
                const auto p = s->add_phase();
                for (const auto& phases:spectrum.phase)
                {
                    for (const auto& phase:phases)
                    {
                        p->add_phase(phase);
                    }
                }
            }
            return spectrum_response;
        }

        WaveInformation* from_wave_information(const WaveRequest& wave_request, const double t, const EnvironmentAndFrames& env) const
        {
            WaveInformation* wave_information = new WaveInformation();
            if (env.w.use_count())
            {
                if (wave_request.need_spectrum)
                {
                    try
                    {
                        const auto directional_spectra = env.w->get_directional_spectra(wave_request.spectrum.x, wave_request.spectrum.y, wave_request.spectrum.t);
                        auto spectrum = from_discrete_directional_wave_spectra(directional_spectra);
                        wave_information->set_allocated_spectrum(spectrum);
                    }
                    catch (const ssc::exception_handling::Exception& e)
                    {
                        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which requires a linearized wave directional spectrum. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                    }
                }
                try
                {
                    wave_information->mutable_orbital_velocities()->set_t(t);
                    copy_from_double_vector(wave_request.orbital_velocities.x, wave_information->mutable_orbital_velocities()->mutable_x());
                    copy_from_double_vector(wave_request.orbital_velocities.y, wave_information->mutable_orbital_velocities()->mutable_y());
                    copy_from_double_vector(wave_request.orbital_velocities.z, wave_information->mutable_orbital_velocities()->mutable_z());
                    std::vector<double> eta;
                    try
                    {
                        eta = env.w->wave_height(wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.t);
                    }
                    catch (const ssc::exception_handling::Exception& e)
                    {
                        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which, indirectly, needs wave elevations (to compute the dynamic pressures). When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                    }
                    const ssc::kinematics::PointMatrix orbital_velocities = env.w->orbital_velocity(env.g, wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.z, t, eta);

                    std::vector<double> vx(orbital_velocities.m.cols()), vy(orbital_velocities.m.cols()), vz(orbital_velocities.m.cols());
                    for (int j = 0 ; j < orbital_velocities.m.cols() ; ++j)
                    {
                        vx[j] = orbital_velocities.m(0,j);
                        vy[j] = orbital_velocities.m(1,j);
                        vz[j] = orbital_velocities.m(2,j);
                    }
                    copy_from_double_vector(vx, wave_information->mutable_orbital_velocities()->mutable_vx());
                    copy_from_double_vector(vy, wave_information->mutable_orbital_velocities()->mutable_vy());
                    copy_from_double_vector(vz, wave_information->mutable_orbital_velocities()->mutable_vz());
                }
                catch (const ssc::exception_handling::Exception& e)
                {
                    THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs orbital velocities. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                }
                try
                {
                    wave_information->mutable_elevations()->set_t(t);
                    copy_from_double_vector(wave_request.elevations.x, wave_information->mutable_elevations()->mutable_x());
                    copy_from_double_vector(wave_request.elevations.y, wave_information->mutable_elevations()->mutable_y());
                    copy_from_double_vector(env.w->wave_height(wave_request.elevations.x, wave_request.elevations.y, wave_request.elevations.t), wave_information->mutable_elevations()->mutable_z());
                }
                catch (const ssc::exception_handling::Exception& e)
                {
                    THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs wave elevations. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                }
                try
                {
                    wave_information->mutable_dynamic_pressures()->set_t(t);
                    copy_from_double_vector(wave_request.dynamic_pressures.x, wave_information->mutable_dynamic_pressures()->mutable_x());
                    copy_from_double_vector(wave_request.dynamic_pressures.y, wave_information->mutable_dynamic_pressures()->mutable_y());
                    copy_from_double_vector(wave_request.dynamic_pressures.z, wave_information->mutable_dynamic_pressures()->mutable_z());
                    const std::vector<double> eta = env.w->wave_height(wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.t);
                    copy_from_double_vector(env.w->dynamic_pressure(env.rho, env.g, wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.z, eta, t), wave_information->mutable_dynamic_pressures()->mutable_pdyn());
                }
                catch (const ssc::exception_handling::Exception& e)
                {
                    THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs dynamic pressures. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                }
                try
                {
                    wave_information->mutable_orbital_velocities()->set_t(t);
                    copy_from_double_vector(wave_request.orbital_velocities.x, wave_information->mutable_orbital_velocities()->mutable_x());
                    copy_from_double_vector(wave_request.orbital_velocities.y, wave_information->mutable_orbital_velocities()->mutable_y());
                    copy_from_double_vector(wave_request.orbital_velocities.z, wave_information->mutable_orbital_velocities()->mutable_z());
                }
                catch (const ssc::exception_handling::Exception& e)
                {
                    THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs orbital velocities. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
                }
            }
            else
            {
                THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which needs data from a wave model. However, none were defined in the YAML file: please define a wave model in the 'environment models' section of the YAML file.");
            }
            return wave_information;
        }

        States* from_state(const BodyStates& state, const double max_history_length, const EnvironmentAndFrames& env) const
        {
            const auto qr = state.qr.get_values(max_history_length);
            const auto qi = state.qi.get_values(max_history_length);
            const auto qj = state.qj.get_values(max_history_length);
            const auto qk = state.qk.get_values(max_history_length);
            std::vector<double> phi(qr.size());
            std::vector<double> theta(qr.size());
            std::vector<double> psi(qr.size());
            for (size_t i = 0 ; i < qr.size() ; ++i)
            {
                ssc::kinematics::RotationMatrix R = Eigen::Quaternion<double>(qr[i],qi[i],qj[i],qk[i]).matrix();
                const ssc::kinematics::EulerAngles euler_angles = state.convert(R, env.rot);
                phi[i] = euler_angles.phi;
                theta[i] = euler_angles.theta;
                psi[i] = euler_angles.psi;
            }
            States* ret = new States();

            copy_from_double_vector(state.x.get_dates(max_history_length), ret->mutable_t());
            copy_from_double_vector(state.x.get_values(max_history_length), ret->mutable_x());
            copy_from_double_vector(state.y.get_values(max_history_length), ret->mutable_y());
            copy_from_double_vector(state.z.get_values(max_history_length), ret->mutable_z());
            copy_from_double_vector(state.u.get_values(max_history_length), ret->mutable_u());
            copy_from_double_vector(state.v.get_values(max_history_length), ret->mutable_v());
            copy_from_double_vector(state.w.get_values(max_history_length), ret->mutable_w());
            copy_from_double_vector(state.p.get_values(max_history_length), ret->mutable_p());
            copy_from_double_vector(state.q.get_values(max_history_length), ret->mutable_q());
            copy_from_double_vector(state.r.get_values(max_history_length), ret->mutable_r());
            copy_from_double_vector(qr, ret->mutable_qr());
            copy_from_double_vector(qi, ret->mutable_qi());
            copy_from_double_vector(qj, ret->mutable_qj());
            copy_from_double_vector(qk, ret->mutable_qk());
            copy_from_double_vector(phi, ret->mutable_phi());
            copy_from_double_vector(theta, ret->mutable_theta());
            copy_from_double_vector(psi, ret->mutable_psi());
            copy_from_string_vector(env.rot.convention, ret->mutable_rotations_convention());
            return ret;
        }

        ForceRequest from_force_request(States* states, const std::map<std::string, double >& commands, WaveInformation* wave_information) const
        {
            ForceRequest request;
            request.set_allocated_wave_information(wave_information);
            request.mutable_commands()->insert(commands.begin(), commands.end());
            request.set_allocated_states(states);
            return request;
        }

        SetForceParameterRequest from_yaml(const std::string& yaml) const
        {
            SetForceParameterRequest request;
            request.set_parameters(yaml);
            return request;
        }

    private:
        void copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination) const
        {
            *destination = {origin.begin(), origin.end()};
        }

        void copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination) const
        {
            *destination = {origin.begin(), origin.end()};
        }
        GRPCForceModel::Input input;
        ToGRPC(); // Disabled

};

class GRPCForceModel::Impl
{
    public:
        Impl(const Input& input_, const std::vector<std::string>& rotation_convention_)
            : input(input_)
            , stub(Force::NewStub(grpc::CreateChannel(input.url, grpc::InsecureChannelCredentials())))
            , extra_observations()
            , max_history_length()
            , needs_wave_outputs()
            , rotation_convention(rotation_convention_)
            , to_grpc(ToGRPC(input))
            , from_grpc(FromGRPC())
        {
            set_parameters(input.yaml);
        }

        void set_parameters(const std::string& yaml)
        {
            SetForceParameterResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->set_parameters(&context, to_grpc.from_yaml(yaml), &response);
            throw_if_invalid_status(input, "set_parameters", status);
            needs_wave_outputs = response.needs_wave_outputs();
            max_history_length = response.max_history_length();
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
        Input input;
        std::unique_ptr<Force::Stub> stub;
        std::map<std::string,double> extra_observations;
        double max_history_length;
        bool needs_wave_outputs;
        std::vector<std::string> rotation_convention;
        ToGRPC to_grpc;
        FromGRPC from_grpc;
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
    return YamlPosition(YamlCoordinates(), YamlAngle(), body_name);
}

GRPCForceModel::GRPCForceModel(const GRPCForceModel::Input& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(input.name, get_commands_from_grpc(input), get_origin_of_BODY_frame(body_name_), body_name_, env_),
        env(env_),
        pimpl(new Impl(input, env_.rot.convention))

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

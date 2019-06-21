/*
 * GRPCWaveModel.cpp
 *
 *  Created on: May 27, 2019
 *      Author: cady
 */
#include <algorithm>
#include <cmath> // For cos, sin
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <grpcpp/grpcpp.h>
#include "wave_grpc.grpc.pb.h"
#include "wave_types.grpc.pb.h"

#include "discretize.hpp"
#include "SurfaceElevationFromGRPC.hpp"
#include "InternalErrorException.hpp"
#include "GRPCError.hpp"
#include "YamlGRPC.hpp"

class SurfaceElevationFromGRPC::Impl
{
    public:
        Impl(const std::string& url_, const std::string& yaml_)
            : url(url_)
            , stub(Waves::NewStub(grpc::CreateChannel(url, grpc::InsecureChannelCredentials())))
            , yaml(yaml_)
        {
            SetParameterRequest request;
            request.set_parameters(yaml);
            SetParameterResponse response;
            grpc::ClientContext context;
            const grpc::Status status = stub->set_parameters(&context, request, &response);
            throw_if_invalid_status("set_parameters", status);
        }

        void throw_if_invalid_status(const std::string& rpc_method, const grpc::Status& status) const
        {
            const std::string base_error_msg = "an error occurred when using the distant wave model defined via gRPC (method '" + rpc_method + "').\n";
            const std::string yaml_msg = "The YAML you provided for this gRPC model is:\n\n" + yaml + "\n";
            const std::string contact_team_msg = "support team with the following error code (cf. https://grpc.github.io/grpc/cpp/grpc_2impl_2codegen_2status_8h.html): ";
            const std::string user_error_msg = "Check that the server is running and accessible from the URL defined in the YAML file: " + url;
            const std::string dev_error_msg = "This is a problem with xdyn and should never happen: please contact xdyn's " + contact_team_msg;
            const std::string network_error_msg = "We couldn't reach the gRPC wave server at this URL: " + url + " Maybe the server is temporarily inaccessible or hasn't started yet? Either wait and give it another try or change the 'url' key in the YAML file. If all else fails, contact xdyn's " + contact_team_msg;
            const std::string server_error_msg = "The wave model server responded with the following error code: " + status.error_message()  + "\nMaybe the parameters you defined in xdyn's YAML file are incorrect? " + yaml_msg + "\nPlease check the wave server's documentation! If the problem persists, try contacting the wave server's " + contact_team_msg;
            const std::string authentication_error_msg = "This wave server requires authentication and xdyn does not support this yet.";
            const std::string unimplemented = "This wave server does not implement gRPC method '" + rpc_method + "': either disable all force models that need it or use another wave model. Otherwise, contact the wave model's " + contact_team_msg;
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

        std::vector<double> elevations(const std::vector<double>& x, const std::vector<double>& y, const double t)
        {
            XYTGrid request;
            *request.mutable_x() = {x.begin(),x.end()};
            *request.mutable_y() = {y.begin(),y.end()};
            request.set_t(t);
            grpc::ClientContext context;
            XYZTGrid response;
            const grpc::Status status = stub->elevations(&context, request, &response);
            throw_if_invalid_status("elevations", status);
            std::vector<double> ret;
            ret.reserve(response.z_size());
            std::copy(response.z().begin(), response.z().end(), std::back_inserter(ret));
            return ret;
        }

        std::vector<double> dynamic_pressures(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, const double t)
        {
            XYZTGrid request;
            *request.mutable_x() = {x.begin(),x.end()};
            *request.mutable_y() = {y.begin(),y.end()};
            *request.mutable_z() = {z.begin(),z.end()};
            request.set_t(t);
            grpc::ClientContext context;
            DynamicPressuresResponse response;
            const grpc::Status status = stub->dynamic_pressures(&context, request, &response);
            throw_if_invalid_status("dynamic_pressures", status);
            std::vector<double> ret;
            ret.reserve(response.pdyn_size());
            std::copy(response.pdyn().begin(), response.pdyn().end(), std::back_inserter(ret));
            return ret;
        }

        ssc::kinematics::PointMatrix orbital_velocities(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, const double t)
        {
            XYZTGrid request;
            *request.mutable_x() = {x.begin(),x.end()};
            *request.mutable_y() = {y.begin(),y.end()};
            *request.mutable_z() = {z.begin(),z.end()};
            request.set_t(t);
            grpc::ClientContext context;
            OrbitalVelocitiesResponse response;
            const grpc::Status status = stub->orbital_velocities(&context, request, &response);
            throw_if_invalid_status("orbital_velocities", status);
            if (response.vx_size() != 1)
            {
                THROW(__PRETTY_FUNCTION__, GRPCError, "An error has occurred when using a distant wave model defined via gRPC. The model is defined in the YAML by:\n" + yaml + "\nxdyn managed to contact the wave server but during initialization (set_parameters), the server returned the following error message:\n" << "Was only expecting one orbital velocity for x, but got a vector of size " << response.vx_size());
            }
            if (response.vy_size() != 1)
            {
                THROW(__PRETTY_FUNCTION__, GRPCError, "An error has occurred when using a distant wave model defined via gRPC. The model is defined in the YAML by:\n" + yaml + "\nxdyn managed to contact the wave server but during initialization (set_parameters), the server returned the following error message:\n" << "Was only expecting one orbital velocity for x, but got a vector of size " << response.vy_size());
            }
            if (response.vz_size() != 1)
            {
                THROW(__PRETTY_FUNCTION__, GRPCError, "An error has occurred when using a distant wave model defined via gRPC. The model is defined in the YAML by:\n" + yaml + "\nxdyn managed to contact the wave server but during initialization (set_parameters), the server returned the following error message:\n" << "Was only expecting one orbital velocity for x, but got a vector of size " << response.vz_size());
            }
            std::vector<double> vx, vy, vz;
            vx.reserve(response.vx_size());
            vy.reserve(response.vy_size());
            vz.reserve(response.vz_size());
            std::copy(response.vx().begin(), response.vx().end(), std::back_inserter(vx));
            std::copy(response.vy().begin(), response.vy().end(), std::back_inserter(vy));
            std::copy(response.vz().begin(), response.vz().end(), std::back_inserter(vz));
            ssc::kinematics::PointMatrix ret("NED", response.vx_size());
            for (int i = 0 ; i < response.vx_size() ; ++i)
            {
                ret.m(0, i) = response.vx(i);
                ret.m(1, i) = response.vy(i);
                ret.m(2, i) = response.vz(i);
            }
            return ret;
        }

        std::vector<FlatDiscreteDirectionalWaveSpectrum> directional_spectra(const double x, const double y, const double t)
        {
            SpectrumRequest request;
            request.set_x(x);
            request.set_y(y);
            request.set_t(t);
            grpc::ClientContext context;
            SpectrumResponse response;
            const grpc::Status status = stub->spectrum(&context, request, &response);
            throw_if_invalid_status("spectrum", status);
            std::vector<FlatDiscreteDirectionalWaveSpectrum> ret;
            {
                for (int i = 0 ; i < response.spectrum_size() ; ++i)
                {
                    DiscreteDirectionalWaveSpectrum s;
                    s.Si.reserve(response.spectrum(i).si_size());
                    std::copy(response.spectrum(i).si().begin(), response.spectrum(i).si().end(), std::back_inserter(s.Si));
                    s.Dj.reserve(response.spectrum(i).dj_size());
                    std::copy(response.spectrum(i).dj().begin(), response.spectrum(i).dj().end(), std::back_inserter(s.Dj));
                    s.omega.reserve(response.spectrum(i).omega_size());
                    std::copy(response.spectrum(i).omega().begin(), response.spectrum(i).omega().end(), std::back_inserter(s.omega));
                    s.psi.reserve(response.spectrum(i).psi_size());
                    std::copy(response.spectrum(i).psi().begin(), response.spectrum(i).psi().end(), std::back_inserter(s.psi));
                    s.k.reserve(response.spectrum(i).k_size());
                    std::copy(response.spectrum(i).k().begin(), response.spectrum(i).k().end(), std::back_inserter(s.k));
                    if (response.spectrum(i).phase_size())
                    {
                        s.phase.resize(response.spectrum(i).phase_size());
                        for (int i = 0 ; i < response.spectrum(i).phase_size() ; ++i)
                        {
                            s.phase[i].reserve(response.spectrum(i).phase(i).phase_size());
                            std::copy(response.spectrum(i).phase(i).phase().begin(), response.spectrum(i).phase(i).phase().end(), std::back_inserter(s.phase[i]));
                        }
                    }
                    ret.push_back(flatten(s));
                }
            }
            return ret;
        }

        std::vector<std::vector<double> > get_wave_directions_for_each_model()
        {
            DirectionsRequest request;
            grpc::ClientContext context;
            DirectionsResponse response;
            const grpc::Status status = stub->directions_for_rao(&context, request, &response);
            throw_if_invalid_status("directions_for_rao", status);
            std::vector<std::vector<double> > wave_directions;
            if (response.directions_size())
            {
                wave_directions.resize(response.directions_size());
                for (int i = 0 ; i < response.directions_size() ; ++i)
                {
                    wave_directions[i].reserve(response.directions(i).psis_size());
                    std::copy(response.directions(i).psis().begin(), response.directions(i).psis().end(), std::back_inserter(wave_directions[i]));
                }

            }
            return wave_directions;
        }

        std::vector<std::vector<double> > get_wave_angular_frequency_for_each_model()
        {
            AngularFrequenciesRequest request;
            grpc::ClientContext context;
            AngularFrequenciesResponse response;
            const grpc::Status status = stub->angular_frequencies_for_rao(&context, request, &response);
            throw_if_invalid_status("angular_frequencies_for_rao", status);
            std::vector<std::vector<double> > omegas;
            if (response.angular_frequencies_size())
            {
                omegas.resize(response.angular_frequencies_size());
                for (int i = 0 ; i < response.angular_frequencies_size() ; ++i)
                {
                    omegas[i].reserve(response.angular_frequencies(i).omegas_size());
                    std::copy(response.angular_frequencies(i).omegas().begin(), response.angular_frequencies(i).omegas().end(), std::back_inserter(omegas[i]));
                }
            }
            return omegas;
        }

    private:
        Impl();
        std::string url;
        std::unique_ptr<Waves::Stub> stub;
        std::string yaml;

};

SurfaceElevationFromGRPC::SurfaceElevationFromGRPC(const YamlGRPC& yaml, const ssc::kinematics::PointMatrixPtr& output_mesh) : SurfaceElevationInterface(output_mesh), pimpl(new Impl(yaml.url, yaml.rest_of_the_yaml))
{
}

std::vector<std::vector<double> > SurfaceElevationFromGRPC::get_wave_directions_for_each_model() const
{
    return pimpl->get_wave_directions_for_each_model();
}

std::vector<std::vector<double> > SurfaceElevationFromGRPC::get_wave_angular_frequency_for_each_model() const
{
    return pimpl->get_wave_angular_frequency_for_each_model();
}

std::vector<double> SurfaceElevationFromGRPC::wave_height(const std::vector<double>& x,                                  //!< x-position in the NED frame (in meters)
                              const std::vector<double>& y,                                  //!< y-position in the NED frame (in meters)
                              const double t                                   //!< Current time instant (in seconds)
                              ) const
{
    return pimpl->elevations(x, y, t);
}

/**  \brief Unsteady pressure field induced by undisturbed waves. Used to compute the Froude-Krylov forces.
  *  \details Also called "subsurface pressure" (by DNV), "unsteady pressure" (by Faltinsen) or constant pressure contour (by Lloyd)
  *           The dynamic pressure is in fact one of the terms of Bernoulli's equation, which can be derived from the conservation
  *           of energy for a fluid in motion.
  *           \f[\int_C \frac{\partial \Phi_I(x,y,z,t)}{\partial t}\f]
  *  \returns Dynamic pressure in Pascal
  *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
  *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 185
  *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
  *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation VI.34, page 183
  *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 68
  *  \see "The dynamic of marine craft", 2004, Lewandoski, page 148
  *  \snippet environment_models/unit_tests/src/AiryTest.cpp AiryTest elevation_example
  */
std::vector<double> SurfaceElevationFromGRPC::dynamic_pressure(const double , //!< water density (in kg/m^3) (not used for gRPC)
        const double ,   //!< gravity (in m/s^2) (not used for gRPC)
        const std::vector<double>& x,   //!< x-position in the NED frame (in meters)
        const std::vector<double>& y,   //!< y-position in the NED frame (in meters)
        const std::vector<double>& z,   //!< z-position in the NED frame (in meters)
        const std::vector<double>& , //!< Wave elevation at (x,y) in the NED frame (in meters)
        const double t    //!< Current time instant (in seconds)
        ) const
{
    return pimpl->dynamic_pressures(x, y, z, t);
}

/**  \brief Wave velocity (projected in the NED frame, at a point (x,y,z)).
  *  \returns Orbital velocity in m/s
  *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
  *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 70
  *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
  *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 75
  *  \see "The dynamic of marine craft", 2004, Lewandoski, page 148
  */
ssc::kinematics::Point SurfaceElevationFromGRPC::orbital_velocity(const double g,   //!< gravity (in m/s^2) (not used for gRPC)
                                        const double x,   //!< x-position in the NED frame (in meters)
                                        const double y,   //!< y-position in the NED frame (in meters)
                                        const double z,   //!< z-position in the NED frame (in meters)
                                        const double t,   //!< Current time instant (in seconds)
                                        const double eta  //!< Wave height at x,y,t (in meters) (not used for gRPC)
                                        ) const
{
    const ssc::kinematics::PointMatrix Ps = this->orbital_velocity(g, std::vector<double>(1,x), std::vector<double>(1,y), std::vector<double>(1,z), t, std::vector<double>(1,eta));
    return ssc::kinematics::Point(Ps.get_frame(), Ps.m(0,0), Ps.m(1,0), Ps.m(2,0));
}

ssc::kinematics::PointMatrix SurfaceElevationFromGRPC::orbital_velocity(const double ,                //!< gravity (in m/s^2)
                                              const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
                                              const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
                                              const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
                                              const double t,                //!< Current time instant (in seconds)
                                              const std::vector<double>& //!< Wave elevations at (x,y) in the NED frame (in meters)
                                             ) const
{
    return pimpl->orbital_velocities(x, y, z, t);
}

std::vector<FlatDiscreteDirectionalWaveSpectrum> SurfaceElevationFromGRPC::get_directional_spectra(const double x, const double y, const double t) const
{
    return pimpl->directional_spectra(x, y, t);
}

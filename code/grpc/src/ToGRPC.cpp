/*
 * ToGRPC.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: cady
 */


#include "ToGRPC.hpp"

ToGRPC::ToGRPC(const GRPCForceModel::Input& input_)
    : input(input_)
{}

RequiredWaveInformationRequest ToGRPC::from_required_wave_information(const double t, const double x, const double y, const double z) const
{
    RequiredWaveInformationRequest request;
    request.set_t(t);
    request.set_x(x);
    request.set_y(y);
    request.set_z(z);
    return request;
}

SpectrumResponse* ToGRPC::from_discrete_directional_wave_spectra(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra) const
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
        for (const auto& omega:spectrum.omega)
        {
            s->add_omega(omega);
        }
        for (const auto& psi:spectrum.psi)
        {
            s->add_psi(psi);
        }
        for (const auto& k:spectrum.k)
        {
            s->add_k(k);
        }
        for (const auto& phases:spectrum.phase)
        {
            const auto p = s->add_phase();
            for (const auto& phase:phases)
            {
                p->add_phase(phase);
            }
        }
    }
    return spectrum_response;
}

WaveInformation* ToGRPC::from_wave_information(const WaveRequest& wave_request, const double t, const EnvironmentAndFrames& env) const
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
                eta = env.w->get_and_check_wave_height(wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.t);
            }
            catch (const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the gRPC force model '" << input.name << "' which, indirectly, needs wave elevations (to compute the dynamic pressures). When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
            }
            const ssc::kinematics::PointMatrix orbital_velocities = env.w->get_and_check_orbital_velocity(env.g, wave_request.orbital_velocities.x, wave_request.orbital_velocities.y, wave_request.orbital_velocities.z, t, eta);

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
            copy_from_double_vector(env.w->get_and_check_wave_height(wave_request.elevations.x, wave_request.elevations.y, wave_request.elevations.t), wave_information->mutable_elevations()->mutable_z());
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
            const std::vector<double> eta = env.w->get_and_check_wave_height(wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.t);
            copy_from_double_vector(env.w->get_and_check_dynamic_pressure(env.rho, env.g, wave_request.dynamic_pressures.x, wave_request.dynamic_pressures.y, wave_request.dynamic_pressures.z, eta, t), wave_information->mutable_dynamic_pressures()->mutable_pdyn());
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

States* ToGRPC::from_state(const BodyStates& state, const double max_history_length, const EnvironmentAndFrames& env) const
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

ForceRequest ToGRPC::from_force_request(States* states, const std::map<std::string, double >& commands, WaveInformation* wave_information) const
{
    ForceRequest request;
    request.set_allocated_wave_information(wave_information);
    request.mutable_commands()->insert(commands.begin(), commands.end());
    request.set_allocated_states(states);
    return request;
}

SetForceParameterRequest ToGRPC::from_yaml(const std::string& yaml, const std::string body_name) const
{
    SetForceParameterRequest request;
    request.set_parameters(yaml);
    request.set_body_name(body_name);
    return request;
}

void ToGRPC::copy_from_double_vector(const std::vector<double>& origin, ::google::protobuf::RepeatedField< double >* destination) const
{
    *destination = {origin.begin(), origin.end()};
}

void ToGRPC::copy_from_string_vector(const std::vector<std::string>& origin, ::google::protobuf::RepeatedPtrField< std::string >* destination) const
{
    *destination = {origin.begin(), origin.end()};
}

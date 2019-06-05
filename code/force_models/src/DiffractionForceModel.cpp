/*
 * DiffractionForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include "DiffractionForceModel.hpp"

#include "Body.hpp"
#include "DiffractionInterpolator.hpp"
#include "HDBParser.hpp"
#include "InvalidInputException.hpp"
#include "SurfaceElevationInterface.hpp"
#include "yaml.h"
#include "external_data_structures_parsers.hpp"
#include "yaml2eigen.hpp"

#include <ssc/interpolation.hpp>
#include <ssc/text_file_reader.hpp>

#include <array>
#define TWOPI 6.283185307179586232

std::string DiffractionForceModel::model_name() { return "diffraction";}

HDBParser hdb_from_file(const std::string& filename);
HDBParser hdb_from_file(const std::string& filename)
{
    return HDBParser(ssc::text_file_reader::TextFileReader(filename).get_contents());
}

void check_all_omegas_are_within_bounds(const double min_bound, const std::vector<std::vector<double> >& vector_to_check, const double max_bound);
void check_all_omegas_are_within_bounds(const double min_bound, const std::vector<std::vector<double> >& vector_to_check, const double max_bound)
{
    const double eps = 0.01; // We don't care if we're above or below the bounds by 0.01 s: those are wave frequencies so not very precise.
    for (auto t:vector_to_check)
    {
        for (auto Tp:t)
        {
            if (Tp<(min_bound-eps))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "HDB used by DiffractionForceModel only defines the RAO for wave period Tp within [" << min_bound << "," << max_bound << "]"
                     << " s, but wave spectrum discretization contains Tp = " << Tp
                     << " s which is below the min bound by " << min_bound-Tp << " s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file ")
                ;
            }
            if (Tp>(max_bound+eps))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "HDB used by DiffractionForceModel only defines the RAO for wave period Tp within [" << min_bound << "," << max_bound << "]"
                     << " s, but wave spectrum discretization contains Tp = " << Tp
                     << " s which is above the max bound by " << Tp-max_bound << " s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file ")
                ;
            }
        }
    }
}

std::vector<std::vector<double> > convert_to_periods(std::vector<std::vector<double> > angular_frequencies);
std::vector<std::vector<double> > convert_to_periods(std::vector<std::vector<double> > angular_frequencies)
{
    for (auto& omegas:angular_frequencies)
    {
        for (auto& omega:omegas)
        {
            omega = TWOPI/omega;
        }
    }
    return angular_frequencies;
}


class DiffractionForceModel::Impl
{
    public:

        Impl(const YamlDiffraction& data, const EnvironmentAndFrames& env_, const HDBParser& hdb, const std::string& body_name)
          : initialized(false), env(env_),
        H0(data.calculation_point.x,data.calculation_point.y,data.calculation_point.z),
        rao(DiffractionInterpolator(hdb,std::vector<double>(),std::vector<double>(),data.mirror)),
        periods_for_each_direction(),
        psis()
        {
            if (env.w.use_count()>0)
            {
                // For each directional spectrum (i.e. for each direction), the wave angular frequencies the spectrum was discretized at.
                // periods[direction][omega]
                periods_for_each_direction = convert_to_periods(env.w->get_wave_angular_frequency_for_each_model());
                const auto hdb_periods = hdb.get_diffraction_module_periods();
                if (not(hdb_periods.empty()))
                {
                    check_all_omegas_are_within_bounds(hdb_periods.front(), periods_for_each_direction, hdb_periods.back());
                }
                psis = env.w->get_wave_directions_for_each_model();
            }
            else
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << DiffractionForceModel::model_name << "' needs a wave model, even if it's 'no waves'");
            }
            YamlPosition pos;
            pos.frame = DiffractionForceModel::model_name();
            pos.coordinates = data.calculation_point;
            const auto from_internal_frame_to_a_known_frame = make_transform(pos, body_name, env.rot);
            env.k->add(from_internal_frame_to_a_known_frame);
        }

        ssc::kinematics::Wrench evaluate(const ssc::kinematics::Point& G, const std::string& body_name, const double t, const double psi)
        {
            ssc::kinematics::Wrench ret;
            ssc::kinematics::Vector6d w;
            auto T = env.k->get("NED", body_name);
            T.swap();
            const ssc::kinematics::Point position_in_ned_for_the_wave_model = T*ssc::kinematics::Point(body_name,H0);
            ssc::kinematics::Point point_of_application_in_body_frame(body_name,H0);
            std::array<std::vector<std::vector<double> >, 6 > rao_modules;
            std::array<std::vector<std::vector<double> >, 6 > rao_phases;
            if (env.w.use_count()>0)
            {
                const size_t nb_of_directions = periods_for_each_direction.size();
                if (not(periods_for_each_direction.empty()))
                {
                    // Resize for each degree of freedom
                    for (size_t k = 0 ; k < 6 ; ++k)
                    {
                        rao_modules[k].resize(nb_of_directions);
                        rao_phases[k].resize(nb_of_directions);
                    }
                }
                for (size_t k = 0 ; k < 6 ; ++k) // For each degree of freedom (X, Y, Z, K, M, N)
                {
                    for (size_t i = 0 ; i < nb_of_directions ; ++i) // For each direction
                    {
                        rao_modules[k][i].resize(periods_for_each_direction[i].size());
                        rao_phases[k][i].resize(periods_for_each_direction[i].size());
                        for (size_t j = 0 ; j < psis[i].size() ; ++j) // For each incidence psi
                        {
                            // Wave incidence
                            const double beta = psi - psis.at(i).at(j);
                            // Interpolate RAO module for this axis k, period i and incidence j
                            rao_modules[k][i][j] = rao.interpolate_module(k, periods_for_each_direction[i][j], beta);
                            // Interpolate RAO phase for this axis k, period i and incidence j
                            rao_phases[k][i][j] = -rao.interpolate_phase(k, periods_for_each_direction[i][j], beta);
                        }
                    }

                    w((int)k) = env.w->evaluate_rao(position_in_ned_for_the_wave_model.x(),
                                                    position_in_ned_for_the_wave_model.y(),
                                                    t,
                                                    rao_modules[k],
                                                    rao_phases[k]);
                }
            }
            const auto ww = express_aquaplus_wrench_in_xdyn_coordinates(w);
            const Eigen::Vector3d force(ww(0),ww(1),ww(2));
            const Eigen::Vector3d torque(ww(3),ww(4),ww(5));
            const auto tau_in_internal_frame = ssc::kinematics::UnsafeWrench(ssc::kinematics::Point(DiffractionForceModel::model_name(), 0, 0, 0), force, torque);

            // Origin of the internal frame is P
            // G is the point (not the origin) of the body frame where the forces are summed
            // Ob is the origin of the body frame
            const auto OP = H0;
            const auto GO = -G.v;
            const auto GP = GO + OP;
            const Eigen::Vector3d force_in_G_expressed_in_body_frame = force;
            const Eigen::Vector3d torque_in_G_expressed_in_body_frame = torque+GP.cross(force);

            const ssc::kinematics::Point GG(body_name, G.x(), G.y(), G.z());
            const ssc::kinematics::UnsafeWrench tau_in_body_frame_at_G(GG, force_in_G_expressed_in_body_frame, torque_in_G_expressed_in_body_frame);
            return tau_in_body_frame_at_G;
        }

        ssc::kinematics::Vector6d express_aquaplus_wrench_in_xdyn_coordinates(ssc::kinematics::Vector6d v) const
        {
            v(0) *= -1;
            v(3) *= -1;
            return v;
        }

    private:
        Impl();
        bool initialized;
        EnvironmentAndFrames env;
        Eigen::Vector3d H0;
        DiffractionInterpolator rao;
        std::vector<std::vector<double> > periods_for_each_direction;
        std::vector<std::vector<double> > psis;

};

DiffractionForceModel::DiffractionForceModel(const YamlDiffraction& data, const std::string& body_name_, const EnvironmentAndFrames& env)
: ForceModel("diffraction", body_name_), pimpl(new Impl(data,env,hdb_from_file(data.hdb_filename), body_name_))
{
}

DiffractionForceModel::DiffractionForceModel(const Input& data, const std::string& body_name_, const EnvironmentAndFrames& env, const std::string& hdb_file_contents)
 : ForceModel("diffraction", body_name_), pimpl(new Impl(data,env,HDBParser(hdb_file_contents),body_name_))
{
}

ssc::kinematics::Wrench DiffractionForceModel::operator()(const BodyStates& states, const double t) const
{

    return pimpl->evaluate(states.G, states.name, t, states.get_angles().psi);
}

DiffractionForceModel::Input DiffractionForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDiffraction ret;
    node["hdb"]                             >> ret.hdb_filename;
    node["calculation point in body frame"] >> ret.calculation_point;
    node["mirror for 180 to 360"]           >> ret.mirror;
    return ret;
}

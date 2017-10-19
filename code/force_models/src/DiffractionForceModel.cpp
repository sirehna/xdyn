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

#include <ssc/interpolation.hpp>
#include <ssc/text_file_reader.hpp>

#include <array>

std::string DiffractionForceModel::model_name() { return "diffraction";}

HDBParser hdb_from_file(const std::string& filename);
HDBParser hdb_from_file(const std::string& filename)
{
    return HDBParser(ssc::text_file_reader::TextFileReader(filename).get_contents());
}


class DiffractionForceModel::Impl
{
    public:

        Impl(const YamlDiffraction& data, const EnvironmentAndFrames& env_, const HDBParser& hdb) : initialized(false), env(env_),
        H0(data.calculation_point.x,data.calculation_point.y,data.calculation_point.z),
        rao_module(),
        rao_phase()
        {
            if (env.w.use_count()>0)
            {
                std::stringstream ss;
                const auto omegas = env.w->get_wave_angular_frequency_for_each_model();
                const auto omegas_hdb = hdb.get_diffraction_module_omegas();
                for (auto o:omegas)
                {
                    for (auto omega:o)
                    {
                        if ((omega<omegas_hdb.front()) or (omega>omegas_hdb.back()))
                        {
                            THROW(__PRETTY_FUNCTION__, InvalidInputException,
                                    "HDB only defines the RAO for angular frequencies omega within [" << omegas_hdb.front() << "," << omegas_hdb.back() << "] "
                                 << " Rad/s, but wave spectrum discretization contains omega=" << omega
                                 << " Rad/s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file"
                            );
                        }
                    }
                }
                const auto psis = env.w->get_wave_directions_for_each_model();
                const size_t n = omegas.size();
                for (size_t i = 0 ; i < n ; ++i)
                {
                    DiffractionInterpolator radiation(hdb,omegas.at(i),psis.at(i),data.mirror);
                    for (size_t k = 0 ; k < 6 ; ++k)
                    {
                        rao_module.at(k).push_back(radiation.get_modules_flat(k));
                        rao_phase.at(k).push_back(radiation.get_phases_flat(k));
                    }
                }
            }
            else
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << DiffractionForceModel::model_name << "' needs a wave model, even if it's 'no waves'");
            }

        }

        ssc::kinematics::Wrench evaluate(const std::string& body_name, const double t)
        {
            ssc::kinematics::Wrench ret;
            ssc::kinematics::Vector6d w;
            const auto T = env.k->get(body_name,"NED");
            const ssc::kinematics::Point H = T*ssc::kinematics::Point(body_name,H0);
            for (size_t k = 0 ; k < 6 ; ++k)
            {
                if (env.w.use_count()>0)
                {
                    w((int)k) = env.w->evaluate_rao(H.x(),
                                                    H.y(),
                                                    t,
                                                    rao_module.at(k),
                                                    rao_phase.at(k)
                             );
                }
            }
            return ssc::kinematics::Wrench(H, w);
        }

    private:
        Impl();
        bool initialized;
        EnvironmentAndFrames env;
        Eigen::Vector3d H0;
        std::array<std::vector<std::vector<double> >, 6> rao_module;
        std::array<std::vector<std::vector<double> >, 6> rao_phase;

};

DiffractionForceModel::DiffractionForceModel(const YamlDiffraction& data, const std::string& body_name_, const EnvironmentAndFrames& env) : ForceModel("diffraction", body_name_), pimpl(new Impl(data,env,hdb_from_file(data.hdb_filename)))
{
}

DiffractionForceModel::DiffractionForceModel(const Input& data, const std::string& body_name_, const EnvironmentAndFrames& env, const std::string& hdb_file_contents) : ForceModel("diffraction", body_name_), pimpl(new Impl(data,env,HDBParser(hdb_file_contents)))
{
}

ssc::kinematics::Wrench DiffractionForceModel::operator()(const BodyStates& states, const double t) const
{

    return pimpl->evaluate(states.name, t);
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

/*
 * ResistanceCurveForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include <array>
#include <sstream>

#include <ssc/interpolation.hpp>
#include <ssc/text_file_reader.hpp>

#include "Body.hpp"
#include "DiffractionForceModel.hpp"
#include "DiffractionForceModelException.hpp"
#include "DiffractionInterpolator.hpp"
#include "HDBParser.hpp"
#include "SurfaceElevationInterface.hpp"
#include "YamlDiffraction.hpp"

/*
 *  bool initialized;
        EnvironmentAndFrames env;
        std::array<std::vector<std::vector<std::vector<double> > >,6> rao_module;
        std::array<std::vector<std::vector<std::vector<double> > >,6> rao_phase;
 */

class DiffractionForceModel::Impl
{
    public:
        Impl(const YamlDiffraction& data, const EnvironmentAndFrames& env_) : initialized(false), env(env_),
        H0(data.calculation_point.x,data.calculation_point.y,data.calculation_point.z),
        rao_module(),
        rao_phase()
        {
            if (env.w.use_count()>0)
            {
                std::stringstream ss;
                const HDBParser hdb(ssc::text_file_reader::TextFileReader(data.hdb_filename).get_contents());
                const auto omegas = env.w->get_wave_angular_frequency_for_each_model();
                const auto omegas_hdb = hdb.get_diffraction_module_omegas();
                for (auto o:omegas)
                {
                    for (auto omega:o)
                    {
                        if ((omega<omegas_hdb.front()) or (omega>omegas_hdb.back()))
                        {
                            ss << "HDB only defines the RAO for angular frequencies omega within [" << omegas_hdb.front() << "," << omegas_hdb.back() << "] "
                               << " Rad/s, but wave spectrum discretization contains omega=" << omega
                               << " Rad/s: you need to modify the section 'environment models/model: waves/discretization' in the YAML file or the 'spectrum' section or change the HDB file";
                            THROW(__PRETTY_FUNCTION__, DiffractionForceModelException, ss.str());
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
                        rao_module.at(k).push_back(radiation.get_modules(k));
                        rao_phase.at(k).push_back(radiation.get_phases(k));
                    }
                }
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
        std::array<std::vector<std::vector<std::vector<double> > >,6> rao_module;
        std::array<std::vector<std::vector<std::vector<double> > >,6> rao_phase;

};

DiffractionForceModel::DiffractionForceModel(const YamlDiffraction& data, const EnvironmentAndFrames& env) : ForceModel("diffraction"), pimpl(new Impl(data,env))
{
}

ssc::kinematics::Wrench DiffractionForceModel::operator()(const Body& body, const double t) const
{

    return pimpl->evaluate(body.name, t);
}

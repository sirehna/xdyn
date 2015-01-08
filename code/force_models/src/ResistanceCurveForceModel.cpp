/*
 * ResistanceCurveForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include <ssc/interpolation.hpp>

#include "yaml.h"
#include "parse_unit_value.hpp"
#include "environment_parsers.hpp"
#include "Body.hpp"
#include "ResistanceCurveForceModel.hpp"

const std::string ResistanceCurveForceModel::model_name = "resistance curve";

class ResistanceCurveForceModel::Impl
{
    public:
        Impl(const Yaml& data) : S(data.Va, data.R, /*allow queries outside bounds*/ true), vmin(data.Va.front()), vmax(data.Va.back())
        {
        }

        double get_resistance(const double Va)
        {
            if (Va<vmin) std::cerr << "Warning: resistance curve is tabulated up to " << vmin << " m/s, but received Va = " << Va << " m/s" << std::endl;
            if (Va>vmax) std::cerr << "Warning: resistance curve is tabulated up to " << vmax << " m/s, but received Va = " << Va << " m/s" << std::endl;
            return S.f(Va);
        }

    private:
        Impl();
        ssc::interpolation::SplineVariableStep S;
        double vmin;
        double vmax;
};

ResistanceCurveForceModel::Yaml::Yaml() : Va(), R()
{
}

ResistanceCurveForceModel::ResistanceCurveForceModel(const Yaml& data, const EnvironmentAndFrames&) : ForceModel("resistance curve"), pimpl(new Impl(data))
{
}

ResistanceCurveForceModel::Yaml ResistanceCurveForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    parse_uv(node["speed"], ret.Va);
    parse_uv(node["resistance"], ret.R);
    return ret;
}

ssc::kinematics::Wrench ResistanceCurveForceModel::operator()(const Body& body, const double ) const
{
    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    tau(0) = -pimpl->get_resistance(body.u);
    return ssc::kinematics::Wrench(body.hydrodynamic_forces_calculation_point, tau);
}

/*
 * ResistanceCurveForceModel.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#include <ssc/interpolation.hpp>

#include "Body.hpp"
#include "ResistanceCurveForceModel.hpp"
#include "YamlResistanceCurve.hpp"

class ResistanceCurveForceModel::Impl
{
    public:
        Impl(const YamlResistanceCurve& data) : S(data.Va, data.R, /*allow queries outside bounds*/ true), vmin(data.Va.front()), vmax(data.Va.back())
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

ResistanceCurveForceModel::ResistanceCurveForceModel(const YamlResistanceCurve& data) : ForceModel("resistance curve"), pimpl(new Impl(data))
{
}

ssc::kinematics::Wrench ResistanceCurveForceModel::operator()(const Body& body, const double ) const
{
    ssc::kinematics::Vector6d tau = ssc::kinematics::Vector6d::Zero();
    tau(0) = -pimpl->get_resistance(body.u);
    return ssc::kinematics::Wrench(body.hydrodynamic_forces_calculation_point, tau);
}

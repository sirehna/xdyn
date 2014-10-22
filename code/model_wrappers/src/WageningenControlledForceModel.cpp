/*
 * WageningenControlledForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "WageningenControlledForceModel.hpp"
#include "YamlWageningen.hpp"

WageningenControlledForceModel::WageningenControlledForceModel(const YamlWageningen& input) : ControllableForceModel(input.name,{"rpm","P/D"},input.position_of_propeller_frame),
            w(input.wake_coefficient),
            eta_R(input.relative_rotative_efficiency),
            t(input.thrust_deduction_factor),
            kappa(input.rotating_clockwise ? -1 : 1),
            Z(input.number_of_blades),
            AE_A0(input.blade_area_ratio)
{
}

ssc::kinematics::Wrench WageningenControlledForceModel::get_force(const Body& , const double , std::map<std::string,double> ) const
{
    return ssc::kinematics::Wrench();
}

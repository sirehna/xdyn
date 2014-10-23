/*
 * WageningenControlledForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include <cmath>

#include "WageningenControlledForceModel.hpp"
#include "WageningenControlledForceModelException.hpp"
#include "YamlWageningen.hpp"

WageningenControlledForceModel::WageningenControlledForceModel(const YamlWageningen& input) : ControllableForceModel(input.name,{"rpm","P/D"},input.position_of_propeller_frame),
            w(input.wake_coefficient),
            eta_R(input.relative_rotative_efficiency),
            t(input.thrust_deduction_factor),
            kappa(input.rotating_clockwise ? -1 : 1),
            Z(input.number_of_blades),
            AE_A0(input.blade_area_ratio),
            ct{0.00880496,-0.204554,0.166351,0.158114,-0.147581,-0.481497,0.415437,0.0144043,-0.0530054,0.0143481,0.0606826,-0.0125894,0.0109689,-0.133698,0.00638407,-0.00132718,0.168496,-0.0507214,0.0854559,-0.0504475,0.010465,-0.00648272,-0.00841728,0.0168424,-0.00102296,-0.0317791,0.018604,-0.00410798,-0.000606848,-0.0049819,0.0025983,-0.000560528,-0.00163652,-0.000328787,0.000116502,0.000690904,0.00421749,0.0000565229,-0.00146564},
            st{0,1,0,0,2,1,0,0,2,0,1,0,1,0,0,2,3,0,2,3,1,2,0,1,3,0,1,0,0,1,2,3,1,1,2,0,0,3,0},
            tt{0,0,1,2,0,1,2,0,0,1,1,0,0,3,6,6,0,0,0,0,6,6,3,3,3,3,0,2,0,0,0,0,2,6,6,0,3,6,3},
            ut{0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,2,2,2,2,2,0,0,0,1,2,2,0,0,0,0,0,0,0,1,1,1,2},
            vt{0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2}
{
    std::stringstream ss;
    if ((Z<2) or (Z>7))
    {
        ss << "Invalid number of blades Z received: expected 2 <= Z <= 7 but got Z=" << Z;
        THROW(__PRETTY_FUNCTION__, WageningenControlledForceModelException, ss.str());
    }
    if ((AE_A0<0.3) or (AE_A0>1.05))
    {
        ss << "Invalid number of blade area ratio AE_A0 received: expected 0.3 <= AE_A0 <= 1.05 but got AE_A0=" << AE_A0;
        THROW(__PRETTY_FUNCTION__, WageningenControlledForceModelException, ss.str());
    }
}

ssc::kinematics::Wrench WageningenControlledForceModel::get_force(const Body& , const double , std::map<std::string,double> ) const
{
    return ssc::kinematics::Wrench();
}

void WageningenControlledForceModel::check(const double P_D, const double J) const
{
    if ((P_D<0.5) or (P_D>1.4))
    {
        std::stringstream ss;
        ss << "Invalid pitch/diameter ratio P/D received: expected 0.5 <= P/D <= 1.4 but got P_D=" << P_D;
        THROW(__PRETTY_FUNCTION__, WageningenControlledForceModelException, ss.str());
    }
    if ((J<0) or (J>1.5))
    {
        std::stringstream ss;
        ss << "Invalid advance ratio J received: expected 0 <= J <= 1.5 but got J=" << J;
        THROW(__PRETTY_FUNCTION__, WageningenControlledForceModelException, ss.str());
    }
}

double WageningenControlledForceModel::Kt(const size_t Z, const double AE_A0_, const double P_D, const double J) const
{
    check(P_D, J);
    double kt = 0;
    for (size_t i = 0 ; i < NB_COEFF_KT ; ++i)
    {
        kt += ct[i]*std::pow(J, st[i])*std::pow(P_D, tt[i])*std::pow(AE_A0_, ut[i])*std::pow(Z,vt[i]);
    }
    return kt;
}

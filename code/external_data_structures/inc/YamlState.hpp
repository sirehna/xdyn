#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_

#include <map>
#include <vector>
#include "AbstractStates.hpp"


struct YamlState : AbstractStates<double>
{
    YamlState(const double t
             ,const double x_
             ,const double y_
             ,const double z_
             ,const double u_
             ,const double v_
             ,const double w_
             ,const double p_
             ,const double q_
             ,const double r_
             ,const double qr_
             ,const double qi_
             ,const double qj_
             ,const double qk_
            );
    YamlState();
    double t;
    double phi;
    double theta;
    double psi;
    std::map<std::string, double> extra_observations; //!< extra outputs from force models. Corresponds to 'extra_observations' in code/grpc/force.proto#L99
};

bool operator==(const YamlState& state1, const YamlState& state2);

#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_ */


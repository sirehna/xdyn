#include "YamlState.hpp"

YamlState::YamlState()
    : AbstractStates<double>()
    , t(), phi(), theta(), psi(), extra_observations()
{
}

YamlState::YamlState(const double t_
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
       ) :
        AbstractStates<double>(x_,y_,z_,u_,v_,w_,p_,q_,r_,qr_,qi_,qj_,qk_)
        , t(t_), phi(), theta(), psi(), extra_observations()
{}

bool operator==(const YamlState& lhs, const YamlState& rhs)
{
    return ((AbstractStates<double>)lhs == (AbstractStates<double>)rhs)
            and (lhs.t == rhs.t)
            and (lhs.phi == rhs.phi)
            and (lhs.theta == rhs.theta)
            and (lhs.psi == rhs.psi);
}


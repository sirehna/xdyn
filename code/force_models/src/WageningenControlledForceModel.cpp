/*
 * WageningenControlledForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "WageningenControlledForceModel.hpp"

#include "Body.hpp"
#include "external_data_structures_parsers.hpp"
#include "InvalidInputException.hpp"

#include <ssc/yaml_parser.hpp>

#include "yaml.h"

std::string WageningenControlledForceModel::model_name() {return "wageningen B-series";}


WageningenControlledForceModel::Yaml::Yaml() :
        AbstractWageningen::Yaml(),
        number_of_blades(),
        blade_area_ratio()
{
}

WageningenControlledForceModel::Yaml::Yaml(const AbstractWageningen::Yaml& y) :
                AbstractWageningen::Yaml(y),
                number_of_blades(),
                blade_area_ratio()
{
}

double WageningenControlledForceModel::get_Kt(const std::map<std::string,double>& commands, const double J) const
{
    const double P_D = commands.at("P/D");
    return Kt(Z, AE_A0, P_D, J);
}

double WageningenControlledForceModel::get_Kq(const std::map<std::string,double>& commands, const double J) const
{
    const double P_D = commands.at("P/D");
    return Kq(Z, AE_A0, P_D, J);
}

WageningenControlledForceModel::WageningenControlledForceModel(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
            AbstractWageningen(input, body_name_, env_),
            Z(input.number_of_blades),
            AE_A0(input.blade_area_ratio),
            ct{0.00880496,-0.204554,0.166351,0.158114,-0.147581,-0.481497,0.415437,0.0144043,-0.0530054,0.0143481,0.0606826,-0.0125894,0.0109689,-0.133698,0.00638407,-0.00132718,0.168496,-0.0507214,0.0854559,-0.0504475,0.010465,-0.00648272,-0.00841728,0.0168424,-0.00102296,-0.0317791,0.018604,-0.00410798,-0.000606848,-0.0049819,0.0025983,-0.000560528,-0.00163652,-0.000328787,0.000116502,0.000690904,0.00421749,0.0000565229,-0.00146564},
            st{0,1,0,0,2,1,0,0,2,0,1,0,1,0,0,2,3,0,2,3,1,2,0,1,3,0,1,0,0,1,2,3,1,1,2,0,0,3,0},
            tt{0,0,1,2,0,1,2,0,0,1,1,0,0,3,6,6,0,0,0,0,6,6,3,3,3,3,0,2,0,0,0,0,2,6,6,0,3,6,3},
            ut{0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,1,2,2,2,2,2,0,0,0,1,2,2,0,0,0,0,0,0,0,1,1,1,2},
            vt{0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2},
            cq{0.00379368,0.00886523,-0.032241,0.00344778,-0.0408811,-0.108009,-0.0885381,0.188561,-0.00370871,0.00513696,0.0209449,0.00474319,-0.00723408,0.00438388,-0.0269403,0.0558082,0.0161886,0.00318086,0.015896,0.0471729,0.0196283,-0.0502782,-0.030055,0.0417122,-0.0397722,-0.00350024,-0.0106854,0.00110903,-0.000313912,0.0035985,-0.00142121,-0.00383637,0.0126803,-0.00318278,0.00334268,-0.00183491,0.000112451,-0.0000297228,0.000269551,0.00083265,0.00155334,0.000302683,-0.0001843,-0.000425399,0.0000869243,-0.0004659,0.0000554194},
            sq{0,2,1,0,0,1,2,0,1,0,1,2,2,1,0,3,0,1,0,1,3,0,3,2,0,0,3,3,0,3,0,1,0,2,0,1,3,3,1,2,0,0,0,0,3,0,1},
            tq{0,0,1,2,1,1,1,2,0,1,1,1,0,1,2,0,3,3,0,0,0,1,1,2,3,6,0,3,6,0,6,0,2,3,6,1,2,6,0,0,2,6,0,3,3,6,6},
            uq{0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,0,0,0,1,1,2,2,2,2,0,0,0,1,1,1,1,2,2,2,2,2},
            vq{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2}
{
    commands.push_back("P/D");
    if ((Z<2) or (Z>7))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid number of blades Z received: expected 2 <= Z <= 7 but got Z=" << Z);
    }
    if ((AE_A0<0.3) or (AE_A0>1.05))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Invalid number of blade area ratio AE_A0 received: expected 0.3 <= AE_A0 <= 1.05 but got AE_A0=" << AE_A0);
    }
}

void WageningenControlledForceModel::check(const double P_D, const double J) const
{
    if ((P_D<0.5) or (P_D>1.4))
    {
        std::cerr << "Invalid pitch/diameter ratio P/D received: expected 0.5 <= P/D <= 1.4 but got P_D=" << P_D << std::endl;
    }
    if ((J<0) or (J>1.5))
    {
        std::cerr << "Warning: Wageningen model used outside of its domain. Maybe n is too small? Invalid advance ratio J: expected 0 <= J <= 1.5 but got J=" << J << ". ";
    }
    if (J<0)
    {
        std::cerr <<"Saturating at 0 to continue simulation." << std::endl;
    }
    if (J>1.5)
    {
        std::cerr <<"Saturating at 1.5 to continue simulation." << std::endl;
    }
}

double saturate(const double J);
double saturate(const double J)
{
    return std::max(std::min(J,1.5),0.);
}

double WageningenControlledForceModel::Kt(const size_t Z, const double AE_A0_, const double P_D, const double J) const
{
    check(P_D, J);
    double kt = 0;
    const double saturated_J = saturate(J);
    for (size_t i = 0 ; i < NB_COEFF_KT ; ++i)
    {
        kt += ct[i]*std::pow(saturated_J, st[i])*std::pow(P_D, tt[i])*std::pow(AE_A0_, ut[i])*std::pow(Z,vt[i]);
    }
    return kt;
}

double WageningenControlledForceModel::Kq(const size_t Z, const double AE_A0_, const double P_D, const double J) const
{
    check(P_D, J);
    double kq = 0;
    const double saturated_J = saturate(J);
    for (size_t i = 0 ; i < NB_COEFF_KQ ; ++i)
    {
        kq += cq[i]*std::pow(saturated_J, sq[i])*std::pow(P_D, tq[i])*std::pow(AE_A0_, uq[i])*std::pow(Z,vq[i]);
    }
    return kq;
}

WageningenControlledForceModel::Yaml WageningenControlledForceModel::parse(const std::string& yaml)
{
    WageningenControlledForceModel::Yaml ret(AbstractWageningen::parse(yaml));
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    node["blade area ratio AE/A0"] >> ret.blade_area_ratio;
    ret.number_of_blades = try_to_parse_positive_integer(node,"number of blades");
    return ret;
}


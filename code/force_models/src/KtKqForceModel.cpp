/*
 * KtKqForceModel.cpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */
#include <ssc/yaml_parser.hpp>
#include <ssc/interpolation.hpp>
#include "external_data_structures_parsers.hpp"

#include "KtKqForceModel.hpp"
#include "yaml.h"

std::string KtKqForceModel::model_name() {return "Kt(J) & Kq(J)";}

class KtKqForceModel::Impl
{
    public:
        Impl(const std::vector<double>&J, const std::vector<double>& Kt_, const std::vector<double>& Kq_) :
            Kt(J, Kt_),
            Kq(J, Kq_)
        {
        }

        ssc::interpolation::SplineVariableStep Kt;
        ssc::interpolation::SplineVariableStep Kq;

    private:
        Impl();
};

KtKqForceModel::Yaml::Yaml() :
            AbstractWageningen::Yaml(),
            J(),
            Kt(),
            Kq()
{
}

KtKqForceModel::Yaml::Yaml(const AbstractWageningen::Yaml& y) :
        AbstractWageningen::Yaml(y),
        J(),
        Kt(),
        Kq()
{
}

KtKqForceModel::Yaml KtKqForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret = AbstractWageningen::parse(yaml);;
    node["J"]  >> ret.J;
    node["Kt"] >> ret.Kt;
    node["Kq"] >> ret.Kq;

    return ret;
}

KtKqForceModel::KtKqForceModel(const Yaml& input, const std::string& body_name_, const EnvironmentAndFrames& env_) :
            AbstractWageningen(input, body_name_, env_), pimpl(new Impl(input.J, input.Kt, input.Kq))
{
}

double KtKqForceModel::get_Kt(const double , const double J) const
{
    return pimpl->Kt.f(J);
}

double KtKqForceModel::get_Kq(const double P_D, const double J) const
{
    return pimpl->Kq.f(J);
}

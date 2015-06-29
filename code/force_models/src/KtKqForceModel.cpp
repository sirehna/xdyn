/*
 * KtKqForceModel.cpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */
#include <ssc/yaml_parser.hpp>
#include "external_data_structures_parsers.hpp"

#include "KtKqForceModel.hpp"
#include "yaml.h"

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

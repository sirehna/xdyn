/*
 * SimpleTrackKeepingController.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#include "yaml.h"

#include "external_data_structures_parsers.hpp"
#include "parse_unit_value.hpp"
#include "SimpleTrackKeepingController.hpp"

SimpleTrackKeepingController::Yaml::Yaml() : name(), ksi(0), Tp(0)
{
}

SimpleTrackKeepingController::Yaml SimpleTrackKeepingController::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    node["name"] >> ret.name;
    node["ksi"] >> ret.ksi;
    parse_uv(node["Tp"], ret.Tp);
    return ret;
}

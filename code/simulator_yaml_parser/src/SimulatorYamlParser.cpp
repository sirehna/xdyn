/*
 * SimulatorYamlParser.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "SimulatorYamlParser.hpp"
#include "yaml.h"
#include "DecodeUnit.h"
#include "SimulatorYamlParserException.hpp"

struct UV
{
    UV() : value(0), unit("") {}
    double value;
    std::string unit;
};

void operator >> (const YAML::Node& node, YamlRotation& g);
void operator >> (const YAML::Node& node, UV& g);
void operator >> (const YAML::Node& node, YamlBody& b);
void operator >> (const YAML::Node& node, YamlModel& m);
void operator >> (const YAML::Node& node, YamlPosition& m);
void operator >> (const YAML::Node& node, YamlAngle& a);
void operator >> (const YAML::Node& node, YamlCoordinates& c);

double decode(const UV& uv);

SimulatorYamlParser::SimulatorYamlParser(const std::string& data) : YamlParser(data), node(new YAML::Node())
{
    convert_stream_to_yaml_node(contents, *node);
    if (node->size() == 0)
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Something is wrong with the YAML data: no YAML nodes were detected by the YAML parser.");
    }
}

YamlSimulatorInput SimulatorYamlParser::parse()
{
    YamlSimulatorInput ret;
    (*node)["bodies"] >> ret.bodies;
    (*node)["rotations"] >> ret.rotations;
    (*node)["environment"] >> ret.environment;
    return ret;
}

void operator >> (const YAML::Node& node, YamlRotation& g)
{
    node["convention"] >> g.convention;
    node["order by"] >> g.order_by;
}

void operator >> (const YAML::Node& node, UV& g)
{
    node["unit"] >> g.unit;
    node["value"] >> g.value;
}

double decode(const UV& uv)
{
    return uv.value * DecodeUnit::decodeUnit(uv.unit);
}

void operator >> (const YAML::Node& node, YamlBody& b)
{
    node["name"] >> b.name;
    node["mesh"] >> b.mesh;
    node["external forces"] >> b.external_forces;
    node["position of body frame relative to mesh"] >> b.position_of_body_frame_relative_to_mesh;
    node["initial position of body frame relative to NED"] >> b.initial_position_of_body_frame_relative_to_NED_projected_in_NED;
}

void operator >> (const YAML::Node& node, YamlModel& m)
{
    node["model"] >> m.model;
    YAML::Emitter out;
    out << node;
    m.yaml = out.c_str();
}

void operator >> (const YAML::Node& node, YamlPosition& p)
{
    node >> p.angle;
    node >> p.coordinates;
    node["frame"] >> p.frame;
}

void operator >> (const YAML::Node& node, YamlAngle& a)
{
    UV uv;
    node["phi"] >> uv; a.phi = decode(uv);
    node["theta"] >> uv; a.theta = decode(uv);
    node["psi"] >> uv; a.psi = decode(uv);
}
void operator >> (const YAML::Node& node, YamlCoordinates& c)
{
    UV uv;
    node["x"] >> uv; c.x = decode(uv);
    node["y"] >> uv; c.y = decode(uv);
    node["z"] >> uv; c.z = decode(uv);
}

#include "yaml.h"
#include "YamlState.hpp"
#include "parse_history.hpp"
#include <vector>
#include "InvalidInputException.hpp"
#include "external_data_structures_parsers.hpp"

void operator >> (const YAML::Node& node, YamlState& s);
void operator >> (const YAML::Node& node, YamlState& s)
{
    node["t"] >> s.t;
    node["x"] >> s.x;
    node["y"] >> s.y;
    node["z"] >> s.z;
    node["u"] >> s.u;
    node["v"] >> s.v;
    node["w"] >> s.w;
    node["p"] >> s.p;
    node["q"] >> s.q;
    node["r"] >> s.r;
    node["qr"] >> s.qr;
    node["qi"] >> s.qi;
    node["qj"] >> s.qj;
    node["qk"] >> s.qk;
}

void operator << (YAML::Emitter& out, const YamlState& state);
void operator << (YAML::Emitter& out, const YamlState& state)
{
    out<<YAML::Flow;
    out<<YAML::BeginMap;
    out<<YAML::Key<<"t"<<YAML::Value<<state.t;
    out<<YAML::Key<<"x"<<YAML::Value<<state.x;
    out<<YAML::Key<<"y"<<YAML::Value<<state.y;
    out<<YAML::Key<<"z"<<YAML::Value<<state.z;
    out<<YAML::Key<<"u"<<YAML::Value<<state.u;
    out<<YAML::Key<<"v"<<YAML::Value<<state.v;
    out<<YAML::Key<<"w"<<YAML::Value<<state.w;
    out<<YAML::Key<<"p"<<YAML::Value<<state.p;
    out<<YAML::Key<<"q"<<YAML::Value<<state.q;
    out<<YAML::Key<<"r"<<YAML::Value<<state.r;
    out<<YAML::Key<<"qr"<<YAML::Value<<state.qr;
    out<<YAML::Key<<"qi"<<YAML::Value<<state.qi;
    out<<YAML::Key<<"qj"<<YAML::Value<<state.qj;
    out<<YAML::Key<<"qk"<<YAML::Value<<state.qk;
    out<<YAML::EndMap;
}

void operator>> (const YAML::Node& node, YamlSimServerInputs& infos);
void operator>> (const YAML::Node& node, YamlSimServerInputs& infos)
{
    infos.Dt = 0;
    try_to_parse(node, "Dt", infos.Dt);
    node["states"]   >> infos.states;
    node["commands"] >> infos.commands;
}

YamlSimServerInputs decode_YamlSimServerInputs(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlSimServerInputs infos;
    node >> infos;
    return infos;
}

std::string encode_YamlStates(const std::vector<YamlState>& states)
{
    YAML::Emitter e;
    e << states;
    return e.c_str();
}

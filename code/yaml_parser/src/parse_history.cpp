#include <ssc/json.hpp>
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

YamlSimServerInputs decode_YamlSimServerInputs(const std::string& json)
{
    rapidjson::Document document;
    ssc::json::parse(json, document);
    YamlSimServerInputs infos;
    infos.Dt = ssc::json::find_optional_double("Dt", document, 0);
    if (not(document.HasMember("states")))
    {
        THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Missing key 'states' in JSON root.")
    }
    if (not(document["states"].IsArray()))
    {
      THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "Expecting a JSON array but got '" << ssc::json::dump(document["states"]));
    }
    for (rapidjson::Value& v:document["states"].GetArray())
    {
        YamlState s;
        s.t = ssc::json::find_double("t", v);
        s.x = ssc::json::find_double("x", v);
        s.y = ssc::json::find_double("y", v);
        s.z = ssc::json::find_double("z", v);
        s.u = ssc::json::find_double("u", v);
        s.v = ssc::json::find_double("v", v);
        s.w = ssc::json::find_double("w", v);
        s.p = ssc::json::find_double("p", v);
        s.q = ssc::json::find_double("q", v);
        s.r = ssc::json::find_double("r", v);
        s.qr = ssc::json::find_double("qr", v);
        s.qi = ssc::json::find_double("qi", v);
        s.qj = ssc::json::find_double("qj", v);
        s.qk = ssc::json::find_double("qk", v);
        infos.states.push_back(s);
    }
    if (document.HasMember("commands"))
    {
        const rapidjson::Value& commands = document["commands"];
        if (not(commands.IsObject()) && not(commands.IsNull()))
        {
            THROW(__PRETTY_FUNCTION__, ssc::json::Exception, "'commands' should be a JSON object (key-values): got " << ssc::json::print_type(commands))
        }
        else
        {
            if (not(commands.IsNull()))
            {
                for (rapidjson::Value::ConstMemberIterator it = commands.MemberBegin(); it != commands.MemberEnd(); ++it)
                {
                    infos.commands[it->name.GetString()] = ssc::json::find_double(it->name.GetString(), commands);
                }
            }
        }
    }

    return infos;
}

std::string encode_YamlStates(const std::vector<YamlState>& states)
{
    YAML::Emitter e;
    e << states;
    return e.c_str();
}

#include "yaml.h"
#include "YamlState.hpp"
#include "parse_history.hpp"
#include <vector>
#include "InvalidInputException.hpp"


void operator >> (const YAML::Node& node, std::pair<double,double>& v);
void operator >> (const YAML::Node& node, std::pair<double,double>& v)
{
    if (node.size() != 2)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "dimension error : in yaml history, sublist size must be 2");

    }
    node[0] >> v.first;
    node[1] >> v.second;
}

void operator >> (const YAML::Node& node, std::vector<std::pair<double,double>>& v);
void operator >> (const YAML::Node& node, std::vector<std::pair<double,double>>& v)
{
    v.resize(node.size());
    for (size_t i = 0 ; i< node.size() ; ++i) node[i] >> v[i];
}

void operator >> (const YAML::Node& node, YamlHistory& h);
void operator >> (const YAML::Node& node, YamlHistory& h)
{
    std::vector<std::pair<double,double>> valeur;
    node>>valeur;
    for(auto i=valeur.begin();i!=valeur.end(); i++)
    {
        h.tau.push_back(i->first);
        h.values.push_back(i->second);
    }

}

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



YamlState parse_history_yaml(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlState ret;
    node >> ret;
    return ret;
}


void operator << (YAML::Emitter& out, std::pair<double,double>& p);
void operator << (YAML::Emitter& out, std::pair<double,double>& p)
{
    out<<YAML::Flow;
    out<<YAML::BeginSeq<<p.first<<p.second<<YAML::EndSeq;
}

void operator << (YAML::Emitter& out, std::vector<std::pair<double,double>>& v);
void operator << (YAML::Emitter& out, std::vector<std::pair<double,double>>& v)
{
    out<<YAML::BeginSeq;
    for(size_t i=0 ; i<v.size(); i++)
    {
        out<<v[i];
    }
    out<<YAML::EndSeq;

}

void operator << (std::vector<std::pair<double,double>>& lists, const YamlHistory& h);
void operator << (std::vector<std::pair<double,double>>& lists, const YamlHistory& h)
{
    for(size_t it=0; it<h.values.size();it++)
    {
        std::pair<double,double> p(h.tau[it], h.values[it]);
        lists.push_back(p);
    }
}

void operator << (YAML::Emitter& out, const YamlHistory& h);
void operator << (YAML::Emitter& out, const YamlHistory& h)
{
    std::vector<std::pair<double,double>> lists;
    lists<<h;

    out<<YAML::BeginSeq;
    for(size_t it=0; it<lists.size();it++)
    {
        out<<lists[it];
    }
    out<<YAML::EndSeq;
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


std::string generate_history_yaml(const YamlState& state)
{
    YAML::Emitter e;
    e<<state;
    return e.c_str();
}

void operator>> (const YAML::Node& node, YamlSimStepperInfo& infos);
void operator>> (const YAML::Node& node, YamlSimStepperInfo& infos)
{
    node["states"]   >> infos.state;
    node["commands"] >> infos.commands;

}

YamlSimStepperInfo get_yamlsimstepperinfo(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlSimStepperInfo infos;
    node>>infos;
    return infos;


}






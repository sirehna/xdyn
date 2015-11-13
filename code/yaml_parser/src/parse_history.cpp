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
    //

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

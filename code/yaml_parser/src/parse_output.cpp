/*
 * parse_output.cpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */

#include "yaml.h"
#include "parse_output.hpp"

void operator >> (const YAML::Node& node, YamlOutput& f);
void operator >> (const YAML::Node& node, YamlOutput& f)
{
    node["filename"] >> f.filename;
    node["format"]   >> f.format;
    node["data"]     >> f.data;
}

std::vector<YamlOutput> parse_output(const std::string yaml)
{
    std::vector<YamlOutput> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    try
    {
        node["output"]         >> ret;
    }
    catch(std::exception& ) // Nothing to do: 'output' section is not mandatory
    {
    }
    return ret;
}

std::string customize(const std::string& var_name, const std::string& body_name);
std::string customize(const std::string& var_name, const std::string& body_name)
{
    return var_name + "(" + body_name + ")";
}

void fill(YamlOutput& out, const std::string& body_name);
void fill(YamlOutput& out, const std::string& body_name)
{
    out.data.push_back(customize("x", body_name));
    out.data.push_back(customize("y", body_name));
    out.data.push_back(customize("z", body_name));
    out.data.push_back(customize("u", body_name));
    out.data.push_back(customize("v", body_name));
    out.data.push_back(customize("w", body_name));
    out.data.push_back(customize("p", body_name));
    out.data.push_back(customize("q", body_name));
    out.data.push_back(customize("r", body_name));
    out.data.push_back(customize("qr", body_name));
    out.data.push_back(customize("qi", body_name));
    out.data.push_back(customize("qj", body_name));
    out.data.push_back(customize("qk", body_name));
}

std::vector<std::string> get_body_names(const std::string yaml);
std::vector<std::string> get_body_names(const std::string yaml)
{
    std::vector<std::string> out;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    const YAML::Node *parameter = node.FindValue("bodies");
    if (parameter)
    {
        for (size_t i = 0 ; i < parameter->size() ; ++i)
        {
            std::string name;
            (*parameter)[i]["name"] >> name;
            out.push_back(name);
        }
    }
    return out;
}

std::string get_format(const std::string& filename)
{
    const size_t n = filename.size();
    if (!n)                              return "tsv";
    if (filename.substr(n-3,3)==".h5")   return "hdf5";
    if (filename.substr(n-5,5)==".hdf5") return "hdf5";
    if (filename.substr(n-4,4)==".csv")  return "csv";
    if (filename.substr(n-4,4)==".tsv")  return "tsv";
    std::cerr << "Warning: could not recognize the format of specified output file '" << filename << "'";
                                         return "???";
}

YamlOutput generate_default_outputter_with_all_states_in_it(const std::string yaml, const std::string& filename)
{
    YamlOutput out;
    out.format = get_format(filename);
    out.filename = filename;
    out.data.push_back("t");
    const auto bodies = get_body_names(yaml);
    for (auto body:bodies) fill(out, body);
    return out;
}

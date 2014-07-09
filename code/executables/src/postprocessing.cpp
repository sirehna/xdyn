/*
 * postprocessing.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: cady
 */

#include <iostream>
#include <string>

#include "CSVFileReader.hpp"
#include "OutputTransformer.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include "TextFileReader.hpp"

std::string description();
std::string description()
{
    std::stringstream ss;
    ss << "This is the post-processing tool used to calculate derived quantities from the simulator's CSV output."
       << "It writes to the standard output." << std::endl
       << "(c) SIREHNA 2014." << std::endl
       << std::endl
       << "ID : @GIT_SHA1@" << std::endl
       << std::endl;
    return ss.str();
}

Res make(const std::vector<double>& res);
Res make(const std::vector<double>& res)
{
    Res ret;
    ret.t = res.front();
    for (size_t i = 1 ; i < res.size() ; ++i)
    {
        ret.x.push_back(res[i]);
    }
    return ret;
}

template <typename T> void write_val(std::ostream& os, const bool last, const T& val)
{
    os << val;
    if (not(last)) os << ",";
}

void write_title(std::ostream& os, const std::map<std::string,double>& res);
void write_title(std::ostream& os, const std::map<std::string,double>& res)
{
    const size_t n = res.size();
    size_t i = 0;
    for (auto that_var=res.begin() ; that_var != res.end() ; ++that_var)
    {
        write_val(os, i==n-1, that_var->first);
        i++;
    }
    if (i) os << std::endl;
}

void write_line(std::ostream& os, const std::map<std::string,double>& res);
void write_line(std::ostream& os, const std::map<std::string,double>& res)
{
    const size_t n = res.size();
    size_t i = 0;
    for (auto that_var=res.begin() ; that_var != res.end() ; ++that_var)
    {
        write_val(os, i==n-1, that_var->second);
        i++;
    }
    if (i) os << std::endl;
}

void write(std::ostream& os, const std::vector<std::map<std::string,double> >& res);
void write(std::ostream& os, const std::vector<std::map<std::string,double> >& res)
{
    if (not(res.empty()))
    {
        write_title(os, res.front());
        for (auto that_line=res.begin() ; that_line != res.end() ; ++that_line) write_line(os, *that_line);
    }
}

std::vector<std::map<std::string,double> > transpose(const std::map<std::string,std::vector<double> >& r);
std::vector<std::map<std::string,double> > transpose(const std::map<std::string,std::vector<double> >& r)
{
    std::vector<std::map<std::string,double> > ret(r.begin()->second.size(), std::map<std::string,double>());
    for (auto e=r.begin() ; e != r.end() ; ++e)
    {
        for (size_t i = 0 ; i < e->second.size() ; ++i)
        {
            ret[i][e->first] = e->second[i];
        }
    }
    return ret;
}

void append(std::map<std::string,double>& to, const std::map<std::string,double>& from);
void append(std::map<std::string,double>& to, const std::map<std::string,double>& from)
{
    for (auto m=from.begin() ; m != from.end() ; ++m) to[m->first] = m->second;
}

std::vector<std::map<std::string,double> > build_output(CSVFileReader& csv_file, const OutputTransformer& transform);
std::vector<std::map<std::string,double> > build_output(CSVFileReader& csv_file, const OutputTransformer& transform)
{
    const auto csv = transpose(csv_file.get_map());
    std::vector<std::map<std::string,double> > res;
    size_t i = 0;
    while(csv_file.has_more_data())
    {
        auto new_data = transform(make(csv_file.get_line()));
        append(new_data, csv[i]);
        ++i;
        res.push_back(new_data);
    }
    return res;
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << description() << std::endl << "Usage: " << argv[0] << " <csv file> <yaml file>" << std::endl;
        return 1;
    }

    const TextFileReader yaml_file(std::vector<std::string>(1, std::string(argv[2])));
    const auto yaml = SimulatorYamlParser(yaml_file.get_contents()).parse();

    const size_t nb_of_columns = 1+yaml.bodies.size()*13;
    CSVFileReader csv(argv[1],nb_of_columns);
    SimulatorBuilder builder(yaml);
    const OutputTransformer transform(builder);

    write(std::cout, build_output(csv, transform));
    return 0;
}

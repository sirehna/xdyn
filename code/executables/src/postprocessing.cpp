
/*
 * postprocessing.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: cady
 */

#include <iostream>
#include <string>
#include <iterator>     // std::istream_iterator

#include <ssc/csv_file_reader.hpp>
#include "OutputTransformer.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include <ssc/text_file_reader.hpp>

#include "simulator_api.hpp"

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

std::vector<std::map<std::string,double> > build_output(ssc::csv_file_reader::CSVFileReader& csv_file, OutputTransformer& transform);
std::vector<std::map<std::string,double> > build_output(ssc::csv_file_reader::CSVFileReader& csv_file, OutputTransformer& transform)
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

std::string get_contents_from_stdin();
std::string get_contents_from_stdin()
{
    std::cin.sync_with_stdio(false); // As per http://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python
    std::cin >> std::noskipws; // Don't skip the whitespace while reading

    // Use stream iterators to copy the stream to a string
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    return std::string(it, end);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << description() << std::endl << "Usage: " << argv[0] << " <yaml files>" << std::endl
                  << "The yaml files need to have an 'output' section in them." << std::endl
                  << "The CSV file is read from the standard output & corresponds to the output of the simulator so the executables can be piped." << std::endl;
        return 1;
    }

    const ssc::text_file_reader::TextFileReader yaml_file(std::vector<std::string>(argv+1, argv+argc));
    const auto yaml = SimulatorYamlParser(yaml_file.get_contents()).parse();

    ssc::csv_file_reader::CSVFileReader csv(get_contents_from_stdin());
    OutputTransformer transform(get_builder(yaml));

    write(std::cout, build_output(csv, transform));
    return 0;
}

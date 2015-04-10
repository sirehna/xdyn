/*
 * gz.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>

#include "GZCurve.hpp"
#include "make_sim_for_GZ.hpp"
#include "OptionPrinter.hpp"
#include "utilities_for_InputData.hpp"

struct GZOptions
{
    GZOptions() : dphi(0), phi_max(0), stl_filename(), yaml_files()
    {}
    double dphi;
    double phi_max;
    std::string stl_filename;
    std::vector<std::string> yaml_files;
    bool empty() const
    {
        return (dphi==0) and (phi_max == 0) and stl_filename.empty() and yaml_files.empty();
    }
};

bool invalid(const GZOptions& input);
bool invalid(const GZOptions& input)
{
    if (input.empty()) return true;
    if (input.stl_filename.empty())
    {
        std::cerr << "Error: no STL file defined." << std::endl;
        return true;
    }
    if (input.yaml_files.empty())
    {
        std::cerr << "Error: no input YAML files defined: need at least one." << std::endl;
        return true;
    }
    if (input.dphi<=0)
    {
        std::cerr << "Error: the roll step should be strictly positive. Received " << input.dphi << std::endl;
        return true;
    }
    if (input.phi_max<input.dphi)
    {
        std::cerr << "Error: the maximum roll angle should be greater than the roll step. Received " << input.phi_max << std::endl;
        return true;
    }
    return false;
}

po::options_description gz_options(GZOptions& input_data);
po::options_description gz_options(GZOptions& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                "Show this help message")
        ("yml,y",   po::value<std::vector<std::string> >(&input_data.yaml_files), "Path(s) to the YAML file(s)")
        ("stl,s",   po::value<std::string>(&input_data.stl_filename),             "Path to the STL file")
        ("dphi",    po::value<double>(&input_data.dphi),                          "Roll angle step (in degrees)")
        ("phi_max", po::value<double>(&input_data.phi_max),                       "Maximum roll angle (in degrees)")
    ;
    return desc;
}

int get_gz_data(int argc, char **argv, GZOptions& input_data);
int get_gz_data(int argc, char **argv, GZOptions& input_data)
{
    const po::options_description desc = gz_options(input_data);
    const bool help = parse_input(argc, argv, desc);
    if (invalid(input_data) or help)
    {
        print_usage(std::cout, desc, argv[0], "Righting lever curve computer");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    GZOptions input_data;
    const int error = get_gz_data(argc, argv, input_data);
    if (not(error))
    {
        const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_files);
        const ssc::text_file_reader::TextFileReader stl_reader(input_data.stl_filename);
        const Sim sim = GZ::make_sim(yaml_reader.get_contents(), stl_reader.get_contents());
        const GZ::Curve calculate(sim);
        const auto phis = calculate.get_phi(input_data.dphi*PI/180., input_data.phi_max*PI/180.);
        std::cout << "Phi [deg]\tGZ(phi) [m]" << std::endl;
        for (auto phi:phis)
        {
            std::cout << phi*180./PI << '\t' << calculate.gz(phi) << std::endl;
        }
    }
    return 0;
}
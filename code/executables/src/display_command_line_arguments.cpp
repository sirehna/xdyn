/*
 * display_command_line_arguments.cpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */

#include <ssc/check_ssc_version.hpp>

#include "display_command_line_arguments.hpp"
#include "OptionPrinter.hpp"

std::string description(const std::string& des)
{
    std::stringstream ss;
    ss << des << " created during the project 'Bassin Numerique (IRT Jules Verne)'." << std::endl
       << "(c) 2014-2015, IRT Jules Verne (https://www.irt-jules-verne.fr/)," << std::endl
       << "               SIREHNA (http://www.sirehna.com/)," << std::endl
       << "               Naval Group (https://www.naval-group.com/en/)," << std::endl
       << "               Bureau Veritas (https://www.bureauveritas.fr/)," << std::endl
       << "               Hydrocean (https://marine-offshore.bureauveritas.com/bvsolutions)," << std::endl
       << "               STX France (http://chantiers-atlantique.com/en/)," << std::endl
       << "               LHEEA (https://lheea.ec-nantes.fr/)" << std::endl
       << "for the initial version." << std::endl
       << "(c) 2015-2020 SIREHNA & Naval Group for all subsequent versions." << std::endl
       << std::endl
       << "ID: @GIT_SHA1@" << std::endl
       << "SHA of the SSC used: " << LONG_SSC_GIT_SHA << std::endl
       << std::endl;
    return ss.str();
}

BooleanArguments parse_input(int argc, char **argv, const po::options_description& desc)
{
    po::positional_options_description p;
    p.add("yml", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc)
                                                 .positional(p)
                                                 .run(), vm);
    po::notify(vm);
    BooleanArguments ret;
    ret.help = vm.count("help")>0;
    ret.debug = vm.count("debug")>0;
    ret.verbose = vm.count("verbose")>0;
    ret.show_websocket_debug_information = vm.count("websocket-debug")>0;
    ret.grpc = vm.count("grpc")>0;
    return ret;
}

void print_usage(std::ostream& os, const po::options_description& desc, const std::string& program_name, const std::string& des)
{
    po::positional_options_description positionalOptions;
    os << description(des) << std::endl;
    rad::OptionPrinter::printStandardAppDesc(program_name + " <yaml file>",
                                             os,
                                             desc,
                                             &positionalOptions);
    os << desc << std::endl
       << std::endl;
}

void copy_stream(const std::ostream& from_stream, std::ostream& to_stream)
{
    to_stream.copyfmt(from_stream);                                  //1
    to_stream.clear(from_stream.rdstate());                          //2
    to_stream.basic_ios<char>::rdbuf(from_stream.rdbuf());           //3
}

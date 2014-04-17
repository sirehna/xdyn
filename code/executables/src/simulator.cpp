/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "boost/program_options.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    boost::program_options::options_description desc("Options");
    desc.add_options()("help", "Print this help message")
                      ("yaml", "One or several YAML input files")
                      ("like", "this");

    boost::program_options::positional_options_description p;
    p.add("yaml", -1);
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    return 0;
}

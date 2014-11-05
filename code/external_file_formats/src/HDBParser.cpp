/*
 * HDBParser.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cady
 */

#include "HDBParser.hpp"

HDBParser::HDBParser(const std::string& contents_) : contents(contents_)
{
}

HDBFile HDBParser::parse() const
{
    return HDBFile();
}

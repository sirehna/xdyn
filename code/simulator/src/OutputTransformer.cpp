/*
 * OutputGenerator.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformer.hpp"


OutputTransformer::OutputTransformer()
{
}

std::map<std::string,double> OutputTransformer::operator()(const Res& ) const
{
    return std::map<std::string,double>();
}

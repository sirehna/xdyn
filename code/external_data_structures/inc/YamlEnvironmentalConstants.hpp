/*
 * YamlEnvironmentalConstants.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef YAMLENVIRONMENTALCONSTANTS_HPP_
#define YAMLENVIRONMENTALCONSTANTS_HPP_

#include <boost/optional.hpp>

struct YamlEnvironmentalConstants
{
    YamlEnvironmentalConstants();
    double g;
    double rho;
    boost::optional<double> rho_air;
    double nu;
};

#endif /* YAMLENVIRONMENTALCONSTANTS_HPP_ */

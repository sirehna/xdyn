#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_

#include <vector>
#include "AbstractStates.hpp"

struct YamlHistory
{
    YamlHistory();
    YamlHistory(const double Tmax);
    std::vector<double> values;
    std::vector<double> tau;
};


struct YamlState : AbstractStates<YamlHistory>
{
    YamlState();
    double t;
};

#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_ */


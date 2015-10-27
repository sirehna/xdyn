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

bool operator == (const YamlState& state1, const YamlState& state2);
bool operator == (const YamlHistory& h1, const YamlHistory& h2);

#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLSTATE_HPP_ */


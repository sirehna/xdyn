#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSERVERINPUTS_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSERVERINPUTS_HPP_
#include "YamlState.hpp"
#include <map>

struct YamlSimServerInputs
{
    YamlSimServerInputs();
    double Dt;
    YamlState state;
    std::map<std::string, double> commands;
};



#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSERVERINPUTS_HPP_ */

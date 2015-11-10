#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSTEPPERINFO_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSTEPPERINFO_HPP_
#include "YamlState.hpp"
#include <map>

struct YamlSimStepperInfo
{
    YamlSimStepperInfo();
    double Dt;
    YamlState state;
    std::map<std::string, double> commands;
};



#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLSIMSTEPPERINFO_HPP_ */

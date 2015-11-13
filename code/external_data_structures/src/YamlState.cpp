#include "YamlState.hpp"

YamlHistory::YamlHistory()
    : values()
    , tau()
{
}

YamlHistory::YamlHistory(const double) // To keep AbstractStates happy
    : values()
    , tau()
{
}

YamlState::YamlState()
    : AbstractStates<YamlHistory>()
    , t()
{
}


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

bool operator==(const YamlState& lhs, const YamlState& rhs)
{
    return ((AbstractStates<YamlHistory>)lhs == (AbstractStates<YamlHistory>)rhs) and (lhs.t == rhs.t);
}

bool operator==(const YamlHistory& lhs, const YamlHistory& rhs)
{
    return (lhs.tau == rhs.tau) and (lhs.values == rhs.values);
}

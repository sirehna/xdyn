#ifndef OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_
#define OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_
#include "State.hpp"
#include "SimStepper.hpp"
#include <map>

class HistoryParser
{
    public :
        HistoryParser(const double Tmax);
        const double Tmax;


        State parse_state_history_yaml(const std::string& yaml, const double Tmax);
        std::string  emit_state_history_yaml(const State& state);
        SimServerInputs parse_SimServerInputs(const std::string& yaml) const;
    private:
        HistoryParser();


};



#endif /* OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_ */

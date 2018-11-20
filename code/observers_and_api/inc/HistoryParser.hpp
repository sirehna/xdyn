#ifndef OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_
#define OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_

#include "Res.hpp"
#include "State.hpp"
#include "SimServerInputs.hpp"
#include <map>


std::string emit_state_history_json(const std::vector<Res>& states);
SimServerInputs parse_SimServerInputs(const std::string& json, const double max_history_length);

#endif /* OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_ */

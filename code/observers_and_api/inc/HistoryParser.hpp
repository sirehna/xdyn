#ifndef OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_
#define OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_

#include <string>
#include "SimServerInputs.hpp"

SimServerInputs parse_SimServerInputs(const std::string& json, const double max_history_length);

#endif /* OBSERVERS_AND_API_INC_HISTORYPARSER_HPP_ */

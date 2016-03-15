#ifndef UTILITIES_FOR_SIMULATOR_HPP_
#define UTILITIES_FOR_SIMULATOR_HPP_

#include <string>
#include "YamlOutput.hpp"
#include "ListOfObservers.hpp"

class InputData;

std::vector<YamlOutput> get_observers_description(const std::string& yaml, const InputData& input_data);
ListOfObservers get_observers(const std::string& yaml, const InputData& input_data);

#endif

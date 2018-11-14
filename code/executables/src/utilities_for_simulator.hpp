#ifndef UTILITIES_FOR_SIMULATOR_HPP_
#define UTILITIES_FOR_SIMULATOR_HPP_

#include <string>
#include "YamlOutput.hpp"
#include "ListOfObservers.hpp"

class XdynCommandLineArguments;

std::vector<YamlOutput> get_observers_description(const std::string& yaml, const XdynCommandLineArguments& input_data);
ListOfObservers get_observers(const std::string& yaml, const XdynCommandLineArguments& input_data);

#endif

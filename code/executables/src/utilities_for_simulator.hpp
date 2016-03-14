#ifndef UTILITIES_FOR_SIMULATOR_HPP_
#define UTILITIES_FOR_SIMULATOR_HPP_

#include <string>
#include "ListOfObservers.hpp"

class InputData;

ListOfObservers get_observers(const std::string& yaml, const InputData& input_data);

#endif

#ifndef UTILITIES_FOR_SIMULATOR_HPP_
#define UTILITIES_FOR_SIMULATOR_HPP_

#include <string>
#include "InputData.hpp"
#include "ListOfObservers.hpp"

ListOfObservers get_observers(const std::string& yaml, const InputData& input_data);

#endif

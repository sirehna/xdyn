#ifndef PARSE_ADDRESS_HPP_
#define PARSE_ADDRESS_HPP_

#include <string>
#include "YamlOutput.hpp"

bool parseWebSocketURL(const std::string& url);
YamlOutput build_YamlOutput_from_WS_URL(const std::string& address);

#endif

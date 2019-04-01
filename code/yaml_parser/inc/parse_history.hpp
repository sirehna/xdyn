#ifndef YAML_PARSER_INC_PARSE_HISTORY_HPP_
#define YAML_PARSER_INC_PARSE_HISTORY_HPP_

#include "YamlSimServerInputs.hpp"
#include "YamlState.hpp"
#include <string>
#include <vector>

std::string encode_YamlStates(const std::vector<YamlState>& states);
YamlSimServerInputs decode_YamlSimServerInputs(const std::string& yaml);


#endif /* YAML_PARSER_INC_PARSE_HISTORY_HPP_ */

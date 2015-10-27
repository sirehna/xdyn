#ifndef YAML_PARSER_INC_PARSE_HISTORY_HPP_
#define YAML_PARSER_INC_PARSE_HISTORY_HPP_

#include "YamlState.hpp"
YamlState parse_history_yaml(const std::string& yaml //!< YAML data
                                            );


std::string generate_history_yaml(const YamlState& state);

#endif /* YAML_PARSER_INC_PARSE_HISTORY_HPP_ */

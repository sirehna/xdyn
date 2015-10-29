#ifndef YAML_PARSER_INC_PARSE_HISTORY_HPP_
#define YAML_PARSER_INC_PARSE_HISTORY_HPP_

#include "YamlState.hpp"
#include "YamlSimStepperInfo.hpp"


YamlState parse_history_yaml(const std::string& yaml //!< YAML data
                                            );

std::string generate_history_yaml(const YamlState& state);
YamlSimStepperInfo get_yamlsimstepperinfo(const std::string& yaml);



#endif /* YAML_PARSER_INC_PARSE_HISTORY_HPP_ */

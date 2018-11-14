#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

#include <functional>
#include <string>

void catch_exceptions(const std::function<void(void)>& f, const std::string& solver);

#endif

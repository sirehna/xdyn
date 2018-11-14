#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

#include <functional>
#include <string>

void report_xdyn_exceptions_to_user(const std::function<void(void)>& f, const std::string& solver);

#endif

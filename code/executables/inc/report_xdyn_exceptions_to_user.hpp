#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

#include <functional>
#include <string>

void report_xdyn_exceptions_to_user(const std::function<void(void)>& f, const std::function<void(const std::string&)>& outputter);

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
std::string current_date_time();

#endif

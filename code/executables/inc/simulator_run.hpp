#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

#include <functional>

struct XdynCommandLineArguments;
class Sim;
class ListOfObservers;

void solve(const XdynCommandLineArguments& input_data, Sim& sys, ListOfObservers& observer);
void run_simulation(const XdynCommandLineArguments& input_data);
void catch_exceptions(const std::function<void(void)>& f, const std::string& solver);

#endif

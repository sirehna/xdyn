#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

#include <functional>

struct InputData;
class Sim;
class ListOfObservers;

void solve(const InputData& input_data, Sim& sys, ListOfObservers& observer);
void run_simulation(const InputData& input_data);
void catch_exceptions(const std::function<void(void)>& f, const std::string& solver);

#endif

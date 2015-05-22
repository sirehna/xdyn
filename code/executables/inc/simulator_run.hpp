#ifndef SIMULATOR_RUN_HPP_
#define SIMULATOR_RUN_HPP_

class InputData;
class Sim;
class ListOfObservers;

void solve(const InputData& input_data, Sim& sys, ListOfObservers& observer);
void run_simulation(const InputData& input_data);

#endif

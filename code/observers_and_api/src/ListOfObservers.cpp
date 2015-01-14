/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CSVObserver.hpp"
#include "ListOfObservers.hpp"
#include "YamlOutput.hpp"

ListOfObservers::ListOfObservers(const std::vector<YamlOutput>& yaml) :
observers()
{
    for (auto output:yaml)
    {
        if (output.format == "csv")  observers.push_back(ObserverPtr(new CSVObserver(output.filename,output.data)));
//        if (output.first == "hdf5") observers.push_back(ObserverPtr(new HDF5Observer(output.second)));
    }
}

void ListOfObservers::observe(const Sim& sys, const double t)
{
    for (auto observer:observers)
    {
        observer->observe(sys,t);
    }
}

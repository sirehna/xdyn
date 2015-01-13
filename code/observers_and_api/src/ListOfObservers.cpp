/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */


#include "ListOfObservers.hpp"

ListOfObservers::ListOfObservers(const std::map<std::string, std::vector<std::string> >& data_per_format) :
observers()
{
    for (auto output:data_per_format)
    {
//        if (output.first == "csv")  observers.push_back(ObserverPtr(new CSVObserver(output.second)));
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

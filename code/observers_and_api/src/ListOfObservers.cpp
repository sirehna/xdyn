/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */


#include "ListOfObservers.hpp"

ListOfObservers::ListOfObservers(const std::vector<std::string>& formats, const std::vector<std::string>& data) :
observers()
{
    (void) formats;
    (void) data;
//    for (auto format:formats)
//    {
//        if (format == "csv")  observers.push_back(ObserverPtr(new CSVObserver(data)));
//        if (format == "hdf5") observers.push_back(ObserverPtr(new HDF5Observer(data)));
//    }
}

void ListOfObservers::observe(const Sim& sys, const double t)
{
    for (auto observer:observers)
    {
        observer->observe(sys,t);
    }
}

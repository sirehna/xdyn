/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "YamlOutput.hpp"
#include "CsvObserver.hpp"
#include "TsvObserver.hpp"
#include "JsonObserver.hpp"
#include "MapObserver.hpp"
#include "Hdf5Observer.hpp"
#include "WebSocketObserver.hpp"
#include "ListOfObservers.hpp"

ListOfObservers::ListOfObservers(const std::vector<YamlOutput>& yaml) : observers()
{
    for (auto output:yaml)
    {
        if (output.format == "csv")  observers.push_back(ObserverPtr(new CsvObserver(output.filename,output.data)));
        if (output.format == "h5")   observers.push_back(ObserverPtr(new Hdf5Observer(output.filename,output.data)));
        if (output.format == "hdf5") observers.push_back(ObserverPtr(new Hdf5Observer(output.filename,output.data)));
        if (output.format == "tsv")  observers.push_back(ObserverPtr(new TsvObserver(output.filename,output.data)));
        if (output.format == "map")  observers.push_back(ObserverPtr(new MapObserver(output.data)));
        if (output.format == "json") observers.push_back(ObserverPtr(new JsonObserver(output.filename,output.data)));
        if (output.format == "ws")   observers.push_back(ObserverPtr(new WebSocketObserver(output.address,output.port,output.data)));
    }
}

ListOfObservers::ListOfObservers(const std::vector<ObserverPtr>& observers_) : observers(observers_)
{
}

void ListOfObservers::observe(const Sim& sys, const double t)
{
    for (auto observer:observers)
    {
        observer->observe(sys,t);
    }
}

std::vector<ObserverPtr> ListOfObservers::get() const
{
    return observers;
}

bool ListOfObservers::empty() const
{
    return observers.empty();
}

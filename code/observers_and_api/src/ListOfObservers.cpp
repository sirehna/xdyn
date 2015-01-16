/*
 * ListOfObservers.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "CsvObserver.hpp"
#include "Hdf5Observer.hpp"
#include "ListOfObservers.hpp"
#include "TsvObserver.hpp"
#include "YamlOutput.hpp"

ListOfObservers::ListOfObservers(const std::vector<YamlOutput>& yaml) : observers()
{
    for (auto output:yaml)
    {
        if (output.format == "csv")  observers.push_back(ObserverPtr(new CsvObserver(output.filename,output.data)));
        if (output.format == "hdf5") observers.push_back(ObserverPtr(new Hdf5Observer(output.filename,output.data)));
        if (output.format == "tsv")  observers.push_back(ObserverPtr(new TsvObserver(output.filename,output.data)));
    }
}

void ListOfObservers::observe(const Sim& sys, const double t)
{
    for (auto observer:observers)
    {
        observer->observe(sys,t);
    }
}

/*
 * ListOfObservers.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef LISTOFOBSERVERS_HPP_
#define LISTOFOBSERVERS_HPP_

#include <map>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "Observer.hpp"

typedef std::tr1::shared_ptr<Observer> ObserverPtr;

class ListOfObservers
{
    public:
        ListOfObservers(const std::string& basename, const std::map<std::string, std::vector<std::string> >& data_per_format);
        void observe(const Sim& sys, const double t);

    private:

        std::vector<ObserverPtr> observers;
};


#endif /* LISTOFOBSERVERS_HPP_ */

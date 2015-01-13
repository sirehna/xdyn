/*
 * ListOfObservers.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef LISTOFOBSERVERS_HPP_
#define LISTOFOBSERVERS_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "Observer.hpp"

typedef std::tr1::shared_ptr<Observer> ObserverPtr;

class YamlOutput2;

class ListOfObservers
{
    public:
        ListOfObservers(const std::string& basename, const std::vector<YamlOutput2>& yaml);
        void observe(const Sim& sys, const double t);

    private:

        std::vector<ObserverPtr> observers;
};


#endif /* LISTOFOBSERVERS_HPP_ */

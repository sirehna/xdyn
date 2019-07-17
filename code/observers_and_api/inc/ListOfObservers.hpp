/*
 * ListOfObservers.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef LISTOFOBSERVERS_HPP_
#define LISTOFOBSERVERS_HPP_

#include "Observer.hpp"

struct YamlOutput;

class ListOfObservers
{
    public:
        ListOfObservers(const std::vector<YamlOutput>& yaml);
        ListOfObservers(const std::vector<ObserverPtr>& observers);
        void observe(const Sim& sys, const double t);
        std::vector<ObserverPtr> get() const;
        bool empty() const;

        template <typename T> void write(
                const T& val,
                const DataAddressing& address)
        {
            for (auto observer:observers)
            {
                observer->write(val, address);
            }
        }

        template <typename T> void write_before_simulation(
                        const T& val,
                        const DataAddressing& address)
        {
            for (auto observer:observers)
            {
                observer->write_before_simulation(val, address);
            }
        }

    private:

        std::vector<ObserverPtr> observers;
};

#endif /* LISTOFOBSERVERS_HPP_ */

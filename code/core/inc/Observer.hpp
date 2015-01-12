/*
 * Observer.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */


#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include "OutputtedVar.hpp"

class Sim;

class Observer
{
    public:
        Observer(const std::vector<std::string>& data);
        void observe(const Sim& sys, const double t);
        virtual ~Observer();

        virtual void write(const TypedOutputtedVar<double>& val) = 0;
        virtual void write(const TypedOutputtedVar<std::string>& val);
        virtual void write(const TypedOutputtedVar<std::vector<double> >& val);

    private:
        std::vector<std::string> data;
};

#endif  /* OBSERVER_HPP_ */

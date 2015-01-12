/*
 * Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */



#include "Observer.hpp"

Observer::Observer(const std::vector<std::string>& data_) : data(data_)
{

}

void Observer::observe(const Sim& , const double )
{

}

Observer::~Observer()
{
}

void Observer::write(const TypedOutputtedVar<std::string>& )
{

}

void Observer::write(const TypedOutputtedVar<std::vector<double> >& )
{

}

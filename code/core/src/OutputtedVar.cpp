/*
 * OutputtedVar.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "Observer.hpp"
#include "OutputtedVar.hpp"

OutputtedVar::~OutputtedVar()
{
}

template <> void TypedOutputtedVar<double>::write_on(Observer& obs) const;
template <> void TypedOutputtedVar<double>::write_on(Observer& obs) const
{
    obs.write(*this);
}

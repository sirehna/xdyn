/*
 * DsSolve.hpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#ifndef DSSOLVE_HPP_
#define DSSOLVE_HPP_

#include "Observers.hpp"
#include "DsSystem.hpp"

void integrate(DataSource& ds, const double tstart, const double tend, DefaultObserver<DSSystem>& observer);
void integrate(DataSource& ds, const double tstart, const double tend);

#endif /* DSSOLVE_HPP_ */

/*
 * DsSolve.hpp
 *
 *  Created on: 25 mars 2014
 *      Author: cec
 */

#ifndef DSSOLVE_HPP_
#define DSSOLVE_HPP_

class DataSource;
class DsObserver;
void integrate(DataSource& ds, const double tstart, const double tend, DsObserver& observer);
void integrate(DataSource& ds, const double tstart, const double tend);

#endif /* DSSOLVE_HPP_ */

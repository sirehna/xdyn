/*
 * DsSolve.hpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#ifndef DSSOLVE_HPP_
#define DSSOLVE_HPP_

class DataSource;
class DsCsvObserver;
void integrate(DataSource& ds, const double tstart, const double tend, DsCsvObserver& observer);
void integrate(DataSource& ds, const double tstart, const double tend);

#endif /* DSSOLVE_HPP_ */

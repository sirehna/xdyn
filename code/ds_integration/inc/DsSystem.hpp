/*
 * DsSystem.hpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#ifndef DSSYSTEM_HPP_
#define DSSYSTEM_HPP_

#include <vector>

class DataSource;
class DSSystem
{
    public:
        DSSystem(DataSource& ds);
        void operator()(const std::vector<double>& x, std::vector<double>& dx_dt, double t);
        void update_discrete_states();
        void update_continuous_states();

        std::vector<double> state;

    private:
        DSSystem();
        DataSource& ds;
};

#endif /* DSSYSTEM_HPP_ */

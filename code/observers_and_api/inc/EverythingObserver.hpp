/*
 * EverythingObserver.hpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_
#define OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_

#include "MapObserver.hpp"
#include "Res.hpp"

class EverythingObserver : public MapObserver
{
    public:
        EverythingObserver();
        void observe(const Sim& sys, const double t);
        std::vector<Res> get() const;

    private:
        std::function<void(Res&, const double)> get_inserter(const std::string& body_name, const std::string& var_name) const;
        void fill_res();
        void add(const std::function<void(Res&, const double)>& inserter, const std::vector<double>& values);
        std::vector<Res> res;
};



#endif /* OBSERVERS_AND_API_INC_EVERYTHINGOBSERVER_HPP_ */

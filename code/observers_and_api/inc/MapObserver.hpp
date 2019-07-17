/*
 * MapObserver.hpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */

#ifndef MAPOBSERVER_HPP_
#define MAPOBSERVER_HPP_

#include "Observer.hpp"

class MapObserver : public Observer
{
    public:
        MapObserver(const std::vector<std::string>& data);
        std::map<std::string,std::vector<double> > get() const;

    protected:
        std::map<std::string,std::vector<double> > m;

    private:
        using Observer::get_serializer;
        using Observer::get_initializer;
        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();
};

#endif /* MAPOBSERVER_HPP_ */

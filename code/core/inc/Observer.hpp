/*
 * Observer.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "DiscreteDirectionalWaveSpectrum.hpp"

class Sim;
class SurfaceElevationGrid;

struct DataAddressing
{
    std::string name;
    std::vector<std::string> address;
    DataAddressing():name(),address(){};
    DataAddressing(
            const std::vector<std::string>& address_,
            const std::string& name_):
        name(name_),address(address_){};
};

class Observer
{
    public:
        Observer(const std::vector<std::string>& data);
        void observe(const Sim& sys, const double t);
        virtual ~Observer();

        template <typename T> void write(
                const T& val,
                const DataAddressing& address)
        {
            initialize[address.name] = get_initializer(val, address);
            serialize[address.name] = get_serializer(val, address);
        }

        virtual void write_before_simulation(const std::vector<FlatDiscreteDirectionalWaveSpectrum>& val, const DataAddressing& address);

    protected:

        virtual std::function<void()> get_serializer(const double val, const DataAddressing& address) = 0;
        virtual std::function<void()> get_initializer(const double val, const DataAddressing& address) = 0;

        virtual std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address);
        virtual std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address);

        virtual void flush_after_initialization() = 0;
        virtual void before_write();
        virtual void flush_after_write() = 0;
        virtual void flush_value_during_write() = 0;
        virtual void flush_value_during_initialization();

    private:
        Observer(); // Disabled

        void initialize_everything_if_necessary();
        void serialize_everything();

        bool initialized;
        std::vector<std::string> stuff_to_write;
        std::map<std::string, std::function<void()> > serialize;
        std::map<std::string, std::function<void()> > initialize;
};

typedef TR1(shared_ptr)<Observer> ObserverPtr;

#endif  /* OBSERVER_HPP_ */

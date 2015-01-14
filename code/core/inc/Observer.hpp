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

class Sim;

class Observer
{
    public:
        Observer(const std::vector<std::string>& data);
        void observe(const Sim& sys, const double t);
        virtual ~Observer();

        template <typename T> void write(
                const T& val,
                const std::vector<std::string>& address,
                const std::vector<std::string>& columnName,
                const std::string& name)
        {
            serialize[name] = get_serializer(val, address, columnName, name);
            initialize[name] = get_initializer(val, address, columnName, name);
        }

    protected:

        virtual std::function<void()> get_serializer(const double val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_serializer(const std::string& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_serializer(const std::vector<double>& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_serializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;

        virtual std::function<void()> get_initializer(const double val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const std::string& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const std::vector<double>& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name) = 0;
        virtual void flush_after_initialization() = 0;
        virtual void flush_after_write() = 0;
        virtual void flush_value() = 0;

    private:
        Observer(); // Disabled

        void initialize_everything_if_necessary();
        void serialize_everything();

        bool initialized;
        std::vector<std::string> stuff_to_write;
        std::map<std::string,std::function<void()> > serialize;
        std::map<std::string,std::function<void()> > initialize;
};

#endif  /* OBSERVER_HPP_ */

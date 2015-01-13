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

        template <typename T> void write(const T& val, const std::vector<std::string>& where, const std::string& name)
        {
            serialize[name] = get_serializer(val, where, name);
            initialize[name] = get_initializer(val, where, name);
        }

    protected:
        bool is_initialized() const;

        virtual std::function<void()> get_serializer(const double val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual std::function<void()> get_serializer(const std::string& val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual std::function<void()> get_serializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const double val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const std::string& val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual std::function<void()> get_initializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name) = 0;
        virtual void flush_after_initialization() = 0;
        virtual void flush_after_write() = 0;

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

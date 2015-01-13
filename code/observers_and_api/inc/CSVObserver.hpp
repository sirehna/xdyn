/*
 * CSVObserver.hpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#ifndef CSVOBSERVER_HPP_
#define CSVOBSERVER_HPP_

#include <ostream>

#include "Observer.hpp"

class CSVObserver : public Observer
{
    public:
        CSVObserver(std::ostream& os, const std::vector<std::string>& data);

    private:
        void flush_after_initialization();
        void flush_after_write();

        bool should_add_coma();
        bool add_coma;
        std::ostream& os;
        using Observer::get_serializer;
        using Observer::get_initializer;
        std::function<void()> get_serializer(const double val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_serializer(const std::string& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_serializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const double val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const std::string& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name);
};

#endif /* CSVOBSERVER_HPP_ */

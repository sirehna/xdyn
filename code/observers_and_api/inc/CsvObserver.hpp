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

class CsvObserver : public Observer
{
    public:
        CsvObserver(const std::string& filename, const std::vector<std::string>& data);
        ~CsvObserver();

    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();

        bool output_to_file;
        std::ostream& os;

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);
};

#endif /* CSVOBSERVER_HPP_ */

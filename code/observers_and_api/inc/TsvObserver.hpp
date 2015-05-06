#ifndef TSVOBSERVER_HPP_
#define TSVOBSERVER_HPP_

#include <ostream>

#include "Observer.hpp"

class TsvObserver : public Observer
{
    public:
        TsvObserver(const std::string& filename, const std::vector<std::string>& data);
        ~TsvObserver();

    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();

        bool output_to_file;
        std::ostream& os;
        size_t length_of_title_line;

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);
};

#endif /* TSVOBSERVER_HPP_ */

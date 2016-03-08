#ifndef JSONOBSERVER_HPP_
#define JSONOBSERVER_HPP_

#include <string>
#include <vector>
#include <ostream>

#include "DictObserver.hpp"

class JsonObserver : public DictObserver
{
    public:
        JsonObserver(const std::string& filename, const std::vector<std::string>& d);
        ~JsonObserver();

    private:
        void flush_after_write();

        bool output_to_file;
        std::ostream& os;
};

#endif

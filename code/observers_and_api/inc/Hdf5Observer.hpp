#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include <set>
#include <map>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "Observer.hpp"

class Hdf5Observer : public Observer
{
    public:
        Hdf5Observer(const std::string& filename, const std::vector<std::string>& data);

    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value();

        using Observer::get_serializer;
        using Observer::get_initializer;
        std::function<void()> get_serializer(const double val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_serializer(const std::string& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_serializer(const std::vector<double>& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_serializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);

        std::function<void()> get_initializer(const double val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_initializer(const std::string& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_initializer(const std::vector<double>& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);
        std::function<void()> get_initializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& address, const std::vector<std::string>& columnName, const std::string& name);

        H5::H5File h5File;
        std::string basename;
        std::set<std::string> datasetNames;
        std::map<std::string,H5::DataSet> h5Dataset;
};

#endif

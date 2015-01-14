#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

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
        std::function<void()> get_serializer(const double val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_serializer(const std::string& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_serializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_serializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& where, const std::string& name);

        std::function<void()> get_initializer(const double val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const std::string& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const std::vector<double>& val, const std::vector<std::string>& where, const std::string& name);
        std::function<void()> get_initializer(const std::vector<std::vector<double> >& val, const std::vector<std::string>& where, const std::string& name);
};

#endif

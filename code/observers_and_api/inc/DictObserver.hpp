#ifndef DICTOBSERVER_HPP_
#define DICTOBSERVER_HPP_

#include "Observer.hpp"
#include "SurfaceElevationGrid.hpp"

#include <sstream>
#include <utility>
#include <map>

typedef std::map<std::string, double> DictMap1;
typedef std::map<std::string, std::map<std::string, double> > DictMap2;

class DictObserver : public Observer
{
    public:
        DictObserver(const std::vector<std::string>& d);
        ~DictObserver();

    protected:
        virtual void flush_after_initialization(){};
        virtual void flush_value_during_write(){};
        virtual void flush_after_write();

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);

        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address);
        std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address);

        std::stringstream ss;
    private:
        std::stringstream ssSurfaceElevationGrid;
        DictMap1 dictMap1;
        DictMap2 dictMap2;
        bool shouldWeAddAStartingComma;
        void serializeDictMap1();
        void serializeDictMap2();
        void serializeDictMap2Attitudes();
        void serializeDictMap2Wrenches();
        void serializeDictSurfaceElevationGrid();
};

#endif

/*
 * DataSourceSerializer.hpp
 *
 *  Created on: 26 févr. 2014
 *      Author: maroff
 */

#ifndef DATASOURCESERIALIZER_HPP_
#define DATASOURCESERIALIZER_HPP_

#include <ostream>
#include <string>
#include <tr1/memory>
#include <tr1/unordered_map>
#include <vector>

#include "DataSource.hpp"
#include "TypeWriter.hpp"

typedef std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Writer> > Writers;

template <typename Target> class DataSourceSerializer
{
    public:
        DataSourceSerializer(std::ostream& rhs) : stream(rhs), writers(Writers())
        {
        }
        virtual ~DataSourceSerializer(){}

        bool write_value(const std::string& type, const boost::any& value)
        {
            const Writers::const_iterator that_writer = writers.find(type);
            return (that_writer != writers.end()) and that_writer->second->write(type, value);
        }

        template <typename Type> void can_serialize()
        {
            const std::string type = typeid(Type).name();
            std::tr1::shared_ptr<DataSourceSerializer<Target> > c = this->clone();
            TypeWriter<Target,Type> type_writer(c);
            writers.insert(std::make_pair(type, type_writer.clone()));
        }

        virtual std::tr1::shared_ptr<DataSourceSerializer<Target> > clone() = 0;

        virtual void serialize_module(const std::string& module_name, const std::string& module_type) = 0;

        void serialize(const DataSource& ds)
        {
            write_compiler_data();
            initialize_datasource_serialization();
            write_modules(ds.get_module_list());
            write_signals(ds.get_signals());
            flush();
        }

    protected:
        std::ostream& stream;

    private:
        DataSourceSerializer();
        virtual void flush() = 0;
        virtual void initialize_datasource_serialization() = 0;
        virtual void write_compiler_data() = 0;
        virtual void write_signals(const std::list<std::pair<TypedSignalName, boost::any> >& signals) = 0;
        virtual void write_modules(const std::list<TypedModuleName>& modules) = 0;

        Writers writers;
};

#endif /* DATASOURCESERIALIZER_HPP_ */

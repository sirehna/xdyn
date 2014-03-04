/*
 * DataSourceYamlSerializer.hpp
 *
 *  Created on: 26 févr. 2014
 *      Author: maroff
 */

#ifndef DATASOURCEYAMLSERIALIZER_HPP_
#define DATASOURCEYAMLSERIALIZER_HPP_


#include "DataSourceSerializer.hpp"
#include "yaml-cpp/yaml.h"

#include <set>

class DataSourceYamlSerializer : public DataSourceSerializer<DataSourceYamlSerializer> // "Curiously Recurring Template" pattern
{
    public:
        DataSourceYamlSerializer(std::ostream& rhs);
        void serialize_module(const std::string& module_name, const std::string& module_type);
        std::tr1::shared_ptr<DataSourceSerializer<DataSourceYamlSerializer> > clone();
        std::tr1::shared_ptr<YAML::Emitter> out;

    private:
        DataSourceYamlSerializer();
        void flush();
        void initialize_datasource_serialization();
        void write_compiler_data();
        void write_signals(const std::list<std::pair<TypedSignalName, boost::any> >& signals);
        void write_modules(const std::list<TypedModuleName>& modules);
};

#endif /* DATASOURCEYAMLSERIALIZER_HPP_ */

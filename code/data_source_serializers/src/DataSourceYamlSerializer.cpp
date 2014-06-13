/*
 * DataSourceYamlSerializer.cpp
 *
 *  Created on: 26 f�vr. 2014
 *      Author: cec
 */

#include "DataSourceYamlSerializer.hpp"
#include "DataSourceYamlSerializerException.hpp"
#include "common_serialization_constants.hpp"


#include "test_macros.hpp"

DataSourceYamlSerializer::DataSourceYamlSerializer(std::ostream& rhs) : DataSourceSerializer(rhs), out(TR1(shared_ptr)<YAML::Emitter>(new YAML::Emitter))
{
    out->SetIndent(4);
}

TR1(shared_ptr)<DataSourceSerializer<DataSourceYamlSerializer> > DataSourceYamlSerializer::clone()
{
    return TR1(shared_ptr)<DataSourceSerializer<DataSourceYamlSerializer> >(new DataSourceYamlSerializer(*this));
}

void DataSourceYamlSerializer::serialize_module(const std::string& module_name, const std::string& module_type)
{
    *out << YAML::BeginMap;
    *out << YAML::Key   << "name";
    *out << YAML::Value << module_name;
    *out << YAML::Key   << "type";
    *out << YAML::Value << module_type;
    *out << YAML::EndMap;
}

void DataSourceYamlSerializer::write_compiler_data()
{
    *out << YAML::BeginMap;
    *out << YAML::Key   << common_serialization_constants::compiler_key;
    *out << YAML::Value << common_serialization_constants::compiler_name;
}

void DataSourceYamlSerializer::write_signals(const std::list<std::pair<TypedSignalName, boost::any> >& signals)
{
    *out << YAML::Key << common_serialization_constants::signals_key
            << YAML::Value
            << YAML::BeginSeq;
    for (std::list<std::pair<TypedSignalName, boost::any> >::const_iterator that_signal = signals.begin() ; that_signal != signals.end() ; ++that_signal)
    {
        const std::string name = that_signal->first.get_signal_name();
        const std::string type = that_signal->first.get_type_name();
        *out << YAML::BeginMap
             << YAML::Key << "name"
             << YAML::Value << name
             << YAML::Key << "type"
             << YAML::Value << type
             << YAML::Key << "value"
             << YAML::Value;
        write_value(type,that_signal->second);
            // << that_signal->get_value()
        *out << YAML::EndMap;
    }
    *out << YAML::EndSeq;
    if (not(out->good()))
    {
        //THROW(__PRETTY_FUNCTION__,DataSourceYamlSerializerException, "Problem serializing DataSource signals to YAML");
    }
}

void DataSourceYamlSerializer::write_modules(const std::list<TypedModuleName>& modules)
{
    *out << YAML::Key << common_serialization_constants::modules_key
         << YAML::Value
         << YAML::BeginSeq;
    for (std::list<TypedModuleName>::const_iterator it = modules.begin() ; it != modules.end() ; ++it)
    {
        *out << YAML::BeginMap
             << YAML::Key << "name"
             << YAML::Value << it->get_signal_name()
             << YAML::Key << "type"
             << YAML::Value << it->get_type_name()
             << YAML::EndMap;
    }
    *out << YAML::EndSeq;
    if (not(out->good()))
    {
        THROW(__PRETTY_FUNCTION__,DataSourceYamlSerializerException, "Problem serializing DataSource modules to YAML");
    }
}

void DataSourceYamlSerializer::flush()
{
    *out << YAML::EndMap;
    if (not(out->good()))
    {
       // THROW(__PRETTY_FUNCTION__,DataSourceYamlSerializerException, "Problem serializing DataSource to YAML");
    }
    stream << out->c_str();
}

void DataSourceYamlSerializer::initialize_datasource_serialization()
{
    *out << YAML::Key   << "data source"
         << YAML::Value
         << YAML::BeginMap;
}

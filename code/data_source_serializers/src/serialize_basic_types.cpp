/*
 * serialize_basic_types.cpp
 *
 *  Created on: 3 mars 2014
 *      Author: maroff
 */

#include "serialize_basic_types.hpp"

template <> bool TypeWriter<DataSourceYamlSerializer,double>::write(const std::string& type, const boost::any& value)
{
    if (type == typeid(double).name())
    {
        try
        {
            *((DataSourceYamlSerializer*)serializer.get())->out << boost::any_cast<double>(value);
        }
        catch(boost::bad_any_cast&)
        {
            return false;
        }
        return true;
    }
    return false;
}

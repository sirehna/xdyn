/*
 * TypeWriter.hpp
 *
 *  Created on: 3 mars 2014
 *      Author: maroff
 */

#ifndef TYPEWRITER_HPP_
#define TYPEWRITER_HPP_

#include <tr1/memory>
#include <tr1/unordered_map>
#include <boost/static_assert.hpp>

#include "Writer.hpp"

template <typename Target> class DataSourceSerializer;

template <typename Target, typename Type> class TypeWriter : public Writer
{
    public:
        TypeWriter(std::tr1::shared_ptr<DataSourceSerializer<Target> >& s) : serializer(s) {}

        std::tr1::shared_ptr<Writer> clone() const
        {
            return std::tr1::shared_ptr<Writer>(new TypeWriter<Target,Type>(*this));
        }

        bool write(const std::string& type, const boost::any& value);

    private:
        std::tr1::shared_ptr<DataSourceSerializer<Target> > serializer;
};


#endif /* TYPEWRITER_HPP_ */

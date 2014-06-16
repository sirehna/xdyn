/*
 * TypeWriter.hpp
 *
 *  Created on: 3 mars 2014
 *      Author: cec
 */

#ifndef TYPEWRITER_HPP_
#define TYPEWRITER_HPP_

#include "tr1_macros.hpp"
#include TR1INC(unordered_map)
#include <boost/static_assert.hpp>

#include "Writer.hpp"

template <typename Target> class DataSourceSerializer;

template <typename Target, typename Type> class TypeWriter : public Writer
{
    public:
        TypeWriter(TR1(shared_ptr)<DataSourceSerializer<Target> >& s) : serializer(s) {}

        TR1(shared_ptr)<Writer> clone() const
        {
            return TR1(shared_ptr)<Writer>(new TypeWriter<Target,Type>(*this));
        }

        bool write(const std::string& type, const boost::any& value);

    private:
        TR1(shared_ptr)<DataSourceSerializer<Target> > serializer;
};


#endif /* TYPEWRITER_HPP_ */

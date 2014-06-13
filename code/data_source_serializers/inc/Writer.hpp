/*
 * Writer.hpp
 *
 *  Created on: 3 mars 2014
 *      Author: cec
 */

#ifndef WRITER_HPP_
#define WRITER_HPP_

#include <string>
namespace boost{class any;}


class Writer
{
    public:
        Writer() {};
        virtual ~Writer() {};
        virtual std::tr1::shared_ptr<Writer> clone() const = 0;
        virtual bool write(const std::string& type, const boost::any& value) = 0;
};


#endif /* WRITER_HPP_ */

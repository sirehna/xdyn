/*
 * HDBParser.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: cady
 */

#ifndef HDBPARSER_HPP_
#define HDBPARSER_HPP_

#include "HDBFile.hpp"

/** \brief
 *  \details
 *  \addtogroup external_file_formats
 *  \ingroup external_file_formats
 *  \section ex1 Example
 *  \snippet external_file_formats/unit_tests/src/HDBParserTest.cpp HDBParserTest example
 *  \section ex2 Expected output
 *  \snippet external_file_formats/unit_tests/src/HDBParserTest.cpp HDBParserTest expected output
 */
class HDBParser
{
    public:
        HDBParser(const std::string& contents);
        HDBFile parse() const;

    private:
        HDBParser();
        std::string contents;
};

#endif /* HDBPARSER_HPP_ */

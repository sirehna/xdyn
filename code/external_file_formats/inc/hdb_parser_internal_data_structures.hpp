/*
 * hdb_parser_internal_data_structures.hpp
 *
 *  Created on: Nov 6, 2014
 *      Author: cady
 */

#ifndef HDB_PARSER_INTERNAL_DATA_STRUCTURES_HPP_
#define HDB_PARSER_INTERNAL_DATA_STRUCTURES_HPP_

#include <string>
#include <vector>

namespace hdb
{
    typedef std::string        Header;
    typedef double             Value;
    typedef std::vector<Value> Values;

    template <typename T> struct Key
    {
        Key() : header(), value() {}

        Header header;
        T      value;
    };

    struct Section
    {
        Section() : header(), values() {}

        Header header;
        Values values;
    };

    struct SectionWithId
    {
        SectionWithId() : header(), id(), values() {}

        Header header;
        Value  id;
        Values values;
    };

    struct ListOfSections
    {
        ListOfSections() : header(), sections() {}

        Header               header;
        std::vector<Section> sections;
    };

    struct ListOfSectionsWithId
    {
        ListOfSectionsWithId() : header(), sections_with_id() {}

        Header                     header;
        std::vector<SectionWithId> sections_with_id;
    };

    struct AST
    {
        AST() : string_keys(), value_keys(), sections(), sections_with_id(), list_of_sections(), list_of_sections_with_id() {}

        std::vector<Key<std::string> >    string_keys;
        std::vector<Key<double> >         value_keys;
        std::vector<Section>              sections;
        std::vector<SectionWithId>        sections_with_id;
        std::vector<ListOfSections>       list_of_sections;
        std::vector<ListOfSectionsWithId> list_of_sections_with_id;
    };
}

#endif /* HDB_PARSER_INTERNAL_DATA_STRUCTURES_HPP_ */

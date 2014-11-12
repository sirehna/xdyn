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
    typedef std::vector<Values> ListOfValues;

    template <typename T> struct Key
    {
        Key() : header(), value() {}

        Header header;
        T      value;
    };

    struct VectorSection
    {
            VectorSection() : header(), values() {}

        Header header;
        Values values;
    };

    struct MatrixSection
    {
            MatrixSection() : header(), values() {}

        Header header;
        ListOfValues values;
    };

    struct SectionWithId
    {
        SectionWithId() : header(), id(), values() {}

        Header header;
        Value  id;
        ListOfValues values;
    };

    struct ListOfSections
    {
        ListOfSections() : header(), sections() {}

        Header               header;
        std::vector<MatrixSection> sections;
    };

    struct ListOfSectionsWithId
    {
        ListOfSectionsWithId() : header(), sections_with_id() {}

        Header                     header;
        std::vector<SectionWithId> sections_with_id;
    };

    struct AST
    {
        AST() : string_keys(), value_keys(), vector_sections(), matrix_sections(), sections_with_id(), list_of_sections(), list_of_sections_with_id() {}

        std::vector<Key<std::string> >    string_keys;
        std::vector<Key<double> >         value_keys;
        std::vector<VectorSection>        vector_sections;
        std::vector<MatrixSection>        matrix_sections;
        std::vector<SectionWithId>        sections_with_id;
        std::vector<ListOfSections>       list_of_sections;
        std::vector<ListOfSectionsWithId> list_of_sections_with_id;
    };
}

#endif /* HDB_PARSER_INTERNAL_DATA_STRUCTURES_HPP_ */

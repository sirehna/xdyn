/*
 * hdb_grammar.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#ifndef HDB_GRAMMAR_HPP_
#define HDB_GRAMMAR_HPP_

#include "boost_spirit_hdb_adapters.hpp"
#include <boost/spirit/include/phoenix_core.hpp>

typedef ascii::blank_type SpaceType;

namespace hdb
{
    template <typename Iterator>
    struct hdb_grammar : qi::grammar<Iterator, hdb::Ast(), SpaceType>
    {
        hdb_grammar() : hdb_grammar::base_type(ast)
        {
            ast                             = string_key
                                            | value_key
                                            | vector_section
                                            | matrix_section
                                            | list_of_matrix_sections
                                            | list_of_matrix_sections_with_id;
            str                             %= qi::lexeme[qi::char_("_a-zA-Z") >> +(qi::char_("-_a-zA-Z0-9+")) >> *(qi::hold[+(qi::char_(' ')) >> +(qi::char_("-_a-zA-Z0-9+"))])]; // 'hold' parses space only if next token matches word;
            header                          %= lit('[') >> str > lit(']');
            string_key                      %= header >> str >> -eol;
            value_key                       %= header >> double_ >> -eol;
            values                          %= double_ >> +double_ >> -eol;
            vector_section                  %= header >> eol >> +(double_ >> eol) >> -eol;
            matrix_section                  %= header >> eol >> +(values % eol) >> -eol;
            list_of_matrix_sections         %= header >> eol >> +matrix_section >> -eol;
            section_with_id                 %= header >> double_ >> eol >> (values % eol) >> -eol;
            list_of_matrix_sections_with_id %= header >> eol >> +(header >> double_ >> eol >> +(values % eol));
        }

        qi::rule<Iterator, hdb::Ast(), SpaceType>                        ast;
        qi::rule<Iterator, hdb::Header(), SpaceType>                     header;
        qi::rule<Iterator, std::string(), SpaceType>                     str;
        qi::rule<Iterator, hdb::Key<double>(), SpaceType>                value_key;
        qi::rule<Iterator, hdb::Key<std::string>(), SpaceType>           string_key;
        qi::rule<Iterator, hdb::ListOfMatrixSections(), SpaceType>       list_of_matrix_sections;
        qi::rule<Iterator, hdb::ListOfMatrixSectionsWithId(), SpaceType> list_of_matrix_sections_with_id;
        qi::rule<Iterator, hdb::VectorSection(), SpaceType>              vector_section;
        qi::rule<Iterator, hdb::MatrixSection(), SpaceType>              matrix_section;
        qi::rule<Iterator, hdb::SectionWithId(), SpaceType>              section_with_id;
        qi::rule<Iterator, hdb::Values(), SpaceType>                     values;
    };

    typedef hdb_grammar<std::string::const_iterator> grammar;
}

#endif /* HDB_GRAMMAR_HPP_ */

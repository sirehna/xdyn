/*
 * hdb_grammar.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#ifndef HDB_GRAMMAR_HPP_
#define HDB_GRAMMAR_HPP_

#include "boost_spirit_hdb_adapters.hpp"

namespace hdb
{
    template <typename Iterator>
    struct hdb_grammar : qi::grammar<Iterator, hdb::AST(), ascii::space_type>
    {
        hdb_grammar() : hdb_grammar::base_type(ast)
        {
            ast                      %= string_key | value_key | vector_section | matrix_section | section_with_id | list_of_matrix_sections | list_of_sections_with_id;
            str   %= qi::lexeme[qi::char_("_a-zA-Z") >> +(qi::char_("-_a-zA-Z0-9+")) >> *(qi::hold[+(qi::char_(' ')) >> +(qi::char_("-_a-zA-Z0-9+"))])]; // 'hold' parses space only if next token matches word;

            header                   %= lit('[') >> str >> lit(']');
            string_key               %= header >> str;
            value_key                %= header >> double_;
            values                   %= double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> double_;
            vector_section           %= header >> +(double_ % qi::eol);
            matrix_section           %= header >> +(values % qi::eol);
            list_of_matrix_sections  %= header >> +matrix_section;// % qi::eol);
            section_with_id          %= header >> double_ >> +(eol >> values);
            list_of_sections_with_id %= header >> +(eol >> section_with_id);
        }

        qi::rule<Iterator, hdb::AST(), ascii::space_type>                  ast;
        qi::rule<Iterator, hdb::Header(), ascii::space_type>               header;
        qi::rule<Iterator, std::string(), ascii::space_type>               str;
        qi::rule<Iterator, hdb::Key<double>(), ascii::space_type>          value_key;
        qi::rule<Iterator, hdb::Key<std::string>(), ascii::space_type>     string_key;
        qi::rule<Iterator, hdb::ListOfMatrixSections(), ascii::space_type> list_of_matrix_sections;
        qi::rule<Iterator, hdb::ListOfSectionsWithId(), ascii::space_type> list_of_sections_with_id;
        qi::rule<Iterator, hdb::VectorSection(), ascii::space_type>        vector_section;
        qi::rule<Iterator, hdb::MatrixSection(), ascii::space_type>        matrix_section;
        qi::rule<Iterator, hdb::SectionWithId(), ascii::space_type>        section_with_id;
        qi::rule<Iterator, hdb::Values(), ascii::space_type>               values;
    };

    typedef hdb_grammar<std::string::const_iterator> grammar;
}

#endif /* HDB_GRAMMAR_HPP_ */

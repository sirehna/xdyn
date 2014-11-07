/*
 * hdb_to_ast.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#include "hdb_to_ast.hpp"
#include "hdb_grammar.hpp"

#include <ssc/macros.hpp>

hdb::AST hdb::parse(const std::string& contents)
{
    hdb::grammar g; // Our grammar
    hdb::AST ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator begin = contents.begin(), end = contents.end();

    qi::phrase_parse(begin, end, g.ast, space, ast);

    return ast;
}

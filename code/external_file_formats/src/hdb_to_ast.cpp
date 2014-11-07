/*
 * hdb_to_ast.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#include "hdb_to_ast.hpp"
#include "hdb_grammar.hpp"

#include <ssc/macros.hpp>
//
//distributor parsed;
//        qi::rule<std::string::const_iterator, std::string()> string_rule =
//                    +~qi::char_(',');
//        qi::rule<std::string::const_iterator, distributor()> rule =
//                    (qi::int_ | string_rule)%qi::lit(',');
//        std::string test="abc,1,2,def,ghi,3,jkl";
//        std::string::const_iterator iter=test.begin(), end=test.end();
//        bool result = qi::parse(iter,end,rule,parsed);


hdb::AST hdb::parse(const std::string& contents)
{
    typedef hdb_grammar<std::string::const_iterator> grammar;
    grammar g; // Our grammar
    hdb::AST ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator begin = contents.begin(), end = contents.end();
    bool success = qi::phrase_parse(begin, end, g.ast, space, ast);
//    COUT(success);
//
//    if(success && begin==end)
//    {
//        std::cout << "Success." << std::endl;
//    }
//    else
//    {
//        std::cout << "Failure." << std::endl;
//        //std::cout << "Unparsed: " << std::string(begin,end) << std::endl;
//    }
    return ast;
}

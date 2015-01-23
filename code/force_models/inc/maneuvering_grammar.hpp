/*
 * maneuvering_grammar.hpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#ifndef MANEUVERING_GRAMMAR_HPP_
#define MANEUVERING_GRAMMAR_HPP_

#include "boost_spirit_includes.hpp"
#include <boost/spirit/include/phoenix_core.hpp>

#include "ManeuveringInternal.hpp"

typedef ascii::blank_type SpaceType;

namespace maneuvering
{
    struct nil {};
    struct expression;
    struct function;

    typedef std::string identifier;

    typedef boost::variant<
                nil
              , identifier
              , boost::recursive_wrapper<function>
              , double
            >
        atom;


    typedef boost::variant<
                        nil
                      , boost::recursive_wrapper<atom>
                      , boost::recursive_wrapper<expression>
                      , double
                    >
                term;

    struct expression
    {
        std::string operator_;
        term lhs;
        term rhs;
    };

    struct function
    {
        std::string identifier;
        term operand;
    };

    struct whole_equation
    {
        std::list<term> rest;
    };


    template <typename Iterator>
    struct Grammar : qi::grammar<Iterator, std::vector<double>(), SpaceType>
    {
        //using qi::_1;
        Grammar() : Grammar::base_type(values_for_tests)
        {
            values_for_tests                          %= double_ >> +double_ >> -eol;
            //constant                          %= double_[&maneuvering::make_constant];
        }
        qi::rule<Iterator, std::vector<double>(), SpaceType>                     values_for_tests;
        //qi::rule<Iterator, NodePtr(), SpaceType>                     constant;
       // qi::rule<Iterator, NodePtr(), SpaceType>                     constant;

    };
    typedef Grammar<std::string::const_iterator> grammar;
}

#endif  /* MANEUVERING_GRAMMAR_HPP_ */

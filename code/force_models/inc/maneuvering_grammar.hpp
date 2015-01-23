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
    struct Nil {};
    struct Expression;
    struct Functional;

    typedef std::string Identifier;

    typedef boost::variant<
                Nil
              , Identifier
              , boost::recursive_wrapper<Functional>
              , double
            >
        Atom;

    typedef boost::variant<
                        Nil
                      , boost::recursive_wrapper<Atom>
                      , boost::recursive_wrapper<Expression>
                      , double
                    >
                Term;



    struct Expression
    {
        Term lhs;
        std::string operator_name;
        Term rhs;
    };

    struct Functional
    {
        std::string identifier;
        Term operand;
    };

}

    BOOST_FUSION_ADAPT_STRUCT(
            maneuvering::Expression,
        (maneuvering::Term, lhs)
        (std::string, operator_name)
        (maneuvering::Term, rhs)
    )

    BOOST_FUSION_ADAPT_STRUCT(
            maneuvering::Functional,
            (std::string, identifier)
            (maneuvering::Term, operand)
        )

namespace maneuvering
{
    template <typename Iterator>
    struct SimpleGrammar : qi::grammar<Iterator, std::vector<double>(), SpaceType>
    {
        SimpleGrammar() : SimpleGrammar::base_type(values_for_tests)
        {
            values_for_tests                          %= double_ >> +double_ >> -eol;
        }
        qi::rule<Iterator, std::vector<double>(), SpaceType>                     values_for_tests;
    };
    typedef SimpleGrammar<std::string::const_iterator> simple_grammar;


    using boost::spirit::qi::hold;
    template <typename Iterator>
    struct Grammar : qi::grammar<Iterator, maneuvering::Term(), SpaceType>
    {
        //using qi::_1;

        Grammar() : Grammar::base_type(ast)
        {
            identifier = +qi::char_("_a-zA-Z");
            addop      = qi::char_("+") | qi::char_("-");
            mulop      = qi::char_("*") | qi::char_("/");
            constant   = double_;
            functional = identifier >> '(' >> term >> ')';
            expression = term >> addop >> term;
            term       = atom | expression | constant;
            factor     = atom | ('(' >> term >> ')');
            atom       = functional | identifier | constant;
            using qi::debug;

        }
        qi::rule<Iterator, Term(), SpaceType>                          ast;
        qi::rule<Iterator, std::string(), SpaceType>                   addop;
        qi::rule<Iterator, std::string(), SpaceType>                   mulop;
        qi::rule<Iterator, std::string(), SpaceType>                   identifier;
        qi::rule<Iterator, double(), SpaceType>                        constant;
        qi::rule<Iterator, Functional(), SpaceType>                    functional;
        qi::rule<Iterator, Expression(), SpaceType>                    expression;
        qi::rule<Iterator, Atom(), SpaceType>                          atom;
        qi::rule<Iterator, Term(), SpaceType>                          term;
        qi::rule<Iterator, Term(), SpaceType>                          factor;

    };
    typedef Grammar<std::string::const_iterator> grammar;
}

#endif  /* MANEUVERING_GRAMMAR_HPP_ */

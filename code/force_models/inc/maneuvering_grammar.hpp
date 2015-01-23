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

    typedef boost::variant<
                            Nil
                          , boost::recursive_wrapper<Atom>
                          , boost::recursive_wrapper<Expression>
                          , double
                        >
    ExpressionNode;

    struct Expression
    {
        std::string operator_name;
        std::vector<ExpressionNode> children;
    };

    struct Functional
    {
        std::string identifier;
        Expression operand;
    };
}
    BOOST_FUSION_ADAPT_STRUCT(
            maneuvering::Expression,
        (std::string, operator_)
        (std::vector<maneuvering::ExpressionNode>, children)
    )

    BOOST_FUSION_ADAPT_STRUCT(
            maneuvering::Functional,
            (std::string, identifier)
            (maneuvering::Expression, operand)
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



    template <typename Iterator>
    struct Grammar : qi::grammar<Iterator, maneuvering::Term(), SpaceType>
    {
        //using qi::_1;
        Grammar() : Grammar::base_type(ast)
        {
            identifier = qi::lexeme[qi::char_("_a-zA-Z") >> +(qi::char_("-_a-zA-Z0-9+"))];
            constant   = double_;
            functional = identifier >> lit('(') >> expression >> lit(')');
//            using qi::debug;
//            BOOST_SPIRIT_DEBUG_NODE(ast);
        }
        qi::rule<Iterator, Term(), SpaceType> ast;
        qi::rule<Iterator, Expression(), SpaceType>                    expression;
        qi::rule<Iterator, Functional(), SpaceType>                    functional;
//        qi::rule<Iterator, Term(), SpaceType>                          term;
//        qi::rule<Iterator, Atom(), SpaceType>                          atom;
        qi::rule<Iterator, std::string(), SpaceType>                   identifier;
        qi::rule<Iterator, double(), SpaceType>                        constant;
    };
    typedef Grammar<std::string::const_iterator> grammar;
}

#endif  /* MANEUVERING_GRAMMAR_HPP_ */

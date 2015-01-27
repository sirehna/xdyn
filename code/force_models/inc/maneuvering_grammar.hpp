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

namespace maneuvering
{
    typedef ascii::blank_type SpaceType;

    struct Nil
    {
    };

    struct FunctionCall;

    typedef std::string Identifier;

    typedef boost::variant<
                    Nil
                  , double
                  , Identifier
                  , boost::recursive_wrapper<FunctionCall>
                >
            Atom;

    struct Expr;

    typedef boost::variant<
                    Nil
                  , boost::recursive_wrapper<Expr>
                  , boost::recursive_wrapper<Atom>
                >
            Base;

    struct Factor
    {
        Base base;
        std::vector<Base> exponents;
    };

    struct OperatorAndFactor
    {
        std::string operator_;
        Factor factor;
    };

    struct Term
    {
        Factor first;
        std::vector<OperatorAndFactor> rest;
    };

    struct OperatorAndTerm
    {
        std::string operator_;
        Term term;
    };

    struct Expr
    {
        Term first;
        std::vector<OperatorAndTerm> rest;
    };

    struct FunctionCall
    {
        Identifier function;
        Expr expr;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::Factor,
    (maneuvering::Base, base)
    (std::vector<maneuvering::Base>, exponents)
)

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::OperatorAndTerm,
    (std::string, operator_)
    (maneuvering::Term, term)
)

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::OperatorAndFactor,
    (std::string, operator_)
    (maneuvering::Factor, factor)
)

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::Term,
    (maneuvering::Factor, first)
    (std::vector<maneuvering::OperatorAndFactor>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::Expr,
    (maneuvering::Term, first)
    (std::vector<maneuvering::OperatorAndTerm>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
        maneuvering::FunctionCall,
    (maneuvering::Identifier, function)
    (maneuvering::Expr, expr)
)

namespace maneuvering
{
    struct ArithmeticGrammar : qi::grammar<std::string::const_iterator, Expr(), SpaceType>
    {
        ArithmeticGrammar() : ArithmeticGrammar::base_type(expr)
        {
            using boost::spirit::ascii::alnum;
            using boost::spirit::ascii::alpha;
            expr     = term >> *(operator_and_term);
            add_operators = qi::char_("+") | qi::char_("-");
            mul_operators = qi::char_("*") | qi::char_("/");
            operator_and_term = add_operators >> term;
            operator_and_factor = mul_operators >> factor;
            term     = factor >> *(operator_and_factor);
            factor   = base >> *( '^' >> exponent);
            base     = ('(' >> expr >> ')') | atom;
            exponent = base;
            atom   = function_call | identifier | double_;
            function_call = identifier >> '(' >> expr >> ')';
            identifier = alpha >> *(alnum | qi::char_('_'));
        }

        qi::rule<std::string::const_iterator, Expr(), SpaceType>              expr;
        qi::rule<std::string::const_iterator, std::string(), SpaceType>       add_operators;
        qi::rule<std::string::const_iterator, std::string(), SpaceType>       mul_operators;
        qi::rule<std::string::const_iterator, OperatorAndTerm(), SpaceType>   operator_and_term;
        qi::rule<std::string::const_iterator, OperatorAndFactor(), SpaceType> operator_and_factor;
        qi::rule<std::string::const_iterator, Term(), SpaceType>              term;
        qi::rule<std::string::const_iterator, Factor(), SpaceType>            factor;
        qi::rule<std::string::const_iterator, Base(), SpaceType>              base;
        qi::rule<std::string::const_iterator, Base(), SpaceType>              exponent;
        qi::rule<std::string::const_iterator, Atom(), SpaceType>              atom;
        qi::rule<std::string::const_iterator, FunctionCall(), SpaceType>      function_call;
        qi::rule<std::string::const_iterator, Identifier(), SpaceType>        identifier;
    };
}

#endif  /* MANEUVERING_GRAMMAR_HPP_ */

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
            mulop      = qi::char_("*") | qi::char_("/") | qi::char_("^");
            constant   = double_;
            functional = identifier >> '(' >> term >> ')';
            add_expression = term >> addop >> factor
                           |  add_expression  >> addop >> term
                           |  term  >> addop >> mul_expression
                           |  add_expression  >> addop >> mul_expression;
            mul_expression = term >> mulop >> factor;
            term       = atom | mul_expression | constant;
            factor     = atom | ('(' >> term >> ')');
            atom       = functional | identifier | constant;
            expression = atom;//add_expression | mul_expression | factor | term;
            using qi::debug;

        }
        qi::rule<Iterator, Term(), SpaceType>                          ast;
        qi::rule<Iterator, std::string(), SpaceType>                   addop;
        qi::rule<Iterator, std::string(), SpaceType>                   mulop;
        qi::rule<Iterator, std::string(), SpaceType>                   identifier;
        qi::rule<Iterator, double(), SpaceType>                        constant;
        qi::rule<Iterator, Functional(), SpaceType>                    functional;
        qi::rule<Iterator, Expression(), SpaceType>                    add_expression;
        qi::rule<Iterator, Expression(), SpaceType>                    mul_expression;
        qi::rule<Iterator, Atom(), SpaceType>                          atom;
        qi::rule<Iterator, Term(), SpaceType>                          term;
        qi::rule<Iterator, Term(), SpaceType>                          factor;
        qi::rule<Iterator, Term(), SpaceType>                          expression;

    };
    typedef Grammar<std::string::const_iterator> grammar;
}



struct Nil
{
};
typedef boost::variant<
                Nil
              , double
            >
        Atom;

struct Expr;
typedef boost::variant<
                Nil
              , boost::recursive_wrapper<Expr>
              , double
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

BOOST_FUSION_ADAPT_STRUCT(
        Factor,
    (Base, base)
    (std::vector<Base>, exponents)
)

BOOST_FUSION_ADAPT_STRUCT(
        OperatorAndTerm,
    (std::string, operator_)
    (Term, term)
)

BOOST_FUSION_ADAPT_STRUCT(
        OperatorAndFactor,
    (std::string, operator_)
    (Factor, factor)
)

BOOST_FUSION_ADAPT_STRUCT(
        Term,
    (Factor, first)
    (std::vector<OperatorAndFactor>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
        Expr,
    (Term, first)
    (std::vector<OperatorAndTerm>, rest)
)

struct ArithmeticGrammar : qi::grammar<std::string::const_iterator, Expr(), SpaceType>
{
    ArithmeticGrammar() : ArithmeticGrammar::base_type(expr)
    {
        expr     = term >> *(operator_and_term);
        add_operators = qi::char_("+") | qi::char_("-");
        mul_operators = qi::char_("*") | qi::char_("/");
        operator_and_term = add_operators >> term;
        operator_and_factor = mul_operators >> factor;
        term     = factor >> *(operator_and_factor);
        factor   = base >> *( '^' >> exponent);
        base     = ('(' >> expr >> ')') | atom;
        exponent = base;
        atom   = double_;
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
};


#endif  /* MANEUVERING_GRAMMAR_HPP_ */

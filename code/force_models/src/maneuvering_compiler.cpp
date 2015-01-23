/*
 * maneuvering_compiler.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#include "ManeuveringInternal.hpp"
#include "maneuvering_compiler.hpp"
#include "maneuvering_grammar.hpp"

using namespace maneuvering;
using boost::spirit::ascii::blank;

namespace maneuvering
{
    class AtomVisitor: public boost::static_visitor<NodePtr>
    {
        public:
            template <typename T> NodePtr operator()(const T& ) const
            {
                COUT("");
                return make_constant(0);
            }
    };

    template <> NodePtr AtomVisitor::operator()(const double& d) const;
    template <> NodePtr AtomVisitor::operator()(const double& d) const
    {
        COUT("");
        return make_constant(d);
    }

    template <> NodePtr AtomVisitor::operator()(const Identifier& name) const;
    template <> NodePtr AtomVisitor::operator()(const Identifier& name) const
    {
        COUT("");
        return make_unknown_identifier(name);
    }

    class TermVisitor: public boost::static_visitor<NodePtr>
    {
        public:
            template <typename T> NodePtr operator()(const T& ) const
            {
                COUT("");
                return make_constant(0);
            }
    };

    template <> NodePtr TermVisitor::operator()(const double& d) const;
    template <> NodePtr TermVisitor::operator()(const double& d) const
    {
        COUT("");
        return make_constant(d);
    }

    template <> NodePtr TermVisitor::operator()(const Atom& d) const;
    template <> NodePtr TermVisitor::operator()(const Atom& d) const
    {
        COUT("");
        AtomVisitor visitor;
        return boost::apply_visitor(visitor, d);
    }
}

NodePtr maneuvering::compile(const std::string& expression)
{
    std::string::const_iterator b = expression.begin(), e = expression.end();
    maneuvering::Term ast;
    maneuvering::grammar g;
    COUT("");
    qi::phrase_parse(b, e, g.expression, blank, ast);
    COUT("");
    TermVisitor visitor;
    return boost::apply_visitor(visitor, ast);
}


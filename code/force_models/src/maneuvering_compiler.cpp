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
    /*
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
    */

    class Evaluator: public boost::static_visitor<NodePtr>
    {
        public:
            //template <typename T> NodePtr operator()(const T& ) const;
            NodePtr operator()(const Nil& ) const
            {
                COUT("");
                return make_constant(std::nan(""));
            }
            NodePtr operator()(const double& d) const
            {
                COUT("");
                return make_constant(d);
            }
            NodePtr operator()(const Identifier& name) const
            {
                COUT("");
                if (name == "t") return make_time();
                                 return make_unknown_identifier(name);
            }
            NodePtr operator()(const Base& d) const
            {
                COUT("");
                return boost::apply_visitor(*this,d);
            }
            NodePtr operator()(const Factor& d) const
            {
                COUT("");
                const NodePtr b = this->operator ()(d.base);
                COUT(b);
                std::vector<NodePtr> exponents;
                COUT(d.exponents.size());
                for (auto e:d.exponents){ exponents.push_back(this->operator()(e));COUT(exponents.back());}
                NodePtr ret = b;
                for (auto e:exponents) ret = make_pow(ret, e);
                return ret;
            }
            NodePtr operator()(const ::Term& d) const
            {
                COUT("");
                NodePtr ret = this->operator ()(d.first);
                for (auto op:d.rest)
                {
                    NodePtr val = this->operator()(op.factor);
                    if (op.operator_ == "-") ret = make_difference(ret,val);
                    if (op.operator_ == "+") ret = make_sum(ret,val);
                    if (op.operator_ == "*") ret = make_multiply(ret,val);
                    if (op.operator_ == "/") ret = make_divide(ret,val);
                }
                return ret;
            }
            NodePtr operator()(const ::Expr& d) const
            {
                COUT("");
                NodePtr ret = this->operator ()(d.first);
                for (auto op:d.rest)
                {
                    NodePtr val = this->operator()(op.term);
                    if (op.operator_ == "-") ret = make_difference(ret,val);
                    if (op.operator_ == "+") ret = make_sum(ret,val);
                    if (op.operator_ == "*") ret = make_multiply(ret,val);
                    if (op.operator_ == "/") ret = make_divide(ret,val);
                }
                return ret;
            }
            NodePtr operator()(const Atom& d) const
            {
                COUT("");
                return boost::apply_visitor(*this,d);
            }
            NodePtr operator()(const FunctionCall& d) const
            {
                COUT("");
                if (d.function == "cos")  return make_cos    (this->operator()(d.expr));
                if (d.function == "sin")  return make_sin    (this->operator()(d.expr));
                if (d.function == "exp")  return make_exp    (this->operator()(d.expr));
                if (d.function == "abs")  return make_abs    (this->operator()(d.expr));
                if (d.function == "log")  return make_log    (this->operator()(d.expr));
                if (d.function == "sqrt") return make_sqrt   (this->operator()(d.expr));
                if (d.function == "x")    return make_state_x(this->operator()(d.expr));
                if (d.function == "y")    return make_state_y(this->operator()(d.expr));
                if (d.function == "z")    return make_state_z(this->operator()(d.expr));
                if (d.function == "u")    return make_state_u(this->operator()(d.expr));
                if (d.function == "v")    return make_state_v(this->operator()(d.expr));
                if (d.function == "w")    return make_state_w(this->operator()(d.expr));
                if (d.function == "p")    return make_state_p(this->operator()(d.expr));
                if (d.function == "q")    return make_state_q(this->operator()(d.expr));
                if (d.function == "r")    return make_state_r(this->operator()(d.expr));
                                          return make_constant(0);
            }
    };
/*
    template <> NodePtr Evaluator::operator()(const Nil& d) const;
    template <> NodePtr Evaluator::operator()(const double& d) const;
    template <> NodePtr Evaluator::operator()(const Identifier& d) const;
    template <> NodePtr Evaluator::operator()(const Base& d) const;
    template <> NodePtr Evaluator::operator()(const Factor& d) const;
    template <> NodePtr Evaluator::operator()(const Term& d) const;
    template <> NodePtr Evaluator::operator()(const Expr& d) const;
    template <> NodePtr Evaluator::operator()(const Atom& d) const;
    template <> NodePtr Evaluator::operator()(const FunctionCall& d) const;


    template <> NodePtr Evaluator::operator()(const Nil& ) const
    {
        COUT("");
        return make_constant(std::nan(""));
    }
    template <> NodePtr Evaluator::operator()(const double& d) const
    {
        COUT("");
        return make_constant(d);
    }
    template <> NodePtr Evaluator::operator()(const Identifier& name) const
    {
        COUT("");
        if (name == "t") return make_time();
                         return make_unknown_identifier(name);
    }
    template <> NodePtr Evaluator::operator()(const Base& d) const
    {
        COUT("");
        return boost::apply_visitor(*this,d);
    }
    template <> NodePtr Evaluator::operator()(const Factor& d) const
    {
        COUT("");
        const NodePtr b = this->operator ()(d.base);
        COUT(b);
        std::vector<NodePtr> exponents;
        COUT(d.exponents.size());
        for (auto e:d.exponents){ exponents.push_back(this->operator()(e));COUT(exponents.back());}
        NodePtr ret = b;
        for (auto e:exponents) ret = make_pow(ret, e);
        return ret;
    }
    template <> NodePtr Evaluator::operator()(const ::Term& d) const
    {
        COUT("");
        NodePtr ret = this->operator ()(d.first);
        for (auto op:d.rest)
        {
            NodePtr val = this->operator()(op.factor);
            if (op.operator_ == "-") ret = make_difference(ret,val);
            if (op.operator_ == "+") ret = make_sum(ret,val);
            if (op.operator_ == "*") ret = make_multiply(ret,val);
            if (op.operator_ == "/") ret = make_divide(ret,val);
        }
        return ret;
    }
    template <> NodePtr Evaluator::operator()(const ::Expr& d) const
    {
        COUT("");
        NodePtr ret = this->operator ()(d.first);
        for (auto op:d.rest)
        {
            NodePtr val = this->operator()(op.term);
            if (op.operator_ == "-") ret = make_difference(ret,val);
            if (op.operator_ == "+") ret = make_sum(ret,val);
            if (op.operator_ == "*") ret = make_multiply(ret,val);
            if (op.operator_ == "/") ret = make_divide(ret,val);
        }
        return ret;
    }
    template <> NodePtr Evaluator::operator()(const Atom& d) const
    {
        COUT("");
        return boost::apply_visitor(*this,d);
    }
    template <> NodePtr Evaluator::operator()(const FunctionCall& d) const
    {
        COUT("");
        if (d.function == "cos")  return make_cos    (this->operator()(d.expr));
        if (d.function == "sin")  return make_sin    (this->operator()(d.expr));
        if (d.function == "exp")  return make_exp    (this->operator()(d.expr));
        if (d.function == "abs")  return make_abs    (this->operator()(d.expr));
        if (d.function == "log")  return make_log    (this->operator()(d.expr));
        if (d.function == "sqrt") return make_sqrt   (this->operator()(d.expr));
        if (d.function == "x")    return make_state_x(this->operator()(d.expr));
        if (d.function == "y")    return make_state_y(this->operator()(d.expr));
        if (d.function == "z")    return make_state_z(this->operator()(d.expr));
        if (d.function == "u")    return make_state_u(this->operator()(d.expr));
        if (d.function == "v")    return make_state_v(this->operator()(d.expr));
        if (d.function == "w")    return make_state_w(this->operator()(d.expr));
        if (d.function == "p")    return make_state_p(this->operator()(d.expr));
        if (d.function == "q")    return make_state_q(this->operator()(d.expr));
        if (d.function == "r")    return make_state_r(this->operator()(d.expr));
                                  return make_constant(0);
    }*/
}




NodePtr maneuvering::compile(const std::string& expression)
{
    std::string::const_iterator b = expression.begin(), e = expression.end();
    Expr ast;
    ArithmeticGrammar g;
    COUT("");
    qi::phrase_parse(b, e, g.expr, blank, ast);
    COUT("");
    Evaluator evaluate;
    return evaluate(ast);
}


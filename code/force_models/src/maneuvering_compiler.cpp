/*
 * maneuvering_compiler.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#include "maneuvering_compiler.hpp"
#include "ManeuveringInternal.hpp"
#include "maneuvering_grammar.hpp"

using namespace maneuvering;
using boost::spirit::ascii::blank;

namespace maneuvering
{
    class PrettyPrinter : public boost::static_visitor<std::string>
    {
        public:
            std::string operator()(const Nil& ) const
            {
                return "NaN";
            }
            std::string operator()(const double& d) const
            {
                std::stringstream ss;
                ss << d;
                return ss.str();
            }
            std::string operator()(const Identifier& name) const
            {
                return name;
            }
            std::string operator()(const Base& d) const
            {
                return boost::apply_visitor(*this,d);
            }
            std::string operator()(const Factor& d) const
            {
                const std::string b = this->operator ()(d.base);
                std::vector<std::string> exponents;
                for (auto e:d.exponents) exponents.push_back(this->operator()(e));
                std::string ret = b;
                for (auto e:exponents) ret = std::string("(") + ret + ")^(" + e + ")";
                return ret;
            }
            std::string operator()(const ::Term& d) const
            {
                std::string ret = this->operator ()(d.first);
                for (auto op:d.rest)
                {
                    std::string val = this->operator()(op.factor);
                    ret = ret + op.operator_ + "(" + val + ")";
                }
                return ret;
            }
            std::string operator()(const ::Expr& d) const
            {
                std::string ret = this->operator ()(d.first);
                for (auto op:d.rest)
                {
                    std::string val = this->operator()(op.term);
                    ret = ret + op.operator_ + "(" + val + ")";
                }
                return ret;
            }
            std::string operator()(const Atom& d) const
            {
                return boost::apply_visitor(*this,d);
            }
            std::string operator()(const FunctionCall& d) const
            {
                return d.function + "(" + this->operator()(d.expr) + ")";
            }
    };

    class Evaluator: public boost::static_visitor<NodePtr>
    {
        public:
            Evaluator(const YamlRotation& rot_) : rot(rot_) {}
            NodePtr operator()(const Nil& ) const
            {
                return make_constant(std::nan(""));
            }
            NodePtr operator()(const double& d) const
            {
                return make_constant(d);
            }
            NodePtr operator()(const Identifier& name) const
            {
                if (name == "t") return make_time();
                                 return make_unknown_identifier(name);
            }
            NodePtr operator()(const Base& d) const
            {
                return boost::apply_visitor(*this,d);
            }
            NodePtr operator()(const Factor& d) const
            {
                const NodePtr b = this->operator ()(d.base);
                std::vector<NodePtr> exponents;
                for (auto e:d.exponents) exponents.push_back(this->operator()(e));
                NodePtr ret = b;
                for (auto e:exponents) ret = make_pow(ret, e);
                return ret;
            }
            NodePtr operator()(const ::Term& d) const
            {
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
                return boost::apply_visitor(*this,d);
            }
            NodePtr operator()(const FunctionCall& d) const
            {
                if (d.function == "cos")  return make_cos    (this->operator()(d.expr));
                if (d.function == "sin")  return make_sin    (this->operator()(d.expr));
                if (d.function == "exp")  return make_exp    (this->operator()(d.expr));
                if (d.function == "abs")  return make_abs    (this->operator()(d.expr));
                if (d.function == "log")  return make_log    (this->operator()(d.expr));
                if (d.function == "sqrt") return make_sqrt   (this->operator()(d.expr));
                if (d.function == "x")    return make_state_x(this->operator()(d.expr), rot);
                if (d.function == "y")    return make_state_y(this->operator()(d.expr), rot);
                if (d.function == "z")    return make_state_z(this->operator()(d.expr), rot);
                if (d.function == "u")    return make_state_u(this->operator()(d.expr), rot);
                if (d.function == "v")    return make_state_v(this->operator()(d.expr), rot);
                if (d.function == "w")    return make_state_w(this->operator()(d.expr), rot);
                if (d.function == "p")    return make_state_p(this->operator()(d.expr), rot);
                if (d.function == "q")    return make_state_q(this->operator()(d.expr), rot);
                if (d.function == "r")    return make_state_r(this->operator()(d.expr), rot);
                if (d.function == "phi")  return make_state_phi(this->operator()(d.expr), rot);
                if (d.function == "theta")return make_state_theta(this->operator()(d.expr), rot);
                if (d.function == "psi")  return make_state_psi(this->operator()(d.expr), rot);
                if (d.function == "qr")   return make_state_qr(this->operator()(d.expr), rot);
                if (d.function == "qi")   return make_state_qi(this->operator()(d.expr), rot);
                if (d.function == "qj")   return make_state_qj(this->operator()(d.expr), rot);
                if (d.function == "qk")   return make_state_qk(this->operator()(d.expr), rot);
                                          return make_unknown_identifier(PrettyPrinter()(d));
            }
            YamlRotation rot;
    };

    NodePtr compile(const std::string& expression, const YamlRotation& rot)
    {
        std::string::const_iterator b = expression.begin(), e = expression.end();
        Expr ast;
        ArithmeticGrammar g;
        qi::phrase_parse(b, e, g.expr, blank, ast);
        Evaluator evaluate(rot);
        return evaluate(ast);
    }

    std::string print(const std::string& expression)
    {
        std::string::const_iterator b = expression.begin(), e = expression.end();
        Expr ast;
        ArithmeticGrammar g;
        qi::phrase_parse(b, e, g.expr, blank, ast);
        PrettyPrinter pretty_print;
        return pretty_print(ast);
    }

    double get_Tmax(const NodePtr& node)
    {
        FindTmax find_Tmax;
        node->accept(find_Tmax);
        return find_Tmax.get_Tmax();
    }
}



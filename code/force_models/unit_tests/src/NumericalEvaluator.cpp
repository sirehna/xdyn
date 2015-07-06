/*
 * NumericalEvaluator.cpp
 *
 *  Created on: Jan 26, 2015
 *      Author: cady
 */


#include "NumericalEvaluator.hpp"

template <> double NumericalEvaluator::operator()(const double& d) const
{
    return d;
}

template <> double NumericalEvaluator::operator()(const maneuvering::Identifier& d) const
{
    return std::nan("");
}

template <> double NumericalEvaluator::operator()(const maneuvering::Base& d) const
{
    return boost::apply_visitor(*this,d);
}

template <> double NumericalEvaluator::operator()(const maneuvering::Factor& d) const
{
    const double b = this->operator ()(d.base);
    std::vector<double> exponents;
    for (auto e:d.exponents) exponents.push_back(this->operator()(e));
    double ret = b;
    for (auto e:exponents) ret = std::pow(ret, e);
    return ret;
}


template <> double NumericalEvaluator::operator()(const maneuvering::Term& d) const
{
    double ret = this->operator ()(d.first);
    for (auto op:d.rest)
    {
        const double val = this->operator()(op.factor);
        if (op.operator_ == "-") ret -= val;
        if (op.operator_ == "+") ret += val;
        if (op.operator_ == "*") ret *= val;
        if (op.operator_ == "/") ret /= val;
    }
    return ret;
}

template <> double NumericalEvaluator::operator()(const maneuvering::Expr& d) const
{
    double ret = this->operator ()(d.first);
    for (auto op:d.rest)
    {
        const double val = this->operator()(op.term);
        if (op.operator_ == "-") ret -= val;
        if (op.operator_ == "+") ret += val;
        if (op.operator_ == "*") ret *= val;
        if (op.operator_ == "/") ret /= val;
    }
    return ret;
}

template <> double NumericalEvaluator::operator()(const maneuvering::Atom& d) const
{
    return boost::apply_visitor(*this,d);
}

template <> double NumericalEvaluator::operator()(const maneuvering::FunctionCall& d) const
{
    if (d.function == "cos")  return std::cos(this->operator()(d.expr));
    if (d.function == "sin")  return std::sin(this->operator()(d.expr));
    if (d.function == "exp")  return std::exp(this->operator()(d.expr));
    if (d.function == "abs")  return std::abs(this->operator()(d.expr));
    if (d.function == "sqrt") return std::sqrt(this->operator()(d.expr));
                              return std::nan("");
}

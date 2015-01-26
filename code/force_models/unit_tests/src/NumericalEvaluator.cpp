/*
 * NumericalEvaluator.cpp
 *
 *  Created on: Jan 26, 2015
 *      Author: cady
 */


#include "NumericalEvaluator.hpp"
#include <ssc/macros.hpp>

template <> double NumericalEvaluator::operator()(const double& d) const
{
    COUT("");
    return d;
}

template <> double NumericalEvaluator::operator()(const std::string& d) const
{
    COUT("");
    return std::nan("");
}

template <> double NumericalEvaluator::operator()(const Base& d) const
{
    COUT("");
    return boost::apply_visitor(*this,d);
}

template <> double NumericalEvaluator::operator()(const Factor& d) const
{
    COUT("");
    const double b = this->operator ()(d.base);
    COUT(b);
    std::vector<double> exponents;
    COUT(d.exponents.size());
    for (auto e:d.exponents){ exponents.push_back(this->operator()(e));COUT(exponents.back());}
    double ret = b;
    for (auto e:exponents) ret = std::pow(ret, e);
    return ret;
}


template <> double NumericalEvaluator::operator()(const Term& d) const
{
    COUT("");
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

template <> double NumericalEvaluator::operator()(const Expr& d) const
{
    COUT("");
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

template <> double NumericalEvaluator::operator()(const Atom& d) const
{
    return boost::apply_visitor(*this,d);
}

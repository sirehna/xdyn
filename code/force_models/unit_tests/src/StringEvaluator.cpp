/*
 * StringEvaluator.cpp
 *
 *  Created on: Jan 26, 2015
 *      Author: cady
 */

#include "StringEvaluator.hpp"


StringEvaluator::StringEvaluator(std::stringstream& os_) : os(os_)
{
}

template <> void StringEvaluator::operator()(const double& d)
{
    os << d;
}

template <> void StringEvaluator::operator()(const Identifier& d)
{
    os << d;
}

template <> void StringEvaluator::operator()(const Base& d)
{
    boost::apply_visitor(*this,d);
}

template <> void StringEvaluator::operator()(const Factor& d)
{
    COUT("");
    this->operator ()(d.base);
    COUT(d.exponents.size());
    for (auto e:d.exponents)
    {
        os << "^(";
        this->operator ()(e);
    }
    for (auto e:d.exponents) os << ")";
}
template <> void StringEvaluator::operator()(const Term& d)
{
    COUT("");
    this->operator ()(d.first);
    for (auto op:d.rest)
    {
        os << op.operator_ << "(";
        this->operator()(op.factor);
        os << ")";
    }
}

template <> void StringEvaluator::operator()(const Expr& d)
{
    COUT("");
    this->operator ()(d.first);
    for (auto op:d.rest)
    {
        os << op.operator_;
        this->operator()(op.term);
    }
}

template <> void StringEvaluator::operator()(const FunctionCall& d)
{
    COUT("");
    os << d.function << "(";
    this->operator ()(d.expr);
    os << ")";
}

template <> void StringEvaluator::operator()(const Atom& d)
{
    boost::apply_visitor(*this,d);
}

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

template <> void StringEvaluator::operator()(const maneuvering::Identifier& d)
{
    os << d;
}

template <> void StringEvaluator::operator()(const maneuvering::Base& d)
{
    boost::apply_visitor(*this,d);
}

template <> void StringEvaluator::operator()(const maneuvering::Factor& d)
{
    this->operator ()(d.base);
    for (auto e:d.exponents)
    {
        os << "^(";
        this->operator ()(e);
    }
    for (auto e:d.exponents) os << ")";
}
template <> void StringEvaluator::operator()(const maneuvering::Term& d)
{
    this->operator ()(d.first);
    for (auto op:d.rest)
    {
        os << op.operator_ << "(";
        this->operator()(op.factor);
        os << ")";
    }
}

template <> void StringEvaluator::operator()(const maneuvering::Expr& d)
{
    this->operator ()(d.first);
    for (auto op:d.rest)
    {
        os << op.operator_;
        this->operator()(op.term);
    }
}

template <> void StringEvaluator::operator()(const maneuvering::FunctionCall& d)
{
    os << d.function << "(";
    this->operator ()(d.expr);
    os << ")";
}

template <> void StringEvaluator::operator()(const maneuvering::Atom& d)
{
    boost::apply_visitor(*this,d);
}

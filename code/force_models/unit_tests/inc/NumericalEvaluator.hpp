/*
 * NumericalEvalutator.hpp
 *
 *  Created on: Jan 26, 2015
 *      Author: cady
 */


#ifndef NUMERICALEVALUATOR_HPP_
#define NUMERICALEVALUATOR_HPP_

#include "maneuvering_grammar.hpp"


class NumericalEvaluator: public boost::static_visitor<double>
{
    public:
        template <typename T> double operator()(const T& ) const
        {
            return std::nan("");
        }
};

template <> double NumericalEvaluator::operator()(const double& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Identifier& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Base& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Factor& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Term& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Expr& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::Atom& d) const;
template <> double NumericalEvaluator::operator()(const maneuvering::FunctionCall& d) const;


#endif  /* NUMERICALEVALUATOR_HPP_ */

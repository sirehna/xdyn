/*
 * StringEvaluator.hpp
 *
 *  Created on: Jan 26, 2015
 *      Author: cady
 */


#ifndef STRINGEVALUATOR_HPP_
#define STRINGEVALUATOR_HPP_

#include <sstream>

#include "maneuvering_grammar.hpp"

class StringEvaluator: public boost::static_visitor<>
{
    public:
        StringEvaluator(std::stringstream& os);
        template <typename T> void operator()(const T& )
        {
            os << "NA";
        }

    private:
        std::stringstream& os;
};

template <> void StringEvaluator::operator()(const double& d);
template <> void StringEvaluator::operator()(const maneuvering::Identifier& d);
template <> void StringEvaluator::operator()(const maneuvering::Base& d);
template <> void StringEvaluator::operator()(const maneuvering::Factor& d);
template <> void StringEvaluator::operator()(const maneuvering::Term& d);
template <> void StringEvaluator::operator()(const maneuvering::Expr& d);
template <> void StringEvaluator::operator()(const maneuvering::FunctionCall& d);
template <> void StringEvaluator::operator()(const maneuvering::Atom& d);

#endif  /* STRINGEVALUATOR_HPP_ */

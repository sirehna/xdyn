/*
 * ManeuveringAST.hpp
 *
 *  Created on: Jan 20, 2015
 *      Author: cady
 */


#ifndef MANEUVERINGAST_HPP_
#define MANEUVERINGAST_HPP_

#include <string>
#include <vector>

namespace maneuvering_AST
{
    struct Op
    {
        Op(const std::string& name, const std::vector<Op>& sons);
        virtual ~Op();
        std::string name;
        std::vector<Op> sons;

        private: Op();

    };

    struct Var : Op
    {
        Var(const std::string& name);
        private: Var();
    };

    struct Binary : Op
    {
        Binary(const std::string& operator_name, const Op& lhs, const Op& rhs);
        private: Binary();
    };

    struct Unary : Op
    {
        Unary(const std::string& operator_name, const Op& son);
        private: Unary();
    };

    struct State : Op
    {
        State(const std::string& name, const Op& operand);
        private: State();
    };
}

#endif  /* MANEUVERINGAST_HPP_ */

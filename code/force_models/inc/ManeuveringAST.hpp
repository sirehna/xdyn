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

namespace maneuvering
{
    struct Op
    {
        Op(const std::string& name, const std::vector<Op>& sons);
        virtual ~Op();
        std::string name;
        std::vector<Op> sons;

        private: Op();

    };

    struct VarNode : Op
    {
        VarNode(const std::string& name);
        private: VarNode();
    };

    struct BinaryNode : Op
    {
        BinaryNode(const std::string& operator_name, const Op& lhs, const Op& rhs);
        private: BinaryNode();
    };

    struct UnaryNode : Op
    {
        UnaryNode(const std::string& operator_name, const Op& son);
        private: UnaryNode();
    };

    struct NullaryNode : Op
    {
        NullaryNode(const std::string& operator_name);
        private: NullaryNode();
    };

    struct StateNode : UnaryNode
    {
        StateNode(const std::string& name, const Op& operand);
        private: StateNode();
    };

    struct ConstantNode : NullaryNode
    {
        ConstantNode(const double value);
        double get_value() const;
        private:
            ConstantNode();
            double value;
    };
}

#endif  /* MANEUVERINGAST_HPP_ */

/*
 * ManeuveringInternal.hpp
 *
 *  Created on: Jan 21, 2015
 *      Author: cady
 */


#ifndef MANEUVERINGINTERNAL_HPP_
#define MANEUVERINGINTERNAL_HPP_

#include <functional>

#include <ssc/data_source.hpp>

#include "BodyStates.hpp"


namespace maneuvering
{
    typedef std::function<double(const BodyStates&, ssc::data_source::DataSource&, const double)> Function;
    class Node;
    typedef TR1(shared_ptr)<Node> NodePtr;
    class Node
    {
        public:
            Node(const std::vector<NodePtr>& children);
            virtual ~Node();
            virtual Function get_lambda() const = 0;
            std::vector<NodePtr> get_children() const;

        //protected:
            std::vector<NodePtr> children;

        private:
            Node();
    };

    class Nullary : public Node
    {
        public:
            Nullary();
    };

    class Binary : public Node
    {
        public:
            Binary(const NodePtr& lhs, const NodePtr& rhs);

        protected:
            NodePtr get_lhs() const;
            NodePtr get_rhs() const;

        private:
            Binary();
    };

    class Constant : public Nullary
    {
        public:
            Constant(const double val);
            Function get_lambda() const;

        private:
            double val;
    };

    class Unary : public Node
    {
        public:
            Unary(const NodePtr operand);

        protected:
            NodePtr get_operand() const;
    };

    class Cos : public Unary
    {
        public:
            Cos(const NodePtr& operand);
            Function get_lambda() const;
    };

    class Sin : public Unary
    {
        public:
            Sin(const NodePtr& operand);
            Function get_lambda() const;
    };

    class Abs : public Unary
    {
        public:
            Abs(const NodePtr& operand);
            Function get_lambda() const;
    };

    class Log : public Unary
    {
        public:
            Log(const NodePtr& operand);
            Function get_lambda() const;
    };

    class Sum : public Binary
    {
        public:
            Sum(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
    };

    class Pow : public Binary
    {
        public:
            Pow(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
    };

    class Exp : public Unary
    {
        public:
            Exp(const NodePtr& operand);
            Function get_lambda() const;
    };

    class Difference : public Binary
    {
        public:
            Difference(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
    };

    class Divide : public Binary
    {
        public:
            Divide(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
    };

    class Multiply : public Binary
    {
        public:
            Multiply(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
    };

    class StateX : public Unary
    {
        public:
            StateX(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateY : public Unary
    {
        public:
            StateY(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateZ : public Unary
    {
        public:
            StateZ(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateU : public Unary
    {
        public:
            StateU(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateV : public Unary
    {
        public:
            StateV(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateW : public Unary
    {
        public:
            StateW(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateP : public Unary
    {
        public:
            StateP(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateQ : public Unary
    {
        public:
            StateQ(const NodePtr& operand);
            Function get_lambda() const;
    };

    class StateR : public Unary
    {
        public:
            StateR(const NodePtr& operand);
            Function get_lambda() const;
    };

    NodePtr make_constant(const double val);
    NodePtr make_cos(const NodePtr& operand);
    NodePtr make_sin(const NodePtr& operand);
    NodePtr make_abs(const NodePtr& operand);
    NodePtr make_log(const NodePtr& operand);
    NodePtr make_sum(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_pow(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_exp(const NodePtr& operand);
    NodePtr make_difference(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_divide(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_multiply(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_state_x(const NodePtr& operand);
    NodePtr make_state_y(const NodePtr& operand);
    NodePtr make_state_z(const NodePtr& operand);
    NodePtr make_state_u(const NodePtr& operand);
    NodePtr make_state_v(const NodePtr& operand);
    NodePtr make_state_w(const NodePtr& operand);
    NodePtr make_state_p(const NodePtr& operand);
    NodePtr make_state_q(const NodePtr& operand);
    NodePtr make_state_r(const NodePtr& operand);
}

#endif  /* MANEUVERINGINTERNAL_HPP_ */

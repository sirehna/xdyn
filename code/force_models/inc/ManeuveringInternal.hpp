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

        protected:
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

    class Sqrt : public Unary
    {
        public:
            Sqrt(const NodePtr& operand);
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

    enum class StateType {X, Y, Z, U, V, W, P, Q, R};

    template <StateType S> class State : public Unary
    {
        public:
            State(const NodePtr& operand) : Unary(operand){}
            Function get_lambda() const
            {
                return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
                        {
                            const auto op = get_operand()->get_lambda();
                            switch(S)
                            {
                                case StateType::X : return states.x(t-op(states,ds,t));break;
                                case StateType::Y : return states.y(t-op(states,ds,t));break;
                                case StateType::Z : return states.z(t-op(states,ds,t));break;
                                case StateType::U : return states.u(t-op(states,ds,t));break;
                                case StateType::V : return states.v(t-op(states,ds,t));break;
                                case StateType::W : return states.w(t-op(states,ds,t));break;
                                case StateType::P : return states.p(t-op(states,ds,t));break;
                                case StateType::Q : return states.q(t-op(states,ds,t));break;
                                case StateType::R : return states.r(t-op(states,ds,t));break;
                            }
                            return op(states,ds,t);
                        };
            }
    };

    class Time : public Nullary
    {
        public:
            Time();
            Function get_lambda() const;
    };

    class UnknownIdentifier : public Nullary
    {
        public:
            UnknownIdentifier(const std::string& identifier_name);
            Function get_lambda() const;

        private:
            UnknownIdentifier();
            std::string identifier_name;
    };

    NodePtr make_constant(const double val);
    NodePtr make_cos(const NodePtr& operand);
    NodePtr make_sin(const NodePtr& operand);
    NodePtr make_abs(const NodePtr& operand);
    NodePtr make_log(const NodePtr& operand);
    NodePtr make_sum(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_pow(const NodePtr& lhs, const NodePtr& rhs);
    NodePtr make_exp(const NodePtr& operand);
    NodePtr make_sqrt(const NodePtr& operand);
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
    NodePtr make_time();
    NodePtr make_unknown_identifier(const std::string& identifier_name);
}

#endif  /* MANEUVERINGINTERNAL_HPP_ */

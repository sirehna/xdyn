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

    NodePtr make_constant(const double val);
    NodePtr make_cos(const NodePtr& operand);
    NodePtr make_sin(const NodePtr& operand);
    NodePtr make_abs(const NodePtr& operand);
    NodePtr make_log(const NodePtr& operand);
    NodePtr make_sum(const NodePtr& lhs, const NodePtr& rhs);
}

#endif  /* MANEUVERINGINTERNAL_HPP_ */

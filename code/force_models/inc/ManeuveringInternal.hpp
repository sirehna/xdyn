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
}

#endif  /* MANEUVERINGINTERNAL_HPP_ */

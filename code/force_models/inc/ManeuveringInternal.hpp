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

    class AbstractNodeVisitor;

    class Node
    {
        public:
            Node(const std::vector<NodePtr>& children);
            virtual ~Node();
            virtual Function get_lambda() const = 0;
            std::vector<NodePtr> get_children() const;
            virtual void accept(AbstractNodeVisitor& visitor) const = 0;
            virtual double get_max() const = 0;
            virtual double get_min() const = 0;

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
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;

        private:
            double val;
    };

    class Unary : public Node
    {
        public:
            Unary(const NodePtr operand);

            NodePtr get_operand() const;
    };

    class Cos : public Unary
    {
        public:
            Cos(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Sin : public Unary
    {
        public:
            Sin(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Abs : public Unary
    {
        public:
            Abs(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Log : public Unary
    {
        public:
            Log(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Sum : public Binary
    {
        public:
            Sum(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Pow : public Binary
    {
        public:
            Pow(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Exp : public Unary
    {
        public:
            Exp(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Sqrt : public Unary
    {
        public:
            Sqrt(const NodePtr& operand);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Difference : public Binary
    {
        public:
            Difference(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Divide : public Binary
    {
        public:
            Divide(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class Multiply : public Binary
    {
        public:
            Multiply(const NodePtr& lhs, const NodePtr& rhs);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    enum class StateType {X, Y, Z, U, V, W, P, Q, R, QR, QI, QJ, QK, PHI, THETA, PSI};

    template <StateType S> class State : public Unary
    {
        public:
            State(const NodePtr& operand, const YamlRotation& rot_) : Unary(operand), rot(rot_)
            {}
            Function get_lambda() const
            {
                return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)->double
                        {
                            const auto op = get_operand()->get_lambda();
                            ssc::kinematics::RotationMatrix R;
                            ssc::kinematics::EulerAngles angles;
                            if ((S == StateType::PHI) or (S == StateType::THETA) or (S == StateType::PSI))
                            {
                                R = Eigen::Quaternion<double>(states.qr(t-op(states,ds,t)),states.qi(t-op(states,ds,t)),states.qj(t-op(states,ds,t)),states.qk(t-op(states,ds,t))).matrix();
                                angles = BodyStates::convert(R, rot);
                            }
                            switch(S)
                            {
                                case StateType::X :    return states.x(t-op(states,ds,t)); break;
                                case StateType::Y :    return states.y(t-op(states,ds,t)); break;
                                case StateType::Z :    return states.z(t-op(states,ds,t)); break;
                                case StateType::U :    return states.u(t-op(states,ds,t)); break;
                                case StateType::V :    return states.v(t-op(states,ds,t)); break;
                                case StateType::W :    return states.w(t-op(states,ds,t)); break;
                                case StateType::P :    return states.p(t-op(states,ds,t)); break;
                                case StateType::Q :    return states.q(t-op(states,ds,t)); break;
                                case StateType::R :    return states.r(t-op(states,ds,t)); break;
                                case StateType::QR :   return states.qr(t-op(states,ds,t));break;
                                case StateType::QI :   return states.qi(t-op(states,ds,t));break;
                                case StateType::QJ :   return states.qj(t-op(states,ds,t));break;
                                case StateType::QK :   return states.qk(t-op(states,ds,t));break;
                                case StateType::PHI:   return angles.phi;                  break;
                                case StateType::THETA: return angles.theta;                break;
                                case StateType::PSI:   return angles.psi;                  break;
                            }
                            return op(states,ds,t);
                        };
            }


            double get_max() const
            {
                return 1e15;
            }

            double get_min() const
            {
                return -1e15;
            }
            void accept(AbstractNodeVisitor& visitor) const;
            YamlRotation rot;
    };

    class Time : public Nullary
    {
        public:
            Time();
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            double get_max() const;
            double get_min() const;
    };

    class UnknownIdentifier : public Nullary
    {
        public:
            UnknownIdentifier(const std::string& identifier_name);
            Function get_lambda() const;
            void accept(AbstractNodeVisitor& visitor) const;
            std::string get_name() const;
            double get_max() const;
            double get_min() const;

        private:
            UnknownIdentifier();
            std::string identifier_name;
    };

    class AbstractNodeVisitor
    {
        public:
            AbstractNodeVisitor(){};
            virtual ~AbstractNodeVisitor(){}
            virtual void visit(const UnknownIdentifier& f) = 0;
            virtual void visit(const Time& f) = 0;
            virtual void visit(const State<StateType::X>& f) = 0;
            virtual void visit(const State<StateType::Y>& f) = 0;
            virtual void visit(const State<StateType::Z>& f) = 0;
            virtual void visit(const State<StateType::U>& f) = 0;
            virtual void visit(const State<StateType::V>& f) = 0;
            virtual void visit(const State<StateType::W>& f) = 0;
            virtual void visit(const State<StateType::P>& f) = 0;
            virtual void visit(const State<StateType::Q>& f) = 0;
            virtual void visit(const State<StateType::R>& f) = 0;
            virtual void visit(const State<StateType::PHI>& f) = 0;
            virtual void visit(const State<StateType::THETA>& f) = 0;
            virtual void visit(const State<StateType::PSI>& f) = 0;
            virtual void visit(const State<StateType::QR>& f) = 0;
            virtual void visit(const State<StateType::QI>& f) = 0;
            virtual void visit(const State<StateType::QJ>& f) = 0;
            virtual void visit(const State<StateType::QK>& f) = 0;
            virtual void visit(const Binary& f) = 0;
            virtual void visit(const Unary& f) = 0;
            virtual void visit(const Constant& f) = 0;
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
    NodePtr make_state_x(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_y(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_z(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_u(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_v(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_w(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_p(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_q(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_r(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_phi(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_theta(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_psi(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_qr(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_qi(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_qj(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_state_qk(const NodePtr& operand, const YamlRotation& rot);
    NodePtr make_time();
    NodePtr make_unknown_identifier(const std::string& identifier_name);

    class FindTmax : public AbstractNodeVisitor
    {
        public:
            FindTmax();
            double get_Tmax() const;
            void visit(const UnknownIdentifier& f);
            void visit(const Time& f);
            void visit(const State<StateType::X>& f);
            void visit(const State<StateType::Y>& f);
            void visit(const State<StateType::Z>& f);
            void visit(const State<StateType::U>& f);
            void visit(const State<StateType::V>& f);
            void visit(const State<StateType::W>& f);
            void visit(const State<StateType::P>& f);
            void visit(const State<StateType::Q>& f);
            void visit(const State<StateType::R>& f);
            void visit(const State<StateType::PHI>& f);
            void visit(const State<StateType::THETA>& f);
            void visit(const State<StateType::PSI>& f);
            void visit(const State<StateType::QR>& f);
            void visit(const State<StateType::QI>& f);
            void visit(const State<StateType::QJ>& f);
            void visit(const State<StateType::QK>& f);
            void visit(const Binary& f);
            void visit(const Unary& f);
            void visit(const Constant& f);

        private:
            double Tmax;
    };
}



#endif  /* MANEUVERINGINTERNAL_HPP_ */

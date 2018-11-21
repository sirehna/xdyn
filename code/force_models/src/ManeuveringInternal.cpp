/*
 * ManeuveringInternal.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: cady
 */


#include "ManeuveringInternal.hpp"

using namespace maneuvering;

Node::Node(const std::vector<NodePtr>& children_) : children(children_)
{
}

Node::~Node()
{
}

std::vector<NodePtr> Node::get_children() const
{
    return children;
}

Nullary::Nullary() : Node(std::vector<NodePtr>())
{
}

Binary::Binary(const NodePtr& lhs, const NodePtr& rhs) : Node({lhs,rhs})
{
}

Constant::Constant(const double val_) : val(val_)
{
}

Function Constant::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& , const double )
            {
                return val;
            };
}

void Constant::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Constant::get_max() const
{
    return val;
}

double Constant::get_min() const
{
    return val;
}

Unary::Unary(const NodePtr operand) : Node({operand})
{
}

NodePtr Unary::get_operand() const
{
    return children.front();
}

Cos::Cos(const NodePtr& operand) : Unary(operand)
{
}
Function Cos::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return cos(op(states, ds, t));
            };
}

void Cos::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Cos::get_max() const
{
    return 1;
}

double Cos::get_min() const
{
    return -1;
}

Sin::Sin(const NodePtr& operand) : Unary(operand)
{
}
Function Sin::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return sin(op(states, ds, t));
            };
}

void Sin::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Sin::get_max() const
{
    return 1;
}

double Sin::get_min() const
{
    return -1;
}

Abs::Abs(const NodePtr& operand) : Unary(operand)
{
}

Function Abs::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::abs(op(states, ds, t));
            };
}

void Abs::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Abs::get_max() const
{
    return std::max(std::abs(get_operand()->get_max()), std::abs(get_operand()->get_min()));
}

double Abs::get_min() const
{
    return 0;
}


Log::Log(const NodePtr& operand) : Unary(operand)
{
}

double Log::get_max() const
{
    return std::log(get_operand()->get_max());
}

double Log::get_min() const
{
    return std::log(get_operand()->get_min());
}

Function Log::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::log(op(states, ds, t));
            };
}

void Log::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

NodePtr Binary::get_lhs() const
{
    return children.at(0);
}

NodePtr Binary::get_rhs() const
{
    return children.at(1);
}

Sum::Sum(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Sum::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) + op2(states, ds, t);
    };
}

void Sum::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Sum::get_max() const
{
    return get_lhs()->get_max() + get_rhs()->get_max();
}

double Sum::get_min() const
{
    return get_lhs()->get_min() + get_rhs()->get_min();
}

Pow::Pow(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Pow::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return std::pow(op1(states, ds, t), op2(states, ds, t));
    };
}

void Pow::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Pow::get_min() const
{
    const double m1 = std::pow(get_lhs()->get_min(), get_rhs()->get_min());
    const double m2 = std::pow(get_lhs()->get_min(), get_rhs()->get_max());
    const double m3 = std::pow(get_lhs()->get_max(), get_rhs()->get_min());
    const double m4 = std::pow(get_lhs()->get_max(), get_rhs()->get_max());
    return std::min(std::min(m1,m2),std::min(m3,m4));
}

double Pow::get_max() const
{
    const double m1 = std::pow(get_lhs()->get_min(), get_rhs()->get_min());
    const double m2 = std::pow(get_lhs()->get_min(), get_rhs()->get_max());
    const double m3 = std::pow(get_lhs()->get_max(), get_rhs()->get_min());
    const double m4 = std::pow(get_lhs()->get_max(), get_rhs()->get_max());
    return std::max(std::max(m1,m2),std::max(m3,m4));
}

Exp::Exp(const NodePtr& operand) : Unary(operand)
{
}

Function Exp::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::exp(op(states, ds, t));
            };
}

double Exp::get_min() const
{
    return std::exp(get_operand()->get_min());
}

double Exp::get_max() const
{
    return std::exp(get_operand()->get_max());
}

void Exp::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Function Sqrt::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::sqrt(op(states, ds, t));
            };
}

Sqrt::Sqrt(const NodePtr& operand) : Unary(operand)
{
}

void Sqrt::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Sqrt::get_min() const
{
    return std::sqrt(get_operand()->get_min());
}

double Sqrt::get_max() const
{
    return std::sqrt(get_operand()->get_max());
}

Difference::Difference(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Difference::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) - op2(states, ds, t);
    };
}

void Difference::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Difference::get_min() const
{
    return get_lhs()->get_min() - get_rhs()->get_max();
}

double Difference::get_max() const
{
    return get_lhs()->get_max() - get_rhs()->get_min();
}

Divide::Divide(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Divide::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) / op2(states, ds, t);
    };
}

void Divide::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Divide::get_min() const
{
    return get_lhs()->get_min()/get_rhs()->get_max();
}

double Divide::get_max() const
{
    return get_lhs()->get_max() / get_rhs()->get_min();
}

Multiply::Multiply(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Multiply::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) * op2(states, ds, t);
    };
}

void Multiply::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Multiply::get_min() const
{
    return get_lhs()->get_min()*get_rhs()->get_min();
}

double Multiply::get_max() const
{
    return get_lhs()->get_max() * get_rhs()->get_max();
}

Time::Time()
{
}

Function Time::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& , const double t)
            {
                return t;
            };
}

void Time::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

double Time::get_min() const
{
    return 0;
}

double Time::get_max() const
{
    return 0;
}

UnknownIdentifier::UnknownIdentifier(const std::string& identifier_name_) : identifier_name(identifier_name_)
{
}

Function UnknownIdentifier::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& ds, const double )
            {
                return ds.get<double>(identifier_name);
            };
}

void UnknownIdentifier::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

std::string UnknownIdentifier::get_name() const
{
    return identifier_name;
}


double UnknownIdentifier::get_min() const
{
    return 0;
}

double UnknownIdentifier::get_max() const
{
    return 0;
}
NodePtr maneuvering::make_constant(const double val)
{
    return NodePtr(new Constant(val));
}

NodePtr maneuvering::make_cos(const NodePtr& n)
{
    return NodePtr(new Cos(n));
}

NodePtr maneuvering::make_sin(const NodePtr& n)
{
    return NodePtr(new Sin(n));
}

NodePtr maneuvering::make_abs(const NodePtr& n)
{
    return NodePtr(new Abs(n));
}

NodePtr maneuvering::make_log(const NodePtr& n)
{
    return NodePtr(new Log(n));
}

NodePtr maneuvering::make_sum(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Sum(lhs,rhs));
}

NodePtr maneuvering::make_pow(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Pow(lhs,rhs));
}

NodePtr maneuvering::make_exp(const NodePtr& n)
{
    return NodePtr(new Exp(n));
}

NodePtr maneuvering::make_sqrt(const NodePtr& n)
{
    return NodePtr(new Sqrt(n));
}

NodePtr maneuvering::make_difference(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Difference(lhs,rhs));
}

NodePtr maneuvering::make_divide(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Divide(lhs,rhs));
}

NodePtr maneuvering::make_multiply(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Multiply(lhs,rhs));
}

NodePtr maneuvering::make_state_x(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::X>(n,rot));
}

NodePtr maneuvering::make_state_y(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::Y>(n,rot));
}

NodePtr maneuvering::make_state_z(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::Z>(n,rot));
}

NodePtr maneuvering::make_state_u(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::U>(n,rot));
}

NodePtr maneuvering::make_state_v(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::V>(n,rot));
}

NodePtr maneuvering::make_state_w(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::W>(n,rot));
}

NodePtr maneuvering::make_state_p(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::P>(n,rot));
}

NodePtr maneuvering::make_state_q(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::Q>(n,rot));
}

NodePtr maneuvering::make_state_r(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::R>(n,rot));
}

NodePtr maneuvering::make_state_phi(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::PHI>(n,rot));
}

NodePtr maneuvering::make_state_theta(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::THETA>(n,rot));
}

NodePtr maneuvering::make_state_psi(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::PSI>(n,rot));
}

NodePtr maneuvering::make_state_qr(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::QR>(n,rot));
}

NodePtr maneuvering::make_state_qi(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::QI>(n,rot));
}

NodePtr maneuvering::make_state_qj(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::QJ>(n,rot));
}

NodePtr maneuvering::make_state_qk(const NodePtr& n, const YamlRotation& rot)
{
    return NodePtr(new State<StateType::QK>(n,rot));
}

NodePtr maneuvering::make_time()
{
    return NodePtr(new Time());
}

NodePtr maneuvering::make_unknown_identifier(const std::string& identifier_name)
{
    return NodePtr(new UnknownIdentifier(identifier_name));
}

namespace maneuvering
{
    template <> void State<StateType::X>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Y>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Z>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::U>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::V>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::W>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::P>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Q>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::R>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::PHI>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::THETA>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::PSI>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::QR>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::QI>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::QJ>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::QK>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}
namespace maneuvering
{
    FindTmax::FindTmax() : Tmax(0) {}

    double FindTmax::get_Tmax() const
    {
        return Tmax;
    }

    void FindTmax::visit(const UnknownIdentifier& )
    {
    }
    void FindTmax::visit(const Time& f) {}
    void FindTmax::visit(const State<StateType::X>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_min());
    }
    void FindTmax::visit(const State<StateType::Y>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::Z>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::U>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::V>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::W>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::P>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::Q>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::R>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::PHI>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::THETA>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::PSI>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::QR>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::QI>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::QJ>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const State<StateType::QK>& f)
    {
        const auto op = f.get_operand();
        Tmax = std::max(Tmax, -op->get_max());
    }
    void FindTmax::visit(const Binary& f)
    {
        auto children  = f.get_children();
        for (auto child:children)
        {
            child->accept(*this);
        }
    }
    void FindTmax::visit(const Unary& f)
    {
        auto children  = f.get_children();
        for (auto child:children)
        {
            child->accept(*this);
        }
    }
    void FindTmax::visit(const Constant& ) {}
}

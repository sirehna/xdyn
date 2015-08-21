/*
 * BlockedDOF.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */

#include <map>

#include "BlockedDOF.hpp"
#include "BlockedDOFException.hpp"
#include "SimulatorYamlParserException.hpp"
#include <ssc/yaml_parser.hpp>
#include <ssc/csv_file_reader.hpp>
#include <ssc/text_file_reader.hpp>
#include "yaml.h"

void operator >> (const YAML::Node& node, BlockableState& g);
void operator >> (const YAML::Node& node, BlockableState& g)
{
    std::string t;
    node >> t;
    if      (t == "u") g = BlockableState::U;
    else if (t == "v") g = BlockableState::V;
    else if (t == "w") g = BlockableState::W;
    else if (t == "p") g = BlockableState::P;
    else if (t == "q") g = BlockableState::Q;
    else if (t == "r") g = BlockableState::R;
    else
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Unrecognized state: '" << t << "'. Has to be one of 'u', 'v', 'w', 'p', 'q' or 'r'.");
    }
}

void operator >> (const YAML::Node& node, InterpolationType& g);
void operator >> (const YAML::Node& node, InterpolationType& g)
{
    std::string t;
    node >> t;
    if      (t == "piecewise constant") g = InterpolationType::PIECEWISE_CONSTANT;
    else if (t == "linear")             g = InterpolationType::LINEAR;
    else if (t == "spline")             g = InterpolationType::SPLINE;
    else
    {
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, "Unrecognized interpolation type: '" << t << "'. Has to be one of 'piecewise constant', 'linear', 'spline'");
    }
}

void operator >> (const YAML::Node& node, YamlCSVDOF& g);
void operator >> (const YAML::Node& node, YamlCSVDOF& g)
{
    node["filename"]      >> g.filename;
    node["interpolation"] >> g.interpolation;
    node["state"]         >> g.state;
    node["t"]             >> g.t;
    node["value"]         >> g.value;
}

void operator >> (const YAML::Node& node, YamlDOF<std::vector<double> >& g);
void operator >> (const YAML::Node& node, YamlDOF<std::vector<double> >& g)
{
    node["interpolation"] >> g.interpolation;
    node["state"]         >> g.state;
    node["t"]             >> g.t;
    node["value"]         >> g.value;
}

std::ostream& operator<<(std::ostream& os, const BlockableState& s);
std::ostream& operator<<(std::ostream& os, const BlockableState& s)
{
    switch(s)
    {
        case BlockableState::U:
            os << "u";
            break;
        case BlockableState::V:
            os << "v";
            break;
        case BlockableState::W:
            os << "w";
            break;
        case BlockableState::P:
            os << "p";
            break;
        case BlockableState::Q:
            os << "q";
            break;
        case BlockableState::R:
            os << "r";
            break;
        default:
            break;
    }
    return os;
}

typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> Interpolator;

class Builder
{
    public:
        Builder(const YamlBlockedDOF& yaml) : input(yaml)
        {
            check_states_are_not_defined_twice(input);
        }
        std::map<BlockableState, Interpolator> get_forced_states() const
        {
            std::map<BlockableState, Interpolator> ret;
            for (const auto y:input.from_csv) ret[y.state] = build(y);
            for (const auto y:input.from_yaml) ret[y.state] = build(y);
            return ret;
        }

    private:
        Builder();
        void throw_if_already_defined(const BlockableState& state, std::map<BlockableState, bool>& defined) const
        {
            if (defined[state])
            {
                THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Attempting to define state " << state << " twice in 'blocked dof' section of the YAML file.");
            }
            defined[state] = true;
        }

        void check_states_are_not_defined_twice(const YamlBlockedDOF& input) const
        {
            std::map<BlockableState, bool> defined_in_yaml, defined_in_csv;
            for (const auto state : input.from_yaml)
            {
                throw_if_already_defined(state.state, defined_in_yaml);
            }
            for (const auto state : input.from_csv)
            {
                throw_if_already_defined(state.state, defined_in_yaml);
                throw_if_already_defined(state.state, defined_in_csv);
            }
        }
        Interpolator build(const YamlDOF<std::vector<double> >& y) const
        {
            Interpolator ret;
            try
            {
                ret = build(y.t, y.value, y.interpolation);
            }
            catch(const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from YAML': " << e.get_message());
            }
            catch(const std::exception& e)
            {
                THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from YAML': " << e.what());
            }
            return ret;
        }

        Interpolator build(const YamlCSVDOF& y) const
        {
            try
            {
                const ssc::text_file_reader::TextFileReader txt(y.filename);
                const ssc::csv_file_reader::CSVFileReader reader(txt.get_contents());
                auto m = reader.get_map();
                const auto it1 = m.find(y.t);
                if (it1 == m.end())
                {
                    THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Unable to find column " << y.t << " in CSV file " << y.filename);
                }
                const auto it2 = m.find(y.value);
                if (it2 == m.end())
                {
                    THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Unable to find column " << y.value << " in CSV file " << y.filename);
                }
                const auto t = it1->second;
                const auto state = it2->second;
                return build(t, state, y.interpolation);
            }
            catch(const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.get_message());
            }
            catch(const std::exception& e)
            {
                THROW(__PRETTY_FUNCTION__, BlockedDOFException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.what());
            }
            return Interpolator();
        }
        Interpolator build(const std::vector<double>& t, const std::vector<double>& state, const InterpolationType& interpolation_type) const
        {
            switch(interpolation_type)
            {
                case InterpolationType::LINEAR:
                    return Interpolator(new ssc::interpolation::LinearInterpolationVariableStep(t, state));
                    break;
                case InterpolationType::PIECEWISE_CONSTANT:
                    return Interpolator(new ssc::interpolation::PiecewiseConstantVariableStep<double>(t, state));
                    break;
                case InterpolationType::SPLINE:
                    return Interpolator(new ssc::interpolation::SplineVariableStep(t, state));
                    break;
                default:
                    break;
            }
            return Interpolator();
        }
        YamlBlockedDOF input;
};

struct BlockedDOF::Impl
{
    Impl(const Builder& builder, const size_t body_idx_) : blocked_dof(builder.get_forced_states()), body_idx(body_idx_) {}
    std::map<BlockableState, Interpolator> blocked_dof;
    size_t body_idx;

    size_t state_index(const BlockableState& s)
    {
        switch (s)
        {
            case BlockableState::U:
                return UIDX(body_idx);
                break;
            case BlockableState::V:
                return VIDX(body_idx);
                break;
            case BlockableState::W:
                return WIDX(body_idx);
                break;
            case BlockableState::P:
                return PIDX(body_idx);
                break;
            case BlockableState::Q:
                return QIDX(body_idx);
                break;
            case BlockableState::R:
                return RIDX(body_idx);
                break;
            default:
                break;
        }
        return 0;
    }


    private:Impl();
};

YamlBlockedDOF BlockedDOF::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlBlockedDOF ret;

    if (node.FindValue("blocked dof"))
    {
        if (node["blocked dof"].FindValue("from CSV"))  node["blocked dof"]["from CSV"]  >> ret.from_csv;
        if (node["blocked dof"].FindValue("from YAML")) node["blocked dof"]["from YAML"] >> ret.from_yaml;
    }
    return ret;
}

BlockedDOF::BlockedDOF(const YamlBlockedDOF& input, const size_t body_idx) : pimpl(new Impl(Builder(input),body_idx))
{
}

BlockedDOF::BlockedDOF(const std::string& input, const size_t body_idx) : pimpl(new Impl(Builder(parse(input)),body_idx))
{
}

void BlockedDOF::force_states(StateType& x, const double t) const
{
    for (auto dof:pimpl->blocked_dof)
    {
        x[pimpl->state_index(dof.first)] = dof.second->f(t);
    }
}

void BlockedDOF::force_state_derivatives(StateType& dx_dt, const double t) const
{
    for (auto dof:pimpl->blocked_dof)
    {
        dx_dt[pimpl->state_index(dof.first)] = dof.second->df(t);
    }
}

BlockedDOF::Vector BlockedDOF::get_delta_F(const StateType& dx_dt_, const Eigen::Matrix<double,6,6>& total_inertia, const ssc::kinematics::Wrench& sum_of_other_forces) const
{
    BlockedDOF::Vector delta_F;
    BlockedDOF::Vector dx_dt;
    dx_dt(0) = dx_dt_[UIDX(pimpl->body_idx)];
    dx_dt(1) = dx_dt_[VIDX(pimpl->body_idx)];
    dx_dt(2) = dx_dt_[WIDX(pimpl->body_idx)];
    dx_dt(3) = dx_dt_[PIDX(pimpl->body_idx)];
    dx_dt(4) = dx_dt_[QIDX(pimpl->body_idx)];
    dx_dt(5) = dx_dt_[RIDX(pimpl->body_idx)];
    BlockedDOF::Vector F;
    F(0) = sum_of_other_forces.X();
    F(1) = sum_of_other_forces.Y();
    F(2) = sum_of_other_forces.Z();
    F(3) = sum_of_other_forces.K();
    F(4) = sum_of_other_forces.M();
    F(5) = sum_of_other_forces.N();
    delta_F = total_inertia*dx_dt - F;
    return delta_F;
}

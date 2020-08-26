/*
 * BlockedDOF.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */

#include <map>
#include <vector>

#include "BlockedDOF.hpp"
#include "InvalidInputException.hpp"
#include "NumericalErrorException.hpp"
#include "external_data_structures_parsers.hpp"

#include <ssc/csv_file_reader.hpp>
#include <ssc/text_file_reader.hpp>

typedef YamlDOF<std::vector<double> > Table;

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
            for (const auto y:input.from_yaml) ret[y.state] = build_interpolator(y);
            return ret;
        }

    private:
        Builder();

        void throw_if_already_defined(const BlockableState& state, std::map<BlockableState, bool>& defined) const
        {
            if (defined[state])
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Attempting to define state " << state << " twice in 'blocked dof' section of the YAML file.");
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
        Interpolator build_interpolator(const Table& y) const
        {
            try
            {
                switch(y.interpolation)
                {
                    case InterpolationType::LINEAR:
                        return Interpolator(new ssc::interpolation::LinearInterpolationVariableStep(y.t, y.value));
                        break;
                    case InterpolationType::PIECEWISE_CONSTANT:
                        return Interpolator(new ssc::interpolation::PiecewiseConstantVariableStep<double>(y.t, y.value));
                        break;
                    case InterpolationType::SPLINE:
                        return Interpolator(new ssc::interpolation::SplineVariableStep(y.t, y.value));
                        break;
                    default:
                        break;
                }
                return Interpolator();
            }
            catch(const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.get_message());
            }
            catch(const std::exception& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.what());
            }
            return Interpolator();
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
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find column " << y.t << " in CSV file " << y.filename);
                }
                const auto it2 = m.find(y.value);
                if (it2 == m.end())
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find column " << y.value << " in CSV file " << y.filename);
                }
                const auto t = it1->second;
                const auto state = it2->second;
                Table table;
                table.interpolation = y.interpolation;
                table.state = y.state;
                table.t = t;
                table.value = state;
                return build_interpolator(table);
            }
            catch(const ssc::exception_handling::Exception& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.get_message());
            }
            catch(const std::exception& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Error when building forced state '" << y.state << "' defined in 'forced DOF/from CSV': " << e.what());
            }
            return Interpolator();
        }

        YamlBlockedDOF input;
};

struct BlockedDOF::Impl
{
    Impl(Builder builder, const size_t body_idx_)
            : blocked_dof(builder.get_forced_states())
            , body_idx(body_idx_)
    {}

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
        double forced_value = 0;
        try
        {
            forced_value = dof.second->f(t);
        }
        catch(const ssc::interpolation::IndexFinderException& e)
        {
            std::stringstream ss;
            ss << "Unable to interpolate value of forced state '" << dof.first << "' at t=" << t << "s: " << e.get_message();
            THROW(__PRETTY_FUNCTION__, NumericalErrorException, ss.str());
        }
        x[pimpl->state_index(dof.first)] = forced_value;
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
    if (pimpl->blocked_dof.empty()) return Vector::Zero();
    BlockedDOF::Vector delta_F;
    BlockedDOF::Vector dx_dt;
    dx_dt(0) = dx_dt_.at(UIDX(pimpl->body_idx));
    dx_dt(1) = dx_dt_.at(VIDX(pimpl->body_idx));
    dx_dt(2) = dx_dt_.at(WIDX(pimpl->body_idx));
    dx_dt(3) = dx_dt_.at(PIDX(pimpl->body_idx));
    dx_dt(4) = dx_dt_.at(QIDX(pimpl->body_idx));
    dx_dt(5) = dx_dt_.at(RIDX(pimpl->body_idx));
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

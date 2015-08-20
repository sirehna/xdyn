/*
 * BlockedDOF.hpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */

#ifndef BLOCKEDDOF_HPP_
#define BLOCKEDDOF_HPP_

#include <map>
#include <string>
#include <vector>

#include <ssc/interpolation.hpp>
#include <ssc/macros/tr1_macros.hpp>

#include TR1INC(memory)

#include "StateMacros.hpp"

class BlockedDOF
{
    public:
        enum class InterpolationType {PIECEWISE_CONSTANT, LINEAR, SPLINE};
        enum class BlockableState {U, V, W, P, Q, R};

        template <typename T> struct YamlDOF
        {
            YamlDOF() :
                state(BlockableState::U),
                t(),
                value(),
                interpolation(InterpolationType::PIECEWISE_CONSTANT)
            {}
            virtual ~YamlDOF() {}
            BlockableState state;
            T t;
            T value;
            InterpolationType interpolation;
        };

        struct YamlCSVDOF : public YamlDOF<std::string>
        {
            YamlCSVDOF();
            std::string filename;
        };

        struct Yaml
        {
            Yaml();
            std::vector<YamlDOF<std::vector<double> > > from_yaml;
            std::vector<YamlCSVDOF> from_csv;
        };

        typedef TR1(shared_ptr)<ssc::interpolation::Interpolator> Interpolator;

        class Builder
        {
            public:
                Builder(const Yaml& yaml);
                std::map<BlockableState, Interpolator> get_forced_states() const;

            private:
                Builder();
                Interpolator build(const YamlDOF<std::vector<double> >& y) const;
                Interpolator build(const YamlCSVDOF& y) const;
                Interpolator build(const std::vector<double>& t, const std::vector<double>& state, const InterpolationType& interpolation_type) const;
                Yaml input;
        };

        static Yaml parse(const std::string& yaml);

        BlockedDOF(const Yaml& input, const size_t body_idx=0);
        BlockedDOF(const std::string& input, const size_t body_idx=0);
        void force_states(StateType& x, const double t) const;
        void force_state_derivatives(StateType& dx_dt, const double t) const;
    private:
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* BLOCKEDDOF_HPP_ */

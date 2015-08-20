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

        static Yaml parse(const std::string& yaml);

        BlockedDOF(const Yaml& input);

    private:
        BlockedDOF();
        void check_states_are_not_defined_twice(const Yaml& input) const;
};

#endif /* BLOCKEDDOF_HPP_ */

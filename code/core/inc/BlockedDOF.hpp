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

#include <Eigen/Dense>

#include <ssc/interpolation.hpp>
#include <ssc/kinematics.hpp>
#include <ssc/macros/tr1_macros.hpp>

#include TR1INC(memory)

#include "YamlBody.hpp"
#include "StateMacros.hpp"

class BlockedDOF
{
    public:
        BlockedDOF(const YamlBlockedDOF& input, const size_t body_idx=0);
        BlockedDOF(const std::string& input, const size_t body_idx=0);
        void force_states(StateType& x, const double t) const;
        void force_state_derivatives(StateType& dx_dt, const double t) const;
        typedef Eigen::Matrix<double,6,1> Vector;
        Vector get_delta_F(const StateType& dx_dt, const Eigen::Matrix<double,6,6>& total_inertia, const ssc::kinematics::Wrench& sum_of_other_forces) const;

    private:
        struct Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* BLOCKEDDOF_HPP_ */

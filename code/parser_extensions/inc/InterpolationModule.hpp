/*
 * InterpolationModule.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef INTERPOLATIONMODULE_HPP_
#define INTERPOLATIONMODULE_HPP_

#include <ssc/data_source.hpp>
#include <ssc/interpolation.hpp>

class InterpolationModule : public ssc::data_source::DataSourceModule
{
    public:
        InterpolationModule(ssc::data_source::DataSource* const data_source,
                            const std::string& module_name,
                            const std::string& xname_,
                            const std::string& yname_,
                            const TR1(shared_ptr)<ssc::interpolation::LinearInterpolationVariableStep>& I_);

        InterpolationModule(const InterpolationModule& rhs, ssc::data_source::DataSource* const data_source);

        ssc::data_source::DataSourceModule* clone() const;
        ssc::data_source::DataSourceModule* clone(ssc::data_source::DataSource* const data_source) const;
        void update() const;

    private:
        std::string xname;
        std::string yname;
        TR1(shared_ptr)<ssc::interpolation::LinearInterpolationVariableStep> I;
};

#endif /* INTERPOLATIONMODULE_HPP_ */

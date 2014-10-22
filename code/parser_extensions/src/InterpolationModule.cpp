/*
 * InterpolationModule.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "InterpolationModule.hpp"

InterpolationModule::InterpolationModule(ssc::data_source::DataSource* const data_source,
                            const std::string& module_name,
                            const std::string& xname_,
                            const std::string& yname_,
                            const TR1(shared_ptr)<ssc::interpolation::LinearInterpolationVariableStep>& I_) : ssc::data_source::DataSourceModule(data_source, module_name), xname(xname_), yname(yname_), I(I_)
{
}

InterpolationModule::InterpolationModule(const InterpolationModule& rhs, ssc::data_source::DataSource* const data_source) : ssc::data_source::DataSourceModule(rhs, data_source), xname(rhs.xname), yname(rhs.yname), I(rhs.I)
{
}

ssc::data_source::DataSourceModule* InterpolationModule::clone() const
{
    return new InterpolationModule(*this);
}

ssc::data_source::DataSourceModule* InterpolationModule::clone(ssc::data_source::DataSource* const data_source) const
{
    return new InterpolationModule(*this, data_source);
}

void InterpolationModule::update() const
{
    const double x = ds->get<double>(xname);
    const double y = I->f(x);
    ds->set<double>(yname, y);
}

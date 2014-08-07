/*
 * ListOfWaveModels.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "ListOfWaveModels.hpp"
#include "WaveModel.hpp"

ListOfWaveModels::ListOfWaveModels(const std::vector<TR1(shared_ptr)<WaveModel> >& models_) : models(models_)
{
}

double ListOfWaveModels::elevation(const double x,//!< x-position in the NED frame (in meters)
                                   const double y,//!< y-position in the NED frame (in meters)
                                   const double t //!< Current time instant (in seconds)
                                  ) const
{
    double zeta = 0;
    BOOST_FOREACH(TR1(shared_ptr)<WaveModel> model, models) zeta += model->elevation(x, y, t);
    return zeta;
}

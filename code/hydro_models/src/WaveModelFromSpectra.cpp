/*
 * WaveModelFromSpectra.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "WaveModel.hpp"
#include "WaveModelFromSpectra.hpp"

WaveModelFromSpectra::WaveModelFromSpectra(const TR1(shared_ptr)<WaveModel>& model_, const TR1(shared_ptr)<PointMatrix>& output_mesh_) : WaveModelInterface(output_mesh_), model(model_)
{
}

double WaveModelFromSpectra::wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                         const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                         const double z, //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                         const double t  //!< Current instant (in seconds)
                                         ) const
{
    return z - model->elevation(x,y,t);
}

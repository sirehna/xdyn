/*
 * DefaultWaveBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEBUILDER_HPP_
#define DEFAULTWAVEBUILDER_HPP_

#include "DefaultSurfaceElevation.hpp"
#include "SurfaceElevationBuilder.hpp"

template <>
class SurfaceElevationBuilder<DefaultSurfaceElevation> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};



#endif /* DEFAULTWAVEBUILDER_HPP_ */

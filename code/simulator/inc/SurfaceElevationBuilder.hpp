/*
 * SurfaceElevationBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SURFACEELEVATIONBUILDER_HPP_
#define SURFACEELEVATIONBUILDER_HPP_

#include <vector>

#include <boost/optional/optional.hpp>
#include "tr1_macros.hpp"
#include TR1INC(memory)

#include "DirectionalSpreadingBuilder.hpp"
#include "SpectrumBuilder.hpp"

class SurfaceElevationInterface;
class PointMatrix;
struct YamlWaveOutput;

class SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilderInterface(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                             const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_) :
                             directional_spreading_parsers(directional_spreading_parsers_),
                             spectrum_parsers(spectrum_parsers_)
        {}
        virtual ~SurfaceElevationBuilderInterface();
        TR1(shared_ptr)<PointMatrix> make_wave_mesh(const YamlWaveOutput& output) const;
        virtual boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const = 0;

    private:
        SurfaceElevationBuilderInterface();
        TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> > directional_spreading_parsers;
        TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> > spectrum_parsers;
};

template <typename T>
class SurfaceElevationBuilder : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_) : SurfaceElevationBuilderInterface(directional_spreading_parsers_,spectrum_parsers_)
        {}
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;

    private:
        SurfaceElevationBuilder(); // Disabled
};


typedef TR1(shared_ptr)<SurfaceElevationBuilderInterface> WaveBuilderPtr;
typedef TR1(shared_ptr)<SurfaceElevationInterface> WavePtr;


#endif /* SURFACEELEVATIONBUILDER_HPP_ */

/*
 * builders.hpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#ifndef BUILDERS_HPP_
#define BUILDERS_HPP_

#include "DefaultSurfaceElevation.hpp"
#include "SurfaceElevationBuilder.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "Airy.hpp"
#include "BretschneiderSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "JonswapSpectrum.hpp"
#include "PiersonMoskowitzSpectrum.hpp"

typedef TR1(shared_ptr)<SurfaceElevationInterface> SurfaceElevationInterfacePtr;
typedef TR1(shared_ptr)<WaveSpectralDensity> WaveSpectralDensityPtr;
typedef TR1(shared_ptr)<WaveDirectionalSpreading> WaveDirectionalSpreadingPtr;

template <>
class SurfaceElevationBuilder<DefaultSurfaceElevation> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                                const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<SurfaceElevationInterfacePtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

struct YamlDiscretization;
struct YamlSpectra;

template <>
class SurfaceElevationBuilder<SurfaceElevationFromWaves> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                                const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<SurfaceElevationInterfacePtr> try_to_parse(const std::string& model, const std::string& yaml) const;

    private:
        SurfaceElevationBuilder();
        WaveModelPtr parse_wave_model(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const;
        DiscreteDirectionalWaveSpectrum parse_directional_spectrum(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const;
        WaveSpectralDensityPtr parse_spectral_density(const YamlSpectra& spectrum) const;
        WaveDirectionalSpreadingPtr parse_directional_spreading(const YamlSpectra& spectrum) const;
};

class SurfaceElevationFromGRPC;
template <>
class SurfaceElevationBuilder<SurfaceElevationFromGRPC> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder();
        boost::optional<SurfaceElevationInterfacePtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class WaveModelBuilder<Airy> : public WaveModelBuilderInterface
{
    public:
        WaveModelBuilder();
        boost::optional<WaveModelPtr> try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<BretschneiderSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<WaveSpectralDensityPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<JonswapSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<WaveSpectralDensityPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<PiersonMoskowitzSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<WaveSpectralDensityPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<DiracSpectralDensity> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<WaveSpectralDensityPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class DirectionalSpreadingBuilder<DiracDirectionalSpreading> : public DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilder() : DirectionalSpreadingBuilderInterface(){}
        boost::optional<WaveDirectionalSpreadingPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class DirectionalSpreadingBuilder<Cos2sDirectionalSpreading> : public DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilder() : DirectionalSpreadingBuilderInterface(){}
        boost::optional<WaveDirectionalSpreadingPtr> try_to_parse(const std::string& model, const std::string& yaml) const;
};

#endif /* BUILDERS_HPP_ */

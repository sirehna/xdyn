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
#include "ForceBuilder.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "GravityForceModel.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "Airy.hpp"
#include "BretschneiderSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "JonswapSpectrum.hpp"
#include "PiersonMoskowitzSpectrum.hpp"
#include "FroudeKrylovForceModel.hpp"

template <>
class SurfaceElevationBuilder<DefaultSurfaceElevation> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                                const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};


template <>
class ForceBuilder<GravityForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

template <>
class ForceBuilder<FroudeKrylovForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

template <>
class ForceBuilder<FastHydrostaticForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

template <>
class ForceBuilder<ExactHydrostaticForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

struct YamlDiscretization;
struct YamlSpectra;

template <>
class SurfaceElevationBuilder<SurfaceElevationFromWaves> : public SurfaceElevationBuilderInterface
{
    public:
        SurfaceElevationBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;

    private:
        SurfaceElevationBuilder();
        TR1(shared_ptr)<WaveModel> parse_wave_model(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const;
        DiscreteDirectionalWaveSpectrum parse_directional_spectrum(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const;
        TR1(shared_ptr)<WaveSpectralDensity> parse_spectral_density(const YamlSpectra& spectrum) const;
        TR1(shared_ptr)<WaveDirectionalSpreading> parse_directional_spreading(const YamlSpectra& spectrum) const;
};

template <>
class WaveModelBuilder<Airy> : public WaveModelBuilderInterface
{
    public:
        WaveModelBuilder();
        boost::optional<TR1(shared_ptr)<WaveModel> > try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<BretschneiderSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<JonswapSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<PiersonMoskowitzSpectrum> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class SpectrumBuilder<DiracSpectralDensity> : public SpectrumBuilderInterface
{
    public:
        SpectrumBuilder();
        boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class DirectionalSpreadingBuilder<DiracDirectionalSpreading> : public DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilder() : DirectionalSpreadingBuilderInterface(){}
        boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

template <>
class DirectionalSpreadingBuilder<Cos2sDirectionalSpreading> : public DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilder() : DirectionalSpreadingBuilderInterface(){}
        boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > try_to_parse(const std::string& model, const std::string& yaml) const;
};

#endif /* BUILDERS_HPP_ */

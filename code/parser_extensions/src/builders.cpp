/*
 * builders.cpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#include "builders.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "environment_parsers.hpp"
#include "EnvironmentAndFrames.hpp"
#include "discretize.hpp"
#include "Stretching.hpp"
#include "SurfaceElevationFromGRPC.hpp"
#include "InvalidInputException.hpp"
#include "YamlGRPC.hpp"

boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<DefaultSurfaceElevation>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "no waves")
    {
        if (yaml.empty())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "No yaml data detected for default wave model (expected 'constant wave height in NED frame: {value: xx, unit: yy})'");
        }
        YamlDefaultWaveModel input = parse_default_wave_model(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        SurfaceElevationInterfacePtr p(new DefaultSurfaceElevation(input.zwave, output_mesh));
        ret.reset(p);
    }
    return ret;
}

boost::optional<WaveModelPtr> WaveModelBuilder<Airy>::try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const
{
    boost::optional<WaveModelPtr> ret;
    if (model == "airy")
    {
        const boost::optional<int> seed = parse_seed_of_random_number_generator(yaml);
        if (seed)
        {
            ret.reset(WaveModelPtr(new Airy(spectrum,*seed)));
        }
        else
        {
            ret.reset(WaveModelPtr(new Airy(spectrum,0.0)));
        }
    }
    return ret;
}

WaveSpectralDensityPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_spectral_density(const YamlSpectra& spectrum) const
{
    for (auto that_parser = spectrum_parsers->begin() ; that_parser != spectrum_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveSpectralDensityPtr> w;
        try
        {
            w = (*that_parser)->try_to_parse(spectrum.spectral_density_type, spectrum.spectral_density_yaml);
        }
        catch (const InvalidInputException& exception)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Problem when parsing spectral density '" << spectrum.model << "': " << exception.get_message());
        }
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "The wave spectral density specified in the YAML file is not understood by the simulator ('" << spectrum.spectral_density_type << "'): either it is misspelt or this simulator version is outdated.");
    return WaveSpectralDensityPtr();
}

WaveDirectionalSpreadingPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spreading(const YamlSpectra& spectrum) const
{
    for (auto that_parser = directional_spreading_parsers->begin() ; that_parser != directional_spreading_parsers->end() ; ++that_parser)
    {
        boost::optional<WaveDirectionalSpreadingPtr> w;
        try
        {
            w = (*that_parser)->try_to_parse(spectrum.directional_spreading_type, spectrum.directional_spreading_yaml);
        }
        catch (const InvalidInputException& exception)
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Problem when parsing directional spreading '" << spectrum.model << "': " << exception.get_message());
        }
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "The wave directional spreading specified in the YAML file is not understood by the simulator ('" << spectrum.directional_spreading_type << "'): either it is misspelt or this simulator version is outdated.");
    return WaveDirectionalSpreadingPtr ();
}

DiscreteDirectionalWaveSpectrum SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spectrum(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const
{
    WaveSpectralDensityPtr spectral_density = parse_spectral_density(spectrum);
    WaveDirectionalSpreadingPtr directional_spreading = parse_directional_spreading(spectrum);
    if (spectrum.depth>0)
    {
        return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.n, Stretching(spectrum.stretching));
    }
    return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.n, spectrum.depth, Stretching(spectrum.stretching));
}

WaveModelPtr SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_wave_model(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const
{
    for (auto that_parser = wave_parsers->begin() ; that_parser != wave_parsers->end() ; ++that_parser)
    {
        const DiscreteDirectionalWaveSpectrum discrete_spectrum = parse_directional_spectrum(discretization, spectrum);
        boost::optional<WaveModelPtr> w = (*that_parser)->try_to_parse(spectrum.model, discrete_spectrum, spectrum.model_yaml);
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__,
          InvalidInputException,
          "The wave model specified in the YAML file is not understood by the simulator ('" << spectrum.model << "'): either it is misspelt or this simulator version is outdated.");
    return WaveModelPtr();
}

boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<SurfaceElevationFromWaves>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "waves") // The "model" key is always "waves" for xdyn's internal wave models, except for the default wave model "no waves"
    {
        const YamlWaveModel input = parse_waves(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        std::vector<WaveModelPtr> models;
        for (const auto& spectrum: input.spectra) models.push_back(parse_wave_model(input.discretization, spectrum));
        ret.reset(SurfaceElevationInterfacePtr(new SurfaceElevationFromWaves(models,get_wave_mesh_size(input.output),output_mesh)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<BretschneiderSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "bretschneider")
    {
        const YamlBretschneider data = parse_bretschneider(yaml);
        ret.reset(WaveSpectralDensityPtr(new BretschneiderSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<JonswapSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "jonswap")
    {
        const YamlJonswap data = parse_jonswap(yaml);
        ret.reset(WaveSpectralDensityPtr(new JonswapSpectrum(data.Hs, data.Tp, data.gamma)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<PiersonMoskowitzSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "pierson-moskowitz")
    {
        const YamlPiersonMoskowitz data = parse_pierson_moskowitz(yaml);
        ret.reset(WaveSpectralDensityPtr(new PiersonMoskowitzSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<WaveSpectralDensityPtr> SpectrumBuilder<DiracSpectralDensity>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveSpectralDensityPtr> ret;
    if (model == "dirac")
    {
        const YamlDiracSpectrum data = parse_wave_dirac_spectrum(yaml);
        ret.reset(WaveSpectralDensityPtr(new DiracSpectralDensity(data.omega0, data.Hs)));
    }
    return ret;
}

boost::optional<WaveDirectionalSpreadingPtr> DirectionalSpreadingBuilder<DiracDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveDirectionalSpreadingPtr> ret;
    if (model == "dirac")
    {
        const YamlDiracDirection data = parse_wave_dirac_direction(yaml);
        ret.reset(WaveDirectionalSpreadingPtr (new DiracDirectionalSpreading(data.psi0)));
    }
    return ret;
}

boost::optional<WaveDirectionalSpreadingPtr> DirectionalSpreadingBuilder<Cos2sDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<WaveDirectionalSpreadingPtr> ret;
    if (model == "cos2s")
    {
        const YamlCos2s data = parse_cos2s(yaml);
        ret.reset(WaveDirectionalSpreadingPtr (new Cos2sDirectionalSpreading(data.psi0, data.s)));
    }
    return ret;
}

boost::optional<SurfaceElevationInterfacePtr> SurfaceElevationBuilder<SurfaceElevationFromGRPC>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<SurfaceElevationInterfacePtr> ret;
    if (model == "grpc")
    {
        const YamlGRPC input = parse_grpc(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        ret.reset(SurfaceElevationInterfacePtr(new SurfaceElevationFromGRPC(input, output_mesh)));
    }
    return ret;
}

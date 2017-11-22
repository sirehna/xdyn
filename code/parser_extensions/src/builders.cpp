/*
 * builders.cpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#include "builders.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "environment_parsers.hpp"
#include "InternalErrorException.hpp"
#include "EnvironmentAndFrames.hpp"
#include "discretize.hpp"
#include "Stretching.hpp"
#include <boost/foreach.hpp>
#include "HOS.hpp"

boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > SurfaceElevationBuilder<DefaultSurfaceElevation>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > ret;
    if (model == "no waves")
    {
        if (yaml.empty())
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "No yaml data detected for default wave model (expected 'constant wave height in NED frame: {value: xx, unit: yy})'");
        }
        YamlDefaultWaveModel input = parse_default_wave_model(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        TR1(shared_ptr)<SurfaceElevationInterface> p(new DefaultSurfaceElevation(input.zwave, output_mesh));
        ret.reset(p);
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > SurfaceElevationBuilder<HOS>::try_to_parse(const std::string& model, const std::string& ) const
{
    boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > ret;
    if (model == "hos")
    {
        TR1(shared_ptr)<SurfaceElevationInterface> p(new HOS(0));
        ret.reset(p);
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveModel> > WaveModelBuilder<Airy>::try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveModel> > ret;
    if (model == "airy")
    {
        const boost::optional<int> seed = parse_airy(yaml);
        if (seed)
        {
            ret.reset(TR1(shared_ptr)<WaveModel>(new Airy(spectrum,*seed)));
        }
        else
        {
            ret.reset(TR1(shared_ptr)<WaveModel>(new Airy(spectrum,0.0)));
        }
    }
    return ret;
}

TR1(shared_ptr)<WaveSpectralDensity> SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_spectral_density(const YamlSpectra& spectrum) const
{
    for (auto that_parser = spectrum_parsers->begin() ; that_parser != spectrum_parsers->end() ; ++that_parser)
    {
        boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > w;
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
    return TR1(shared_ptr)<WaveSpectralDensity>();
}

TR1(shared_ptr)<WaveDirectionalSpreading> SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spreading(const YamlSpectra& spectrum) const
{
    for (auto that_parser = directional_spreading_parsers->begin() ; that_parser != directional_spreading_parsers->end() ; ++that_parser)
    {
        boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > w;
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
    return TR1(shared_ptr)<WaveDirectionalSpreading>();
}

DiscreteDirectionalWaveSpectrum SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_directional_spectrum(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const
{
    TR1(shared_ptr)<WaveSpectralDensity> spectral_density = parse_spectral_density(spectrum);
    TR1(shared_ptr)<WaveDirectionalSpreading> directional_spreading = parse_directional_spreading(spectrum);
    if (spectrum.depth>0)
    {
        return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.n, Stretching(spectrum.stretching));
    }
    return discretize(*spectral_density, *directional_spreading, discretization.omega_min, discretization.omega_max, discretization.n, spectrum.depth, Stretching(spectrum.stretching));
}


TR1(shared_ptr)<WaveModel> SurfaceElevationBuilder<SurfaceElevationFromWaves>::parse_wave_model(const YamlDiscretization& discretization, const YamlSpectra& spectrum) const
{
    for (auto that_parser = wave_parsers->begin() ; that_parser != wave_parsers->end() ; ++that_parser)
    {
        const DiscreteDirectionalWaveSpectrum discrete_spectrum = parse_directional_spectrum(discretization, spectrum);
        boost::optional<TR1(shared_ptr)<WaveModel> > w = (*that_parser)->try_to_parse(spectrum.model, discrete_spectrum, spectrum.model_yaml);
        if (w) return w.get();
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "The wave model specified in the YAML file is not understood by the simulator ('" << spectrum.model << "'): either it is misspelt or this simulator version is outdated.");
    return TR1(shared_ptr)<WaveModel>();
}

boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > SurfaceElevationBuilder<SurfaceElevationFromWaves>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > ret;
    if (model == "waves") // The "model" key is always "waves", except for the default wave model "no waves"
    {
        const YamlWaveModel input = parse_waves(yaml);
        const auto output_mesh = make_wave_mesh(input.output);
        std::vector<TR1(shared_ptr)<WaveModel> > models;
        BOOST_FOREACH(YamlSpectra spectrum, input.spectra) models.push_back(parse_wave_model(input.discretization, spectrum));
        ret.reset(TR1(shared_ptr)<SurfaceElevationInterface>(new SurfaceElevationFromWaves(models,get_wave_mesh_size(input.output),output_mesh)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > SpectrumBuilder<BretschneiderSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > ret;
    if (model == "bretschneider")
    {
        const YamlBretschneider data = parse_bretschneider(yaml);
        ret.reset(TR1(shared_ptr)<WaveSpectralDensity>(new BretschneiderSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > SpectrumBuilder<JonswapSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > ret;
    if (model == "jonswap")
    {
        const YamlJonswap data = parse_jonswap(yaml);
        ret.reset(TR1(shared_ptr)<WaveSpectralDensity>(new JonswapSpectrum(data.Hs, data.Tp, data.gamma)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > SpectrumBuilder<PiersonMoskowitzSpectrum>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > ret;
    if (model == "pierson-moskowitz")
    {
        const YamlPiersonMoskowitz data = parse_pierson_moskowitz(yaml);
        ret.reset(TR1(shared_ptr)<WaveSpectralDensity>(new PiersonMoskowitzSpectrum(data.Hs, data.Tp)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > SpectrumBuilder<DiracSpectralDensity>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveSpectralDensity> > ret;
    if (model == "dirac")
    {
        const YamlDiracSpectrum data = parse_wave_dirac_spectrum(yaml);
        ret.reset(TR1(shared_ptr)<WaveSpectralDensity>(new DiracSpectralDensity(data.omega0, data.Hs)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > DirectionalSpreadingBuilder<DiracDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > ret;
    if (model == "dirac")
    {
        const YamlDiracDirection data = parse_wave_dirac_direction(yaml);
        ret.reset(TR1(shared_ptr)<WaveDirectionalSpreading>(new DiracDirectionalSpreading(data.psi0)));
    }
    return ret;
}

boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > DirectionalSpreadingBuilder<Cos2sDirectionalSpreading>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveDirectionalSpreading> > ret;
    if (model == "cos2s")
    {
        const YamlCos2s data = parse_cos2s(yaml);
        ret.reset(TR1(shared_ptr)<WaveDirectionalSpreading>(new Cos2sDirectionalSpreading(data.psi0, data.s)));
    }
    return ret;
}


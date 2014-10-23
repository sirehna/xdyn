/*
 * SimulatorBuilder.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDER_HPP_
#define SIMULATORBUILDER_HPP_

#include <map>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "ControlledForceBuilder.hpp"
#include "ForceBuilder.hpp"
#include "SurfaceElevationBuilder.hpp"
#include "WaveModel.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"
#include "DirectionalSpreadingBuilder.hpp"
#include "SpectrumBuilder.hpp"
#include "WaveModelBuilder.hpp"

#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

#include "EnvironmentAndFrames.hpp"
#include "GeometricTypes3d.hpp"
class BodyBuilder;
typedef std::map<std::string, VectorOfVectorOfPoints> MeshMap;

/*  \brief Builds a Sim object which can be used with quicksolve
 *  \details This class is used to configure the Sim object for simulation.
 *           You have to tell the simulator builder how to parse the force &
 *           environmental models by using the can_parse methods.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest expected output
 */
class SimulatorBuilder
{
    public:
        SimulatorBuilder(const YamlSimulatorInput& input);

        /**  \brief Builds a Sim object using the supplied mesh map (one mesh per body)
          *  \details This function is mainly used in the integration tests.
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest build_example
          */
        Sim build(const MeshMap& input_meshes //!< Map containing a mesh for each body
                 ) const;

        /**  \brief Builds a Sim object reading the meshes from files
          *  \details Reads the STL data from an STL file & call the version of
          *           the build method that accepts a MeshMap as input.
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest build_example
          */
        Sim build() const;

        /**  \brief Add the capacity to parse the default wave model
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<SurfaceElevationInterface,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            surface_elevation_parsers.push_back(SurfaceElevationBuilderPtr(new SurfaceElevationBuilder<T>(wave_parsers, directional_spreading_parsers,spectrum_parsers)));
            return *this;
        }

        /**  \brief Add the capacity to parse the wave models defined in the wave_models module
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WaveModel,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            wave_parsers->push_back(WaveModelBuilderPtr(new WaveModelBuilder<T>(directional_spreading_parsers,spectrum_parsers)));
            return *this;
        }

        /**  \brief Add the capacity to parse certain YAML inputs for forces
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<ForceModel,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            force_parsers.push_back(ForceBuilderPtr(new ForceBuilder<T>()));
            return *this;
        }

        /**  \brief Add the capacity to parse certain YAML inputs for wave directional spreadings (eg. cos2s)
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WaveDirectionalSpreading,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            directional_spreading_parsers->push_back(DirectionalSpreadingBuilderPtr(new DirectionalSpreadingBuilder<T>()));
            return *this;
        }

        /**  \brief Add the capacity to parse certain YAML inputs for controlled forces (eg. Wageningen propellers)
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<ControllableForceModel,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            controlled_force_parsers.push_back(ControlledForceBuilderPtr(new ControlledForceBuilder<T>()));
            return *this;
        }

        /**  \brief Add the capacity to parse certain YAML inputs for wave spectra (eg. Jonswap)
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WaveSpectralDensity,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            spectrum_parsers->push_back(SpectrumBuilderPtr(new SpectrumBuilder<T>()));
            return *this;
        }

        std::vector<Body> get_bodies(const MeshMap& meshes) const;
        EnvironmentAndFrames get_environment_and_frames(const std::vector<Body>& bodies) const;
        std::vector<ListOfForces> get_forces(const EnvironmentAndFrames& env) const;
        StateType get_initial_states() const;
        YamlSimulatorInput get_parsed_yaml() const;
        MeshMap make_mesh_map() const;

    private:
        SimulatorBuilder(); // Disabled
        SurfaceElevationPtr get_wave() const;
        ListOfForces forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const;
        void add(const YamlModel& model, ListOfForces& L, const EnvironmentAndFrames& env) const;
        VectorOfVectorOfPoints get_mesh(const YamlBody& body) const;

        YamlSimulatorInput input;
        TR1(shared_ptr)<BodyBuilder> builder;
        std::vector<ForceBuilderPtr> force_parsers;
        std::vector<ControlledForceBuilderPtr> controlled_force_parsers;
        std::vector<SurfaceElevationBuilderPtr> surface_elevation_parsers;
        TR1(shared_ptr)<std::vector<WaveModelBuilderPtr> > wave_parsers;
        TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> > directional_spreading_parsers;
        TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> > spectrum_parsers;
};


#endif /* SIMULATORBUILDER_HPP_ */

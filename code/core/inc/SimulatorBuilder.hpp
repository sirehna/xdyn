/*
 * SimulatorBuilder.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDER_HPP_
#define SIMULATORBUILDER_HPP_

#include "WaveModel.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"

#include "SurfaceElevationBuilder.hpp"
#include "DirectionalSpreadingBuilder.hpp"
#include "SpectrumBuilder.hpp"
#include "WaveModelBuilder.hpp"

#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

#include "EnvironmentAndFrames.hpp"
#include "GeometricTypes3d.hpp"

#include <map>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <ssc/data_source.hpp>
#include <ssc/kinematics.hpp>

#include "WindModel.hpp"

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
        SimulatorBuilder(const YamlSimulatorInput& input, const double t0, const ssc::data_source::DataSource& command_listener = ssc::data_source::DataSource());

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
            force_parsers.push_back(ForceModel::build_parser<T>());
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
            controllable_force_parsers.push_back(ControllableForceModel::build_parser<T>());
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

        /**  \brief Add the capacity to parse certain YAML inputs for wind models (eg. cRPC wind model)
         *  \details This method must not be called with any parameters: the
         *  default parameter is only there so we can use boost::enable_if. This
         *  allows us to use can_parse for several types derived from a few
         *  base classes (WaveModelInterface, ForceModel...) & the compiler will
         *  automagically choose the right version of can_parse.
         *  \returns *this (so we can chain calls to can_parse)
         *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
         */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WindModel,T> >::type* dummy = 0)
        {
        	(void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
        	wind_model_parsers.push_back(WindModel::build_parser<T>());
        	return *this;
        }

        std::vector<BodyPtr> get_bodies(const MeshMap& meshes, const std::vector<bool>& bodies_contain_surface_forces, std::map<std::string,double> Tmax) const;
        EnvironmentAndFrames get_environment() const;
        std::vector<ListOfForces> get_forces(const EnvironmentAndFrames& env) const;
        std::vector<ListOfControlledForces> get_controlled_forces(const EnvironmentAndFrames& env) const;
        StateType get_initial_states() const;
        YamlSimulatorInput get_parsed_yaml() const;
        MeshMap make_mesh_map() const;

        /**  \brief Create a Kinematics object with transforms from NED to each body
          *  \returns KinematicsPtr containing the initial transforms
          */
        void add_initial_transforms(const std::vector<BodyPtr>& bodies, //!< Bodies containing the initial coordinates
                                    ssc::kinematics::KinematicsPtr& k) const;

        std::vector<bool> are_there_surface_forces_acting_on_body(const std::vector<ListOfForces>& forces) const;

    private:
        SimulatorBuilder(); // Disabled
        std::map<std::string, double> get_max_history_length(const std::vector<ListOfForces>& forces_for_all_bodies, const std::vector<ListOfControlledForces>& controlled_forces_for_all_bodies) const;
        void set_environment_models(EnvironmentAndFrames& env) const;
        ListOfForces forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const;
        ListOfControlledForces controlled_forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const;
        void add(const YamlModel& model, ListOfForces& L, const std::string& name, const EnvironmentAndFrames& env) const;
        void add(const YamlModel& model, ListOfControlledForces& L, const std::string& name, const EnvironmentAndFrames& env) const;
        VectorOfVectorOfPoints get_mesh(const YamlBody& body) const;

        YamlSimulatorInput input;
        TR1(shared_ptr)<BodyBuilder> builder;
        std::vector<ForceParser> force_parsers;
        std::vector<ControllableForceParser> controllable_force_parsers;
        std::vector<SurfaceElevationBuilderPtr> surface_elevation_parsers;
        TR1(shared_ptr)<std::vector<WaveModelBuilderPtr> > wave_parsers;
        TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> > directional_spreading_parsers;
        TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> > spectrum_parsers;
        std::vector<WindParser> wind_model_parsers;
        ssc::data_source::DataSource command_listener;
        double t0; //!< First time step (to initialize state history)
};


#endif /* SIMULATORBUILDER_HPP_ */

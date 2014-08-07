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

#include "ForceBuilder.hpp"
#include "WaveBuilder.hpp"

#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

class EnvironmentAndFrames;
class BodyBuilder;
typedef std::map<std::string, VectorOfVectorOfPoints> MeshMap;


/** \author cec
 *  \date Aug 7, 2014, 2:05:46 PM
 *  \brief Builds a Sim object which can be used with quicksolve
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

        /**  \author cec
          *  \date Aug 7, 2014, 2:55:16 PM
          *  \brief Builds a Sim object using the supplied mesh map (one mesh per body)
          *  \details This function is mainly used in the integration tests.
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest build_example
          */
        Sim build(const MeshMap& input_meshes //!< Map containing a mesh for each body
                 ) const;

        /**  \author cec
          *  \date Aug 7, 2014, 2:55:16 PM
          *  \brief Builds a Sim object reading the meshes from files
          *  \details Reads the STL data from an STL file & call the version of
          *           the build method that accepts a MeshMap as input.
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest build_example
          */
        Sim build() const;

        /**  \author cec
          *  \date Aug 7, 2014, 2:09:04 PM
          *  \brief Add the capacity to parse certain YAML inputs for waves
          *  \details This method must not be called with any parameters: the
          *  default parameter is only there so we can use boost::enable_if. This
          *  allows us to use can_parse for several types derived from a few
          *  base classes (WaveModelInterface, ForceModel...) & the compiler will
          *  automagically choose the right version of can_parse.
          *  \returns *this (so we can chain calls to can_parse)
          *  \snippet simulator/unit_tests/src/SimulatorBuilderTest.cpp SimulatorBuilderTest can_parse_example
          */
        template <typename T> SimulatorBuilder& can_parse(typename boost::enable_if<boost::is_base_of<WaveModelInterface,T> >::type* dummy = 0)
        {
            (void)dummy; // Ignore "unused variable" warning: we just need "dummy" for boost::enable_if
            wave_parsers.push_back(WaveBuilderPtr(new WaveBuilder<T>()));
            return *this;
        }

        /**  \author cec
          *  \date Aug 7, 2014, 2:09:04 PM
          *  \brief Add the capacity to parse certain YAML inputs for forces
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

        std::vector<Body> get_bodies(const MeshMap& meshes) const;
        EnvironmentAndFrames get_environment_and_frames(const std::vector<Body>& bodies) const;
        std::vector<ListOfForces> get_forces(const EnvironmentAndFrames& env) const;
        StateType get_initial_states() const;
        YamlSimulatorInput get_parsed_yaml() const;

    private:
        SimulatorBuilder(); // Disabled
        WavePtr get_wave() const;
        ListOfForces forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const;
        void add(const YamlModel& model, ListOfForces& L, const EnvironmentAndFrames& env) const;
        MeshMap make_mesh_map() const;
        VectorOfVectorOfPoints get_mesh(const YamlBody& body) const;

        YamlSimulatorInput input;
        TR1(shared_ptr)<BodyBuilder> builder;
        std::vector<ForceBuilderPtr> force_parsers;
        std::vector<WaveBuilderPtr> wave_parsers;
};


#endif /* SIMULATORBUILDER_HPP_ */

/*
 * dynamho_tests.cpp
 *
 *  Created on: 24 févr. 2014
 *      Author: maroff
 */

#include "dynamho_tests.hpp"
#include "generate_yaml_data.hpp"
#include "DynamhoYamlParser.hpp"
#include "DynamhoSimulation.hpp"
#include "DataSource.hpp"
#include "DataSourceModule.hpp"
#include "hydrodynamic.hpp"
#include "hydrostatic.hpp"
#include "inertia_coupling.hpp"
#include "propulsion.hpp"
#include "test_macros.hpp"

#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>

#include "simulator.hpp"



dynamho_tests::dynamho_tests() : a(DataGenerator(21452)), yaml(generate_yaml())
{
}

dynamho_tests::~dynamho_tests()
{
}

void dynamho_tests::SetUp()
{
}

void dynamho_tests::TearDown()
{
}

typedef  Eigen::SelfAdjointView<Eigen::Matrix<double, 6, 6>, Eigen::Upper> InertiaMatrix;
typedef Eigen::Matrix<double, 6, 1> Force;

#define FORCE_MODULE(ns, x, code) namespace ns {namespace x{static const std::string signal_name = #ns "_" #x;MODULE(ForceModule, code)}}
#define APPEND_FORCE(x, ds) ds.add<x::ForceModule>(#x);\
	                        try\
	                        {\
	                        	ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                }\
                                catch (const DataSourceException& exc)\
                                {\
                                    ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,simulator_base::list_of_forces::_type());\
                                }\
                                    {\
                                        auto __v = ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                        __v.insert(x::signal_name);\
                                        ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,__v);\
                                    }

DEFINE(simulator_base, list_of_forces, std::set<std::string>)
DEFINE(dynamho, sim_start_stop, SimulationStartStopParameters)
DEFINE(dynamho, inertia_matrix, InertiaMatrix)
DEFINE(dynamho, inertia_parameters, Inertia)
DEFINE(dynamho, added_mass, InertiaMatrix)
DEFINE(dynamho, generalized_inertia_matrix, InertiaMatrix)
DEFINE(dynamho, hydrostatic, Force)
DEFINE(dynamho, inertial_coupling, Force)
DEFINE(dynamho, hydrodynamic, Force)
DEFINE(dynamho, hydrodynamic_parameters, ::hydrodynamic::Parameters)
DEFINE(dynamho, propulsion, Force)
DEFINE(dynamho, external, Force)
DEFINE(dynamho, sum_of_all_forces, Force)
DEFINE(dynamho, geometry, GeometryAndEnvironment)
DEFINE(dynamho, euler_angles, ::EulerAngles<double>)
DEFINE(dynamho, angles_de_barre, AngleDeBarre<double>)
DEFINE(dynamho, yaml, std::string)
DEFINE(dynamho, yaml_parser, std::tr1::shared_ptr<DynamhoYamlParser>)
DEFINE(dynamho, speed, Speed<double>)
DEFINE(dynamho, CzB2, double)
DEFINE(dynamho, Yb2B, double)

double hydrostatic::model::Fx(const Inertia& I, const GeometryAndEnvironment& G, const EulerAngles<double>& angles);

FORCE_MODULE(dynamho, hydrostatic, const auto p = PTR_GET(dynamho::inertia_parameters);\
                                   const auto g = PTR_GET(dynamho::geometry);\
                                   const auto a = PTR_GET(dynamho::euler_angles);\
                                   const Force f;\
                                   (void)p;(void)g;(void)a;\
                                   PTR_SET(dynamho::hydrostatic, f))

MODULE(dynamhoparser, const auto yaml = PTR_GET(dynamho::yaml);\
                      const DynamhoYamlParser parser(yaml);\
                      PTR_SET(dynamho::inertia_parameters, parser.get_inertia());\
                      PTR_SET(dynamho::geometry, parser.get_geometry());)

TEST_F(dynamho_tests, should_get_the_same_results_with_dynamho_and_DataSource)
{
//! [dynamho_tests example]
	auto yaml = generate_yaml();
	const DynamhoYamlParser parser(yaml);
	DynamhoSimulation dynamho(parser.get_simulation_parameters());
	const auto out1 = dynamho.initialize(parser.get_simulation_start_stop_parameters());
	ASSERT_DOUBLE_EQ(0,out1.ship_states.p.coord.x);

	boost::replace_all(yaml, "model", "dynamho");

	DataSource ds;


	ds.set<std::string>("yaml",yaml);
	ds.add<dynamhoparser>("parser");
	APPEND_FORCE(dynamho::hydrostatic, ds);
	const auto angles = parser.get_simulation_start_stop_parameters().initial_angles;
	//SET(ds,dynamho::euler_angles,angles);

//! [dynamho_tests example]
//! [dynamho_tests expected output]
//! [dynamho_tests expected output]
}

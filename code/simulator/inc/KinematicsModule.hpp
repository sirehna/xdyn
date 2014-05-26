/*
 * KinematicsModule.hpp
 *
 *  Created on: Apr 25, 2014
 *      Author: cady
 */

#ifndef KINEMATICSMODULE_HPP_
#define KINEMATICSMODULE_HPP_

#include "DataSourceModule.hpp"
#include "Transform.hpp"
#include <vector>

class Kinematics;

/** \author cec
 *  \date Apr 28, 2014, 12:09:02 PM
 *  \brief Updates a Kinematics object with the lates state values
 *  \details Computes the transforms from each body frame to the NED frame & sets them in the Kinematics object.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/KinematicsModuleTest.cpp KinematicsModuleTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/KinematicsModuleTest.cpp KinematicsModuleTest expected output
 */
class KinematicsModule : public DataSourceModule
{
	public:
	    KinematicsModule(DataSource* const data_source, const std::vector<std::string>& bodies_, const std::string& module_name);
	    KinematicsModule(const KinematicsModule& rhs, DataSource* const data_source);
		DataSourceModule* clone() const;
		DataSourceModule* clone(DataSource* const data_source) const;
		void update() const;

	private:
		kinematics::Transform get_transform_from_ned_to(const std::string& body) const;
		kinematics::Transform get_transform_from_mesh_to(const std::string& body) const;
		Point get_origin(const std::string& body) const;
		RotationMatrix get_rot_from_ned_to(const std::string& body) const;
		Point get_position_of_body_relative_to_mesh(const std::string& body) const;

		std::vector<std::string> bodies;
		std::tr1::shared_ptr<Kinematics> kinematics;
};

#endif /* KINEMATICSMODULE_HPP_ */

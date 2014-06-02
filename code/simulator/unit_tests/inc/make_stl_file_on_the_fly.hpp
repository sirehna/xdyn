/*
 * make_stl_file_on_the_fly.hpp
 *
 *  Created on: Jun 2, 2014
 *      Author: cady
 */

#ifndef MAKE_STL_FILE_ON_THE_FLY_HPP_
#define MAKE_STL_FILE_ON_THE_FLY_HPP_

#include <string>

inline bool file_exists(const std::string& name);
void make_stl_file(const std::string& data, const std::string& filename);


#endif /* MAKE_STL_FILE_ON_THE_FLY_HPP_ */

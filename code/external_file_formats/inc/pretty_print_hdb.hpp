/*
 * pretty_print_hdb.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef PRETTY_PRINT_HDB_HPP_
#define PRETTY_PRINT_HDB_HPP_

#include "hdb_parser_internal_data_structures.hpp"

std::ostream& operator<<(std::ostream& os, const hdb::Key<double>& k);
std::ostream& operator<<(std::ostream& os, const hdb::Key<std::string>& k);
std::ostream& operator<<(std::ostream& os, const hdb::Values& k);
std::ostream& operator<<(std::ostream& os, const hdb::MatrixSection& k);
std::ostream& operator<<(std::ostream& os, const hdb::AST& f);

#endif /* PRETTY_PRINT_HDB_HPP_ */

/*
 * hdb_to_ast.hpp
 *
 *  Created on: Nov 7, 2014
 *      Author: cady
 */

#ifndef HDB_TO_AST_HPP_
#define HDB_TO_AST_HPP_

#include "hdb_parser_internal_data_structures.hpp"

namespace hdb
{
    AST parse(const std::string& contents);
}

#endif /* HDB_TO_AST_HPP_ */

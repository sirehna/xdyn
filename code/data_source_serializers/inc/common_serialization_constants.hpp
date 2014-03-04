/*
 * common_serialization_constants.hpp
 *
 *  Created on: 3 mars 2014
 *      Author: maroff
 */

#ifndef COMMON_SERIALIZATION_CONSTANTS_HPP_
#define COMMON_SERIALIZATION_CONSTANTS_HPP_

#include <string>
#include "test_macros.hpp"

namespace common_serialization_constants
{
#if defined(__clang__)
        /* Clang/LLVM. ---------------------------------------------- */
        const std::string compiler_name = QUOTE(__clang__) " " QUOTE(__clang_version__);
#elif defined(__ICC)
        /* Intel ICC/ICPC. ------------------------------------------ */
        const std::string compiler_name = QUOTE(__ICC);
#elif (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
        /* GNU GCC/G++. --------------------------------------------- */
        const std::string compiler_name = "GNU GCC " QUOTE(__GNUC__) "." QUOTE(__GNUC_MINOR__) "." QUOTE(__GNUC_PATCHLEVEL__);
#elif defined(__HP_cc)
        /* Hewlett-Packard C/aC++. ---------------------------------- */
        const std::string compiler_name = QUOTE(__HP_cc);
#elif defined(__HP_aCC)
        /* Hewlett-Packard C/aC++. ---------------------------------- */
        const std::string compiler_name = QUOTE(__HP_aCC);
#elif defined(__IBMCPP__)
        /* IBM XL C/C++. -------------------------------------------- */
        const std::string compiler_name = QUOTE(__IBMCPP__);
#elif defined(_MSC_VER)
        /* Microsoft Visual Studio. --------------------------------- */
        const std::string compiler_name = QUOTE(_MSC_VER);
#elif defined(__PGI)
        /* Portland Group PGCC/PGCPP. ------------------------------- */
        const std::string compiler_name = QUOTE(__PGI);
#elif defined(__SUNPRO_CC)
        /* Oracle Solaris Studio. ----------------------------------- */
        const std::string compiler_name = QUOTE(__SUNPRO_CC);
#endif

        const std::string compiler_key = "compiler used for serialization";
        const std::string data_source_key = "data source";
        const std::string modules_key = "modules";
        const std::string signals_key = "signals";
}



#endif /* COMMON_SERIALIZATION_CONSTANTS_HPP_ */

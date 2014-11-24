/*
 * pretty_print_hdb.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include <ostream>

#include "pretty_print_hdb.hpp"


std::ostream& operator<<(std::ostream& os, const hdb::Key<double>& k)
{
    os << "[" << k.header << "] = " << k.value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::Key<std::string>& k)
{
    os << "[" << k.header << "] = " << k.value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::Values& k)
{
    os << "[";
    for (size_t i = 0 ; i < k.size()-1 ; ++i)
    {
        os << k.at(i) << ",";
    }
    if (not(k.empty())) os << k.back();
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::MatrixSection& k)
{
    os << "[" << k.header << "] =\n";
    for (auto it  = k.values.begin() ; it != k.values.end() ; ++it)
    {
        os << '\t' << *it << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const hdb::AST& f)
{
    os << "\n*** Value keys: " << f.value_keys.size() << "\n";
    for (auto it = f.value_keys.begin() ; it != f.value_keys.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** String keys: " << f.string_keys.size() << "\n";
    for (auto it = f.string_keys.begin() ; it != f.string_keys.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** Matrix sections: " << f.matrix_sections.size() << "\n";
    for (auto it = f.matrix_sections.begin() ; it != f.matrix_sections.end() ; ++it)
    {
        os << *it << std::endl;
    }
    os << "\n*** Lists of matrix sections: " << f.lists_of_matrix_sections.size() << "\n";
    for (auto it = f.lists_of_matrix_sections.begin() ; it != f.lists_of_matrix_sections.end() ; ++it)
    {
        os << "[" << it->header << "] =\n";
        for (auto it2 = it->sections.begin() ; it2 != it->sections.end() ; ++it2)
        {
            os << '\t' << it2->header << std::endl;
            for (auto it3 = it2->values.begin() ; it3 != it2->values.end() ; ++it3)
            {
                os << "\t\t" << *it3 << std::endl;
            }
        }
    }
    os << "\n*** Lists of matrix sections with ID: " << f.lists_of_matrix_sections_with_id.size() << "\n";
    for (auto it = f.lists_of_matrix_sections_with_id.begin() ; it != f.lists_of_matrix_sections_with_id.end() ; ++it)
    {
        os << "[" << it->header << "] =\n";
        for (auto it2 = it->sections_with_id.begin() ; it2 != it->sections_with_id.end() ; ++it2)
        {
            os << '\t' << it2->header << ", ID: " << it2->id << std::endl;
            for (auto it3 = it2->values.begin() ; it3 != it2->values.end() ; ++it3)
            {
                os << "\t\t" << *it3 << std::endl;
            }
        }
    }
    os << "\n*** Vector sections: " << f.vector_sections.size() << "\n";
    for (auto it = f.vector_sections.begin() ; it != f.vector_sections.end() ; ++it)
    {
        os << "[" << it->header << "] = " << it->values << std::endl;
    }
    return os;
}


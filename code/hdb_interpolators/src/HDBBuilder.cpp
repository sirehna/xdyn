/*
 * HDBBuilder.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include <sstream>

#include "HDBBuilder.hpp"
#include "HDBBuilderException.hpp"
#include "hdb_to_ast.hpp"

HDBBuilder::HDBBuilder(const std::string& data) : tree(hdb::parse(data))
{
}

void HDBBuilder::fill(TimestampedMatrices& ret, const size_t i, const hdb::ListOfValues& M) const
{
    if (ret.empty()) ret.resize(M.size());
    if (M.size() != ret.size())
    {
        std::stringstream ss;
        ss << "Something is wrong with the HDB file: detected "
           << ret.size()
           << " periods in Added_mass_Radiation_Damping, but line "
           << i << " has " << M.size() << " periods";
        THROW(__PRETTY_FUNCTION__, HDBBuilderException,  ss.str());
    }
    for (size_t k = 0 ; k < M.size() ; ++k)
    {
        ret[k].first = M.at(k).front();
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ret[k].second[i][j] = M.at(k).at(j+1);
        }
    }
}

TimestampedMatrices HDBBuilder::get_added_mass() const
{
    TimestampedMatrices ret;
    std::vector<bool> found_line(6,false);
    for (auto M = tree.lists_of_matrix_sections.begin() ; M != tree.lists_of_matrix_sections.end() ; ++M)
    {
        if (M->header == "Added_mass_Radiation_Damping")
        {
            for (auto that_section = M->sections.begin() ; that_section != M->sections.end() ; ++that_section)
            {
                if (that_section->header == "ADDED_MASS_LINE_1") fill(ret, 0, that_section->values); found_line[0] = true;
                if (that_section->header == "ADDED_MASS_LINE_2") fill(ret, 1, that_section->values); found_line[1] = true;
                if (that_section->header == "ADDED_MASS_LINE_3") fill(ret, 2, that_section->values); found_line[2] = true;
                if (that_section->header == "ADDED_MASS_LINE_4") fill(ret, 3, that_section->values); found_line[3] = true;
                if (that_section->header == "ADDED_MASS_LINE_5") fill(ret, 4, that_section->values); found_line[4] = true;
                if (that_section->header == "ADDED_MASS_LINE_6") fill(ret, 5, that_section->values); found_line[5] = true;
            }
        }
    }
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        if (not(found_line[i]))
        {
            std::stringstream ss;
            ss << "Unable to find key 'ADDED_MASS_LINE_" << i+1 << "' in HDB file";
            THROW(__PRETTY_FUNCTION__, HDBBuilderException, "Unable to find key 'ADDED_MASS_LINE_1' in HDB file");
        }
    }
    return ret;
}

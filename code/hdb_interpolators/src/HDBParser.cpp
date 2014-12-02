/*
 * HDBBuilder.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include <list>
#include <set>
#include <sstream>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <boost/lexical_cast.hpp>

#include <ssc/interpolation.hpp>

#include "hdb_parser_internal_data_structures.hpp"
#include "HDBParser.hpp"
#include "HDBBuilderException.hpp"
#include "hdb_to_ast.hpp"

class HDBParser::Impl
{
    public:
        Impl(const std::string& data) : tree(hdb::parse(data)), M(), Br(), Tmin(0)
        {
            (void)Tmin;
        }

        void fill(TimestampedMatrices& ret, const size_t i, const hdb::ListOfValues& M) const
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

        TimestampedMatrices get_matrix(const std::string& header, const std::string& matrix) const
        {
            TimestampedMatrices ret;
            std::vector<bool> found_line(6,false);
            for (auto M = tree.lists_of_matrix_sections.begin() ; M != tree.lists_of_matrix_sections.end() ; ++M)
            {
                if (M->header == header)
                {
                    for (auto that_section = M->sections.begin() ; that_section != M->sections.end() ; ++that_section)
                    {
                        for (size_t i = 0 ; i < 6 ; ++i)
                        {
                            if (that_section->header == matrix + "_" + boost::lexical_cast<std::string>(i+1)) fill(ret, i, that_section->values); found_line[i] = true;
                        }
                    }
                }
            }
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                if (not(found_line[i]))
                {
                    std::stringstream ss;
                    ss << "Unable to find key '" << matrix << "_" << i+1 << "' in HDB file";
                    THROW(__PRETTY_FUNCTION__, HDBBuilderException, ss.str());
                }
            }
            return ret;
        }

        RAOData get_rao(const std::string& section_name, const std::string& subsections) const
        {
            std::set<double> omegas, psi;
            RAOData ret;
            for (auto ms:tree.lists_of_matrix_sections_with_id)
            {
                if (ms.header == section_name)
                {
                    size_t psi_idx = 0;
                    for (auto s:ms.sections_with_id)
                    {
                        if (s.header == subsections)
                        {
                            psi.insert(s.id*PI/180.);
                            std::array<std::vector<double>,6> psi_for_each_axis;
                            size_t omega_idx = 0;
                            for (auto v:s.values)
                            {
                                omegas.insert(2*PI/v.front());
                                for (size_t j = 0 ; j < 6 ; ++j)
                                {
                                    if (ret.values.at(j).size()<omega_idx+1) ret.values.at(j).push_back(std::vector<double>());
                                    if (ret.values.at(j).at(omega_idx).size()<psi_idx+1) ret.values.at(j).at(omega_idx).resize(psi_idx+1);
                                    ret.values.at(j).at(omega_idx).at(psi_idx) = v.at(j+1);
                                }
                                omega_idx++;
                            }
                            psi_idx++;
                        }
                    }
                }
            }
            std::list<double> omega_l(omegas.begin(), omegas.end());
            omega_l.sort();
            ret.omega.insert(ret.omega.begin(),omega_l.begin(), omega_l.end());
            std::list<double> psi_l(psi.begin(), psi.end());
            omega_l.sort();
            ret.psi.insert(ret.psi.begin(),psi_l.begin(), psi_l.end());
            return ret;
        }

        RAOData get_diffraction_module() const
        {
            return get_rao("FROUDE-KRYLOV_FORCES_AND_MOMENTS", "INCIDENCE_EFM_MOD_001");
        }

        RAOData get_diffraction_phase() const
        {
            return get_rao("FROUDE-KRYLOV_FORCES_AND_MOMENTS", "INCIDENCE_EFM_PH_001");
        }

        TimestampedMatrices get_added_mass() const
        {
            return get_matrix("Added_mass_Radiation_Damping", "ADDED_MASS_LINE");
        }

        TimestampedMatrices get_radiation_damping() const
        {
            return get_matrix("Added_mass_Radiation_Damping", "DAMPING_TERM");
        }

    private:
        std::vector<double> get_Tp(const TimestampedMatrices& M)
        {
            std::vector<double> ret;
            for (size_t k = 0 ; k < M.size() ; ++k) ret.push_back(M.at(k).first);
            return ret;
        }

        std::vector<double> get_Mij_for_each_Tp(const TimestampedMatrices& M, const size_t i, const size_t j)
        {
            std::vector<double> ret;
            for (size_t k = 0 ; k < M.size() ; ++k) ret.push_back(M.at(k).second.at(i).at(j));
            return ret;
        }

        hdb::AST tree;
        std::array<std::array<ssc::interpolation::SplineVariableStep,6>,6> M;
        std::array<std::array<std::vector<double>,6>,6> Br;
        double Tmin;
};


HDBParser::HDBParser(const std::string& data) : pimpl(new Impl(data))
{
}

TimestampedMatrices HDBParser::get_added_mass() const
{
    return pimpl->get_added_mass();
}

TimestampedMatrices HDBParser::get_radiation_damping() const
{
    return pimpl->get_radiation_damping();
}

RAOData HDBParser::get_diffraction_module() const
{
    return pimpl->get_diffraction_module();
}

RAOData HDBParser::get_diffraction_phase() const
{
    return pimpl->get_diffraction_phase();
}

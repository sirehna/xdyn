/*
 * HDBParser.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include "HDBParser.hpp"

#include <list>
#include <set>
#include <sstream>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

#include <ssc/interpolation.hpp>

#include "hdb_parser_internal_data_structures.hpp"
#include "InvalidInputException.hpp"
#include "hdb_to_ast.hpp"



class HDBParser::Impl
{
    public:
        Impl() : omega_rad(), tree(), M(), Br(), Tmin(0), diffraction_module(), diffraction_phase(), froude_krylov_module(), froude_krylov_phase()
        {
        }

        Impl(const std::string& data)
        : omega_rad()
        , tree(hdb::parse(data))
        , M()
        , Br()
        , Tmin(0)
        , diffraction_module(get_diffraction_module())
        , diffraction_phase(get_diffraction_phase())
        , froude_krylov_module(get_froude_krylov_module())
        , froude_krylov_phase(get_froude_krylov_phase())
        {
            bool allow_queries_outside_bounds;
            const TimestampedMatrices Ma = get_added_mass_array();
            const TimestampedMatrices B_r = get_radiation_damping_array();
            const auto x = get_Tp(Ma);
            Tmin = x.front();
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    M[i][j] = ssc::interpolation::SplineVariableStep(x, get_Mij_for_each_Tp(Ma,i,j), allow_queries_outside_bounds=true);
                    Br[i][j] = get_Mij_for_each_Tp(B_r, i, j);
                }
            }

            auto v = get_Tp(B_r);
            for (auto it = v.rbegin(); it != v.rend() ; ++it)
            {
                if(*it==0)
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Zero period detected: cannot compute angular frequency. Check Added_mass_Radiation_Damping section of the HDB file.");
                }
                omega_rad.push_back(2*PI/ *it);
            }
        }

        void fill(TimestampedMatrices& ret, const size_t i, const hdb::ListOfValues& M) const
        {
            if (ret.empty()) ret.resize(M.size());
            if (M.size() != ret.size())
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException,
                        "Something is wrong with the HDB file: detected "
                        << ret.size()
                        << " periods in Added_mass_Radiation_Damping, but line "
                        << i << " has " << M.size() << " periods");
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

        TimestampedMatrices convert_matrices_from_aquaplus_to_xdyn_frame(TimestampedMatrices matrices) const
        {
            for (size_t k = 0 ; k < matrices.size() ; ++k)
            {
                matrices[k].second[0][1] = -matrices[k].second[0][1];
                matrices[k].second[0][2] = -matrices[k].second[0][2];
                matrices[k].second[0][4] = -matrices[k].second[0][4];
                matrices[k].second[0][5] = -matrices[k].second[0][5];
                matrices[k].second[1][0] = -matrices[k].second[1][0];
                matrices[k].second[1][3] = -matrices[k].second[1][3];
                matrices[k].second[2][0] = -matrices[k].second[2][0];
                matrices[k].second[2][3] = -matrices[k].second[2][3];
                matrices[k].second[3][1] = -matrices[k].second[3][1];
                matrices[k].second[3][2] = -matrices[k].second[3][2];
                matrices[k].second[3][4] = -matrices[k].second[3][4];
                matrices[k].second[3][5] = -matrices[k].second[3][5];
                matrices[k].second[4][0] = -matrices[k].second[4][0];
                matrices[k].second[4][3] = -matrices[k].second[4][3];
                matrices[k].second[5][0] = -matrices[k].second[5][0];
                matrices[k].second[5][3] = -matrices[k].second[5][3];
                // We need to transform the matrix into X-DYN's frame (z downwards)
                /*
                 *  import sympy

                    m = sympy.Matrix([[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1],[1,1,1,1,1,1]])
                    R = sympy.Matrix([[1,0,0,0,0,0],[0,-1,0,0,0,0],[0,0,-1,0,0,0],[0,0,0,1,0,0],[0,0,0,0,-1,0],[0,0,0,0,0,-1]])
                    for i in range(1,7):
                        for j in range(1,7):
                        exec("m"+str(i)+str(j)+" = sympy.Symbol('m" + str(i) + str(j) + "')")
                    M = sympy.Matrix([[m11,m12,m13,m14,m15,m16],[m21,m22,m23,m24,m25,m26],[m31,m32,m33,m34,m35,m36],[m41,m42,m43,m44,m45,m46],[m51,m52,m53,m54,m55,m56],[m61,m62,m63,m64,m65,m66]])
                    M
                    ⎡m₁₁   -m₁₂  -m₁₃  m₁₄   -m₁₅  -m₁₆⎤
                    ⎢                                  ⎥
                    ⎢-m₂₁  m₂₂   m₂₃   -m₂₄  m₂₅   m₂₆ ⎥
                    ⎢                                  ⎥
                    ⎢-m₃₁  m₃₂   m₃₃   -m₃₄  m₃₅   m₃₆ ⎥
                    ⎢                                  ⎥
                    ⎢m₄₁   -m₄₂  -m₄₃  m₄₄   -m₄₅  -m₄₆⎥
                    ⎢                                  ⎥
                    ⎢-m₅₁  m₅₂   m₅₃   -m₅₄  m₅₅   m₅₆ ⎥
                    ⎢                                  ⎥
                    ⎣-m₆₁  m₆₂   m₆₃   -m₆₄  m₆₅   m₆₆ ⎦
                    def f(m):
                        for i in range(0,6):
                             for j in range(0,6):
                                 if m[i, j]<0:
                                     print("matrices[k].second["+str(i)+"]["+str(j)+"] = -matrices[k].second["+str(i)+"]["+str(j)+"];")
                    f(sympy.transpose(R)*m*R)

                 */
            }
            return matrices;
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
                            if (that_section->header == matrix + "_" + boost::lexical_cast<std::string>(i+1))
                            {
                                fill(ret, i, that_section->values);
                                found_line[i] = true;
                            }
                        }
                    }
                }
            }
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                if (not(found_line[i]))
                {
                    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find key '" << matrix << "_" << i+1 << "' in HDB file");
                }
            }
            return convert_matrices_from_aquaplus_to_xdyn_frame(ret);
        }

        boost::variant<RAOData,std::string> get_rao(const std::string& section_name, const std::string& subsection_name) const
        {
            std::set<double> periods, psi;
            RAOData ret;
            bool found_relevant_rao_section = false;
            bool found_relevant_rao_subsection = false;
            for (auto ms:tree.lists_of_matrix_sections_with_id)
            {
                if (ms.header == section_name)
                {
                    found_relevant_rao_section = true;
                    size_t psi_idx = 0;
                    for (auto s:ms.sections_with_id)
                    {
                        if (s.header == subsection_name)
                        {
                            found_relevant_rao_subsection = true;
                            psi.insert(s.id*PI/180.);
                            std::array<std::vector<double>,6> psi_for_each_axis;
                            size_t period_idx = 0;
                            for (auto v:s.values)
                            {
                                periods.insert(v.front());
                                for (size_t j = 0 ; j < 6 ; ++j)
                                {
                                    if (ret.values.at(j).size()<period_idx+1) ret.values.at(j).push_back(std::vector<double>());
                                    if (ret.values.at(j).at(period_idx).size()<psi_idx+1) ret.values.at(j).at(period_idx).resize(psi_idx+1);
                                    ret.values.at(j).at(period_idx).at(psi_idx) = v.at(j+1);
                                }
                                period_idx++;
                            }
                            psi_idx++;
                        }
                    }
                }
            }
            std::stringstream error_msg;
            if (not(found_relevant_rao_section))
            {
                return std::string("Unable to find section '") + section_name + "' in the RAO file.";
            }
            else if (not(found_relevant_rao_subsection))
            {
                return std::string("Found section '") + section_name + "' but could not find subsection '" + subsection_name + "' in the RAO file.";
            }
            std::list<double> period_list(periods.begin(), periods.end());
            period_list.sort();
            ret.periods.insert(ret.periods.begin(),period_list.begin(), period_list.end());
            std::list<double> psi_l(psi.begin(), psi.end());
            psi_l.sort();
            ret.psi.insert(ret.psi.begin(),psi_l.begin(), psi_l.end());
            return ret;
        }

        boost::variant<RAOData,std::string> get_diffraction_module() const
        {
            return get_rao("DIFFRACTION_FORCES_AND_MOMENTS", "INCIDENCE_EFM_MOD_001");
        }

        boost::variant<RAOData,std::string> get_diffraction_phase() const
        {
            return get_rao("DIFFRACTION_FORCES_AND_MOMENTS", "INCIDENCE_EFM_PH_001");
        }

        boost::variant<RAOData,std::string> get_froude_krylov_module() const
        {
            return get_rao("FROUDE-KRYLOV_FORCES_AND_MOMENTS", "INCIDENCE_EFM_MOD_001");
        }

        boost::variant<RAOData,std::string> get_froude_krylov_phase() const
        {
            return get_rao("FROUDE-KRYLOV_FORCES_AND_MOMENTS", "INCIDENCE_EFM_PH_001");
        }

        std::array<std::vector<std::vector<double> >,6 > get_diffraction_module_tables() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_module);
            return ret->values;
        }

        std::array<std::vector<std::vector<double> >,6 > get_diffraction_phase_tables() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
            return ret->values;
        }

        std::array<std::vector<std::vector<double> >,6 > get_froude_krylov_module_tables() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
            return ret->values;
        }

        std::array<std::vector<std::vector<double> >,6 > get_froude_krylov_phase_tables() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
            return ret->values;
        }

        TimestampedMatrices get_added_mass_array() const
        {
            return get_matrix("Added_mass_Radiation_Damping", "ADDED_MASS_LINE");
        }

        TimestampedMatrices get_radiation_damping_array() const
        {
            return get_matrix("Added_mass_Radiation_Damping", "DAMPING_TERM");
        }

        Eigen::Matrix<double,6,6> get_added_mass(const double Tp)
        {
            Eigen::Matrix<double,6,6> ret;
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    ret((int)i,(int)j) = M[i][j].f(Tp);
                }
            }
            return ret;
        }

        Eigen::Matrix<double,6,6> get_added_mass()
        {
            return get_added_mass(Tmin);
        }

        std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const
        {
            const auto v = Br[i][j];
            return std::vector<double>(v.rbegin(), v.rend());
        }

        std::vector<double> get_diffraction_phase_psis() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
            return ret->psi;
        }

        std::vector<double> get_diffraction_phase_omegas() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_phase);
            return ret->periods;
        }

        std::vector<double> get_diffraction_module_psis() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_module);
            return ret->psi;
        }

        std::vector<double> get_diffraction_module_periods() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&diffraction_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&diffraction_module);
            return ret->periods;
        }

        std::vector<double> get_froude_krylov_phase_psis() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
            return ret->psi;
        }

        std::vector<double> get_froude_krylov_phase_periods() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_phase))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_phase);
            return ret->periods;
        }

        std::vector<double> get_froude_krylov_module_psis() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
            return ret->psi;
        }

        std::vector<double> get_froude_krylov_module_periods() const
        {
            if ( std::string* err = (std::string*)boost::get<std::string>(&froude_krylov_module))
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
            }
            const RAOData* ret = boost::get<RAOData>(&froude_krylov_module);
            return ret->periods;
        }

        std::vector<double> omega_rad;

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
        boost::variant<RAOData,std::string> diffraction_module;
        boost::variant<RAOData,std::string> diffraction_phase;
        boost::variant<RAOData,std::string> froude_krylov_module;
        boost::variant<RAOData,std::string> froude_krylov_phase;
};


HDBParser::HDBParser(const std::string& data) : pimpl(new Impl(data))
{
}

HDBParser::HDBParser() : pimpl(new Impl())
{
}

HDBParser::~HDBParser()
{
}

TimestampedMatrices HDBParser::get_added_mass_array() const
{
    return pimpl->get_added_mass_array();
}

TimestampedMatrices HDBParser::get_radiation_damping_array() const
{
    return pimpl->get_radiation_damping_array();
}

RAOData HDBParser::get_diffraction_module() const
{
    auto diff = pimpl->get_diffraction_module();
    if ( std::string* err = (std::string*)boost::get<std::string>(&diff))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diff);
    return *ret;
}

RAOData HDBParser::get_diffraction_phase() const
{
    auto diff = pimpl->get_diffraction_phase();
    if ( std::string* err = (std::string*)boost::get<std::string>(&diff))
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, *err);
    }
    const RAOData* ret = boost::get<RAOData>(&diff);
    return *ret;
}

Eigen::Matrix<double,6,6> HDBParser::get_added_mass() const
{
    return pimpl->get_added_mass();
}

Eigen::Matrix<double,6,6> HDBParser::get_added_mass(const double Tp //!< Period at which to interpolate the added mass
                                                 ) const
{
    return pimpl->get_added_mass(Tp);
}

std::vector<double> HDBParser::get_radiation_damping_angular_frequencies() const
{
    return pimpl->omega_rad;

}

std::vector<double> HDBParser::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    return pimpl->get_radiation_damping_coeff(i, j);
}

std::array<std::vector<std::vector<double> >,6 > HDBParser::get_diffraction_module_tables() const
{
    return pimpl->get_diffraction_module_tables();
}

std::array<std::vector<std::vector<double> >,6 > HDBParser::get_diffraction_phase_tables() const
{
    return pimpl->get_diffraction_phase_tables();
}


std::array<std::vector<std::vector<double> >,6 > HDBParser::get_froude_krylov_module_tables() const
{
    return pimpl->get_froude_krylov_module_tables();
}

std::array<std::vector<std::vector<double> >,6 > HDBParser::get_froude_krylov_phase_tables() const
{
    return pimpl->get_froude_krylov_phase_tables();
}

std::vector<double> HDBParser::get_diffraction_phase_psis() const
{
    return pimpl->get_diffraction_phase_psis();
}

std::vector<double> HDBParser::get_diffraction_phase_periods() const
{
    return pimpl->get_diffraction_phase_omegas();
}

std::vector<double> HDBParser::get_diffraction_module_psis() const
{
    return pimpl->get_diffraction_module_psis();
}

std::vector<double> HDBParser::get_diffraction_module_periods() const
{
    return pimpl->get_diffraction_module_periods();
}

std::vector<double> HDBParser::get_froude_krylov_phase_psis() const
{
    return pimpl->get_froude_krylov_phase_psis();
}

std::vector<double> HDBParser::get_froude_krylov_phase_periods() const
{
    return pimpl->get_froude_krylov_phase_periods();
}

std::vector<double> HDBParser::get_froude_krylov_module_psis() const
{
    return pimpl->get_froude_krylov_module_psis();
}

std::vector<double> HDBParser::get_froude_krylov_module_periods() const
{
    return pimpl->get_froude_krylov_module_periods();
}

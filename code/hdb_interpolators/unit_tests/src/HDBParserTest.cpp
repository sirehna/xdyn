/*
 * HDBParserTest.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include "HDBParser.hpp"
#include "HDBParserTest.hpp"
#include "hdb_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

HDBParserTest::HDBParserTest() : a(ssc::random_data_generator::DataGenerator(833332))
{
}

HDBParserTest::~HDBParserTest()
{
}

void HDBParserTest::SetUp()
{
}

void HDBParserTest::TearDown()
{
}

TEST_F(HDBParserTest, can_get_added_mass)
{
    const HDBParser builder(test_data::anthineas_hdb());
    const auto Ma = builder.get_added_mass_array();
    ASSERT_EQ(6,Ma.size());
    ASSERT_DOUBLE_EQ(1,Ma.at(0).first);
    ASSERT_EQ(6,Ma.at(0).second.size());
    ASSERT_DOUBLE_EQ(1.097184E+04,Ma.at(0).second.at(0).at(0));
    ASSERT_DOUBLE_EQ(4.443533E+01,Ma.at(0).second.at(3).at(4));

    ASSERT_DOUBLE_EQ(2,Ma.at(1).first);
    ASSERT_EQ(6,Ma.at(1).second.size());
    ASSERT_DOUBLE_EQ(2.102286E+04,Ma.at(1).second.at(1).at(3));
    ASSERT_DOUBLE_EQ(2.702315E+05,Ma.at(1).second.at(2).at(4));

    ASSERT_DOUBLE_EQ(3,Ma.at(2).first);
    ASSERT_EQ(6,Ma.at(2).second.size());
    ASSERT_DOUBLE_EQ(7.183531E+01,Ma.at(2).second.at(5).at(0));
    ASSERT_DOUBLE_EQ(-8.938050E+01,Ma.at(2).second.at(2).at(1));

    ASSERT_DOUBLE_EQ(3.5,Ma.at(3).first);
    ASSERT_EQ(6,Ma.at(3).second.size());

    ASSERT_DOUBLE_EQ(3.8,Ma.at(4).first);
    ASSERT_EQ(6,Ma.at(4).second.size());

    ASSERT_DOUBLE_EQ(4,Ma.at(5).first);
    ASSERT_EQ(6,Ma.at(5).second.size());
}

TEST_F(HDBParserTest, can_retrieve_radiation_damping)
{
    const HDBParser builder(test_data::anthineas_hdb());
    const auto Br = builder.get_radiation_damping_array();
    ASSERT_EQ(6,Br.size());
    ASSERT_DOUBLE_EQ(1,  Br.at(0).first);
    ASSERT_DOUBLE_EQ(2,  Br.at(1).first);
    ASSERT_DOUBLE_EQ(3,  Br.at(2).first);
    ASSERT_DOUBLE_EQ(3.5,Br.at(3).first);
    ASSERT_DOUBLE_EQ(3.8,Br.at(4).first);
    ASSERT_DOUBLE_EQ(4,  Br.at(5).first);

    ASSERT_DOUBLE_EQ(6.771553E+03, Br.at(0).second.at(0).at(0));
    ASSERT_DOUBLE_EQ(2.194728E+05, Br.at(1).second.at(1).at(1));
    ASSERT_DOUBLE_EQ(1.488785E+05, Br.at(2).second.at(2).at(2));
    ASSERT_DOUBLE_EQ(8.694864E+04, Br.at(3).second.at(3).at(3));
    ASSERT_DOUBLE_EQ(5.476829E+06, Br.at(4).second.at(4).at(4));
    ASSERT_DOUBLE_EQ(4.374308E+06, Br.at(5).second.at(5).at(5));
}

TEST_F(HDBParserTest, can_retrieve_initial_values)
{
    //! [HDBDataTest example]
    const HDBParser data(test_data::anthineas_hdb());
    //! [HDBDataTest example]

    //! [HDBDataTest expected output]
    const auto M1 = data.get_added_mass(1);
    ASSERT_EQ(6, M1.cols());
    ASSERT_EQ(6, M1.rows());
    ASSERT_DOUBLE_EQ(1.503212E+04, (double)M1(2,0));
    ASSERT_DOUBLE_EQ(-1.618101E+02, (double)M1(2,1));
    ASSERT_DOUBLE_EQ(1.781351E+05, (double)M1(2,2));
    ASSERT_DOUBLE_EQ(-1.053934E+02, (double)M1(2,3));
    ASSERT_DOUBLE_EQ(3.267841E+05, (double)M1(2,4));
    ASSERT_DOUBLE_EQ(6.774041E+02, (double)M1(2,5));

    const auto M2 = data.get_added_mass(2);
    ASSERT_EQ(6, M2.cols());
    ASSERT_EQ(6, M2.rows());
    ASSERT_DOUBLE_EQ(3.032333E+01, (double)M2(3,0));
    ASSERT_DOUBLE_EQ(2.332197E+04, (double)M2(3,1));
    ASSERT_DOUBLE_EQ(-5.924372E+01, (double)M2(3,2));
    ASSERT_DOUBLE_EQ(2.132689E+05, (double)M2(3,3));
    ASSERT_DOUBLE_EQ(2.994200E+02, (double)M2(3,4));
    ASSERT_DOUBLE_EQ(3.943479E+04, (double)M2(3,5));
    //! [HDBDataTest expected output]
}

TEST_F(HDBParserTest, can_retrieve_added_mass_at_Tp_0)
{
    const HDBParser data(test_data::anthineas_hdb());
    const auto M = data.get_added_mass();
    ASSERT_EQ(6, M.cols());
    ASSERT_EQ(6, M.rows());
    ASSERT_DOUBLE_EQ(1.503212E+04, (double)M(2,0));
    ASSERT_DOUBLE_EQ(-1.618101E+02, (double)M(2,1));
    ASSERT_DOUBLE_EQ(1.781351E+05, (double)M(2,2));
    ASSERT_DOUBLE_EQ(-1.053934E+02, (double)M(2,3));
    ASSERT_DOUBLE_EQ(3.267841E+05, (double)M(2,4));
    ASSERT_DOUBLE_EQ(6.774041E+02, (double)M(2,5));
}

TEST_F(HDBParserTest, can_retrieve_angular_frequencies_for_radiation_damping)
{
    const HDBParser data(test_data::anthineas_hdb());
    const auto angular_frequencies = data.get_radiation_damping_angular_frequencies();
    ASSERT_EQ(6,        angular_frequencies.size());
    ASSERT_EQ(2*PI/1.,  angular_frequencies.at(5));
    ASSERT_EQ(2*PI/2.,  angular_frequencies.at(4));
    ASSERT_EQ(2*PI/3.,  angular_frequencies.at(3));
    ASSERT_EQ(2*PI/3.5, angular_frequencies.at(2));
    ASSERT_EQ(2*PI/3.8, angular_frequencies.at(1));
    ASSERT_EQ(2*PI/4.,  angular_frequencies.at(0));
}

TEST_F(HDBParserTest, can_retrieve_vectors_for_each_element_in_radiation_damping_matrix)
{
    const HDBParser data(test_data::anthineas_hdb());
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_EQ(6, data.get_radiation_damping_coeff(i,j).size());
        }
    }
    const auto v = data.get_radiation_damping_coeff(1,2);
    ASSERT_DOUBLE_EQ(-1.590935E+02, v.at(5));
    ASSERT_DOUBLE_EQ( 2.595528E+02, v.at(4));
    ASSERT_DOUBLE_EQ(-1.614637E+02, v.at(3));
    ASSERT_DOUBLE_EQ(-1.376756E+02, v.at(2));
    ASSERT_DOUBLE_EQ(-1.215545E+02, v.at(1));
    ASSERT_DOUBLE_EQ(-1.083372E+02, v.at(0));
}

TEST_F(HDBParserTest, can_retrieve_vector_of_vectors_for_RAOs)
{
    HDBParser data(test_data::anthineas_hdb());
    const std::array<std::vector<std::vector<double> >,6 > module = data.get_diffraction_module_tables();
    const std::array<std::vector<std::vector<double> >,6 > phase = data.get_diffraction_phase_tables();
    ASSERT_EQ(6,std::get<0>(module).size());
    ASSERT_EQ(6,std::get<1>(module).size());
    ASSERT_EQ(6,std::get<2>(module).size());
    ASSERT_EQ(6,std::get<3>(module).size());
    ASSERT_EQ(6,std::get<4>(module).size());
    ASSERT_EQ(6,std::get<5>(module).size());
    ASSERT_EQ(6,std::get<0>(phase).size());
    ASSERT_EQ(6,std::get<1>(phase).size());
    ASSERT_EQ(6,std::get<2>(phase).size());
    ASSERT_EQ(6,std::get<3>(phase).size());
    ASSERT_EQ(6,std::get<4>(phase).size());
    ASSERT_EQ(6,std::get<5>(phase).size());
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_EQ(13, module.at(i).at(j).size());
            ASSERT_EQ(13, phase.at(i).at(j).size());
        }
    }
    ASSERT_DOUBLE_EQ(3.098978E5,module.at(2).at(4).at(3));
    ASSERT_DOUBLE_EQ(7.774210E4,module.at(1).at(3).at(2));
    ASSERT_DOUBLE_EQ(1.459181E4,module.at(5).at(2).at(6));
    ASSERT_DOUBLE_EQ(-2.004334,phase.at(2).at(4).at(3));
    ASSERT_DOUBLE_EQ(3.041773,phase.at(1).at(3).at(2));
    ASSERT_DOUBLE_EQ(8.036613E-3,phase.at(5).at(2).at(6));
}

TEST_F(HDBParserTest, can_retrieve_omegas_for_RAOs)
{
    HDBParser data(test_data::anthineas_hdb());
    const std::vector<double> omegas1 = data.get_diffraction_phase_omegas();
    const std::vector<double> omegas2 = data.get_diffraction_module_omegas();
    ASSERT_EQ(6, omegas1.size());
    ASSERT_EQ(6, omegas2.size());
    ASSERT_DOUBLE_EQ(2.*PI/4., omegas1[0]);
    ASSERT_DOUBLE_EQ(2.*PI/3.8, omegas1[1]);
    ASSERT_DOUBLE_EQ(2.*PI/3.5, omegas1[2]);
    ASSERT_DOUBLE_EQ(2.*PI/3., omegas1[3]);
    ASSERT_DOUBLE_EQ(2.*PI/2., omegas1[4]);
    ASSERT_DOUBLE_EQ(2.*PI/1., omegas1[5]);
    ASSERT_DOUBLE_EQ(2.*PI/4., omegas2[0]);
    ASSERT_DOUBLE_EQ(2.*PI/3.8, omegas2[1]);
    ASSERT_DOUBLE_EQ(2.*PI/3.5, omegas2[2]);
    ASSERT_DOUBLE_EQ(2.*PI/3., omegas2[3]);
    ASSERT_DOUBLE_EQ(2.*PI/2., omegas2[4]);
    ASSERT_DOUBLE_EQ(2.*PI/1., omegas2[5]);

}

TEST_F(HDBParserTest, can_retrieve_psis_for_RAOs)
{
    HDBParser data(test_data::anthineas_hdb());
    const std::vector<double> psi1 = data.get_diffraction_phase_psis();
    const std::vector<double> psi2 = data.get_diffraction_module_psis();
    ASSERT_EQ(13, psi1.size());
    ASSERT_EQ(13, psi2.size());
    for (size_t i = 0 ; i < 13 ; ++i)
    {
        ASSERT_DOUBLE_EQ(PI/180.*15.*(double)i, psi1[i]);
        ASSERT_DOUBLE_EQ(PI/180.*15.*(double)i, psi2[i]);
    }
}

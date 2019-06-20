/*
 * HistoryTest.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#include <algorithm>    // std::transform
#include <numeric>      // std::partial_sum

#include "History.hpp"
#include "HistoryTest.hpp"
#include "InternalErrorException.hpp"

HistoryTest::HistoryTest() : a(ssc::random_data_generator::DataGenerator(5422))
{
}

HistoryTest::~HistoryTest()
{
}

void HistoryTest::SetUp()
{
}

void HistoryTest::TearDown()
{
}

TEST_F(HistoryTest, should_not_throw_even_if_retrieving_value_too_far_in_the_past) // Cf. bug 3003
{
    const double Tmax = a.random<double>().between(0,100);
    const double t_lower_than_Tmax = a.random<double>().between(0,Tmax);
    const double t_greater_than_Tmax = a.random<double>().greater_than(Tmax);
    History h(Tmax);
    const double t0 = a.random<double>().between(Tmax,10*Tmax);
    h.record(t0, a.random<double>());
    h.record(t0+Tmax, a.random<double>());
    ASSERT_NO_THROW(h(t_lower_than_Tmax));
    ASSERT_NO_THROW(h(Tmax));
    ASSERT_NO_THROW(h(t_greater_than_Tmax));
}

TEST_F(HistoryTest, can_retrieve_initial_values)
{
    History h(2);
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);

    ASSERT_DOUBLE_EQ(3, h(0));
    ASSERT_DOUBLE_EQ(2, h(1));
    ASSERT_DOUBLE_EQ(1, h(2));
}

TEST_F(HistoryTest, cannot_retrieve_value_in_the_future)
{
    History h;
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);
    ASSERT_THROW(h(-1), InternalErrorException);
}

TEST_F(HistoryTest, linear_interpolation_should_be_accurate)
{
    //! [HistoryTest example]
    History h(1e3);
    h.record(24, 1);
    h.record(216, 277);
    h.record(420, 73);
    h.record(421, 1);
    h.record(540, 239);

    //! [HistoryTest example]
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(1,  h(516));
    ASSERT_DOUBLE_EQ(73, h(120));
    ASSERT_DOUBLE_EQ(1,  h(119));
    ASSERT_DOUBLE_EQ(239,h(0));
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(277,h(324));
    ASSERT_DOUBLE_EQ(24, h(500));
    ASSERT_DOUBLE_EQ(193,h(240));
    ASSERT_DOUBLE_EQ(37, h(119.5));
    ASSERT_DOUBLE_EQ(3,  h(118));
}

TEST_F(HistoryTest, can_get_size_of_history)
{
    const size_t N = a.random<size_t>().between(2,1000);
    History h(1e10);
    for (size_t i = 0 ; i < N ; ++i) h.record((double)i, a.random<double>());
    ASSERT_EQ(N, h.size());
}

TEST_F(HistoryTest, should_shift_history)
{
    History h(1E10);
    h.record(10, 10);
    ASSERT_EQ(1, h.size());

    h.record(15, 20);
    ASSERT_EQ(2, h.size());

    h.record(20, 12);
    ASSERT_EQ(3, h.size());

    h.record(21, 14);
    ASSERT_EQ(4, h.size());

    h.record(22, 13);
    ASSERT_EQ(5, h.size());

    h.record(24, 16);
    ASSERT_EQ(6, h.size());
    ASSERT_DOUBLE_EQ(11, h(13.5));

    h.record(30, 25);
    ASSERT_EQ(7, h.size());

    h.record(31, 26);
    ASSERT_EQ(8, h.size());
    ASSERT_DOUBLE_EQ(16, h(13.5));

    h.record(33, 27);
    ASSERT_EQ(9, h.size());
    ASSERT_DOUBLE_EQ(12.8, h(13.5));
}

TEST_F(HistoryTest, can_get_history_max_length)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        //! [HistoryTest get_Tmax_example]
        const double Tmax = a.random<double>().greater_than(0);
        History h(Tmax);
        const double t = a.random<double>().between(0,10);
        h.record(t, a.random<double>());
        h.record(t+Tmax, a.random<double>());
        ASSERT_DOUBLE_EQ(Tmax, h.get_Tmax());
        //! [HistoryTest get_Tmax_example]
    }
}

TEST_F(HistoryTest, can_get_history_length)
{
    //! [HistoryTest get_length_example]
    History h(1e10);
    ASSERT_DOUBLE_EQ(0, h.get_duration());
    h.record(24, 1);
    ASSERT_DOUBLE_EQ(0, h.get_duration());
    h.record(216, 277);
    ASSERT_DOUBLE_EQ(216-24, h.get_duration());
    h.record(420, 73);
    ASSERT_DOUBLE_EQ(420-24, h.get_duration());
    h.record(421, 1);
    ASSERT_DOUBLE_EQ(421-24, h.get_duration());
    h.record(540, 239);
    ASSERT_DOUBLE_EQ(540-24, h.get_duration());
    h.record(2400, 1);
    ASSERT_DOUBLE_EQ(2400-24, h.get_duration());
    //! [HistoryTest get_length_example]
}

TEST_F(HistoryTest, history_should_not_grow_indefinitely)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    h.record(t-9, 3);
    h.record(t-8, 2);
    ASSERT_DOUBLE_EQ(1, h(4));
    ASSERT_EQ(3, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    ASSERT_DOUBLE_EQ(0, h(4.1));
    h.record(t-5, 4);
    ASSERT_EQ(3, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    ASSERT_DOUBLE_EQ(3, h(4));
    h.record(t-4, 2);
    h.record(t-2, 7);
    h.record(t-1, 6);
    ASSERT_EQ(4, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    h.record(t, 8);
    ASSERT_DOUBLE_EQ(2, h(4));
    ASSERT_EQ(4, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
}

TEST_F(HistoryTest, interpolation_should_be_OK_after_shift)
{
    History h(3);
    h.record(-10, 13);
    h.record(0, 23);
    ASSERT_DOUBLE_EQ(20., h(3));
}

TEST_F(HistoryTest, can_calculate_average_value_for_one_point)
{
    History h(2);
    h.record(0,1);
    ASSERT_DOUBLE_EQ(1, h.average(1));
}

TEST_F(HistoryTest, should_throw_if_length_of_average_is_negative)
{
    History h(2);
    h.record(0,1);
    h.record(2,1);
    ASSERT_THROW(h.average(-1), InternalErrorException);
}

TEST_F(HistoryTest, can_retrieve_average_for_two_values)
{
    History h(2);
    h.record(0,0);
    h.record(2,2);
    ASSERT_NEAR(1, h.average(2), 1E-3);
}

TEST_F(HistoryTest, full_average_test)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    h.record(t-9, 3);
    h.record(t-8, 2);
    h.record(t-5, 4);
    ASSERT_DOUBLE_EQ(11.5/4., h.average(4));

    h.record(t-4, 2);
    h.record(t-4+4./5., 4);
    ASSERT_DOUBLE_EQ(3, h.average(1+4./5.));
    h.record(t-2, 7);
    h.record(t-1, 6);
    h.record(t, 8);
    ASSERT_DOUBLE_EQ(7, h.average(1));
    ASSERT_DOUBLE_EQ(7.5, h.average(0.5));
}

TEST_F(HistoryTest, average_is_instant_value_when_length_is_zero)
{
    History h(2);
    h.record(0,1);
    h.record(2,2);
    ASSERT_DOUBLE_EQ(2, h.average(0));
}

TEST_F(HistoryTest, can_ask_for_average_even_if_history_is_not_yet_long_enough)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    ASSERT_DOUBLE_EQ(1, h.average(4));
    h.record(t-9, 3);
    ASSERT_DOUBLE_EQ(2, h.average(4));
    h.record(t-8, 2);
    ASSERT_DOUBLE_EQ(2.125, h.average(4));
}

TEST_F(HistoryTest, average_value_of_an_empty_history_is_simply_zero)
{
    ASSERT_DOUBLE_EQ(0, History().average(1));
}

TEST_F(HistoryTest, inserting_same_state_twice_should_not_change_history_length)
{
    History h(4);
    ASSERT_EQ(0, h.size());
    const double t = a.random<double>().between(0,1E5);
    h.record(t,a.random<double>());
    ASSERT_EQ(1, h.size());
    ASSERT_NO_THROW(h.record(t,a.random<double>()));
    h.record(t,a.random<double>());
    ASSERT_EQ(1, h.size());
}

TEST_F(HistoryTest, if_we_insert_states_out_of_order_we_should_throw)
{
    History h(4);
    ASSERT_EQ(0, h.size());
    const double t1 = 2.6000000000000005329;
    const double t2 = 2.6000000000000000888;
    const double t3 = 2.599999;
    h.record(t1,a.random<double>());
    // t1 and t2 are too close: don't throw
    ASSERT_NO_THROW(h.record(t2,a.random<double>()));
    ASSERT_EQ(1, h.size());
    // t2 and t3 are too far and t3 < t2: throw
    ASSERT_THROW(h.record(t3,a.random<double>()), InternalErrorException);
}

TEST_F(HistoryTest, if_two_time_instants_are_too_close_we_overwrite)
{
    History h(4);
    ASSERT_EQ(0, h.size());
    const double t1 = 2.6000000000000005329;
    const double t2 = 2.6000000000000000888;
    h.record(t2,a.random<double>());
    ASSERT_NO_THROW(h.record(t1,a.random<double>()));
    ASSERT_EQ(1, h.size());
}

TEST_F(HistoryTest, bug_3207_absurd_history_value)
{
    const std::vector<std::pair<double,double> > L = {{0,0}, {0.10000000000000000555,-9.7036909645190687675e-07}, {0.2000000000000000111,-1.9057597889340088962e-06}, {0.30000000000000004441,-2.7771321247778431822e-06}, {0.4000000000000000222,-3.6193245822112549248e-06}, {0.5,-4.3253812362626626212e-06}, {0.60000000000000008882,-5.0063768759283871929e-06}, {0.70000000000000006661,-5.4786836801752297716e-06}, {0.80000000000000004441,-5.9343831831023901606e-06}, {0.9000000000000000222,-6.1127936485714609265e-06}, {1,-6.286408822723359033e-06}, {1.1000000000000000888,-6.1402151460835824404e-06}, {1.1999999999999999556,-6.0015620117862598199e-06}, {1.3000000000000002665,-5.5392965788533718555e-06}, {1.4000000000000001332,-5.0943842359965820909e-06}, {1.500000000000000222,-4.3588816944881406172e-06}, {1.6000000000000000888,-3.6463759719806385388e-06}, {1.7000000000000001776,-2.7023872466189729883e-06}, {1.8000000000000000444,-1.7830843651293733319e-06}, {1.9000000000000001332,-7.0666700564374377178e-07}, {2,3.4609726780418795069e-07}, {2.1000000000000000888,1.4732860791819591381e-06}, {2.2000000000000001776,2.579412023222083059e-06}, {2.3000000000000002665,3.6738765270614193857e-06}, {2.4000000000000003553,4.7507483617327382229e-06}, {2.5000000000000004441,5.7321395816006726114e-06}, {2.6000000000000005329,6.6998959125700857851e-06}, {2.7000000000000001776,7.4969916808873473339e-06}, {2.8000000000000002665,8.2844913556231495683e-06}, {2.9000000000000003553,8.8397540572040148141e-06}, {3.0000000000000004441,9.3891837539355119639e-06}, {3.1000000000000000888,9.660896294038144873e-06}, {3.2000000000000001776,9.9301764224039885948e-06}, {3.3000000000000002665,9.8937075606662533851e-06}, {3.4000000000000003553,9.8580219723901018437e-06}, {3.5000000000000004441,9.5076140717586172569e-06}, {3.6000000000000005329,9.1612821901163982537e-06}, {3.7000000000000001776,8.512363617013259947e-06}, {3.8000000000000002665,7.8708073942084493267e-06}, {3.9000000000000003553,6.9605008314691713855e-06}, {4,6.0606751212520161389e-06}, {4.0999999999999996447,4.9459313405872921157e-06}, {4.2000000000000001776,3.8443574057041430997e-06}, {4.2999999999999998224,2.598284045140424304e-06}, {4.4000000000000003553,1.3674298882430980592e-06}, {4.5,7.4658235247628505223e-08}, {4.6000000000000005329,-1.2016006144703872174e-06}, {4.7000000000000001776,-2.4500511472515844327e-06}, {4.8000000000000007105,-3.6816961284442664278e-06}, {4.9000000000000003553,-4.7948147256080097519e-06}, {5.0000000000000008882,-5.8920041746351397999e-06}, {5.0999999999999996447,-6.7860842799640416146e-06}, {5.2000000000000001776,-7.6664317533160104959e-06}, {5.2999999999999998224,-8.2726519082780597847e-06}, {5.4000000000000003553,-8.868441368511533157e-06}, {5.5,-9.1387862770605922353e-06}, {5.6000000000000005329,-9.4027545725835268815e-06}, {5.7000000000000001776,-9.313681572315640834e-06}, {5.8000000000000007105,-9.222856402578517851e-06}, {5.9000000000000003553,-8.7777562919598975448e-06}, {6.0000000000000008882,-8.3357095162579790046e-06}, {6.0999999999999996447,-7.5651559756541585404e-06}, {6.2000000000000001776,-6.8022934264937335458e-06}, {6.2999999999999998224,-5.7623812707807391871e-06}, {6.4000000000000003553,-4.7342377173929172464e-06}, {6.5,-3.5023181473152497668e-06}, {6.6000000000000005329,-2.2853379801019167838e-06}, {6.7000000000000001776,-9.5388684464636534983e-07}, {6.8000000000000007105,3.6091336714574689964e-07}, {6.9000000000000003553,1.6924672298920901443e-06}, {7.0000000000000008882,3.0066599591631339546e-06}, {7.0999999999999996447,4.2390314303157199577e-06}, {7.2000000000000001776,5.4543757805354862352e-06}, {7.2999999999999998224,6.4953057697914696545e-06}, {7.4000000000000003553,7.5211222633312498642e-06}, {7.5,8.2936728780926679205e-06}, {7.6000000000000005329,9.0538979793550535701e-06}, {7.7000000000000001776,9.5017700616109207647e-06}, {7.8000000000000007105,9.9409612621115725213e-06}, {7.9000000000000003553,1.0033004810046121572e-05}, {8,1.0120540609008757635e-05}, {8.0999999999999996447,9.8527093843762171701e-06}, {8.1999999999999992895,9.5847176542597881685e-06}, {8.3000000000000007105,8.9798369234421734149e-06}, {8.4000000000000003553,8.3791275223156667863e-06}, {8.5,7.4851224465715693877e-06}, {8.5999999999999996447,6.5992333022255387812e-06}, {8.6999999999999992895,5.4853152178736918867e-06}, {8.7999999999999989342,4.3828559083247767167e-06}, {8.9000000000000003553,3.1342599688149827546e-06}, {9,1.8996001824548249437e-06}, {9.0999999999999996447,6.1079497738481726573e-07}, {9.1999999999999992895,-6.6262824236708605382e-07}, {9.3000000000000007105,-1.8953766960769136105e-06}, {9.4000000000000003553,-3.1124494006528930966e-06}, {9.5,-4.1987919400619540239e-06}, {9.5999999999999996447,-5.2702746816307330483e-06}, {9.7000000000000010658,-6.1320806193610403995e-06}, {9.8000000000000007105,-6.9809570379847974122e-06}, {9.9000000000000003553,-7.5589716668752572409e-06}, {10,-8.1267643029808643134e-06}};
    History h(L);
    ASSERT_EQ(101, h.size());
    ASSERT_DOUBLE_EQ(0, h(10));
    const double t = 10.099999999999999645;
    const double val = -8.544370340276039006e-06;
    h.record(t, val);
    ASSERT_EQ(101, h.size());
    ASSERT_DOUBLE_EQ(-9.7036909645190687675e-07, h(10));
}

TEST_F(HistoryTest, can_get_list_of_instants)
{
    for (size_t k = 0 ; k < 100 ; ++k)
    {
        const size_t n = a.random<size_t>().between(1,10);
        const double eps = 1E-15;
        const std::vector<double> dt = a.random_vector_of<double>().of_size(n).between(eps,1);
        const std::vector<double> x = a.random_vector_of<double>().of_size(n);
        std::vector<double> t(n);
        std::partial_sum(dt.begin(), dt.end(), t.begin());
        std::vector<std::pair<double,double> > L(n);
        for (size_t i = 0 ; i < n ; ++i)
        {
            L[i] = {t[i],x[i]};
        }
        const History h(L);
        const double tend = t.back();

        for (size_t i = 0 ; i < n ; ++i)
        {
            const double Dt = tend - t[n-i-1] + eps;
            const std::vector<double> t_out = h.get_dates(Dt);
            ASSERT_EQ(i+1, t_out.size()) << "i = " << i;
            for (size_t j = 0 ; j <= i ; j++)
            {
                ASSERT_DOUBLE_EQ(t_out.at(j), t.at(n-1-i+j)) << "i = " << i << ", j = " << j;
            }
        }
    }
}

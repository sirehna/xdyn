/*
 * History.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#ifndef HISTORY_HPP_
#define HISTORY_HPP_

#include <cstdlib> //size_t
#include <sstream>
#include <vector>

/** \brief
 *  \details
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/HistoryTest.cpp HistoryTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/HistoryTest.cpp HistoryTest expected output
 */
class History
{
    public:
        History(const double Tmax=0);

        /**  \brief Returns the value at t-tau, t being the current instant
          *  \returns Value at t-tau in history
          *  \snippet hdb_interpolator/unit_tests/src/HistoryTest.cpp HistoryTest get_example
          */
        double operator()(double tau = 0//!< How far back in history do we need to go (in seconds)?
                         ) const;
        //double operator()() const;

        /**  \brief Adds a value to history
          *  \details
          *  \snippet hdb_interpolators/unit_tests/src/HistoryTest.cpp HistoryTest record_example
          */
        void record(const double t, //!< Instant corresponding to the value being added
                    const double val //!< Value to add
                    );

        /**  \brief Number of points in history
          *  \snippet hdb_interpolator/unit_tests/src/HistoryTest.cpp HistoryTest size_example
          */
        size_t size() const;

        /**  \brief Accessor for Tmax
         *   \returns Tmax (in seconds)
          *  \snippet hdb_interpolator/unit_tests/src/HistoryTest.cpp HistoryTest get_Tmax_example
          */
        double get_Tmax() const;

        /**  \brief How far back in history can we currently go
         *   \details Maximum value is Tmax, but during initialization it can be less.
         *   \returns Difference between last & first instants in L.
          *  \snippet hdb_interpolator/unit_tests/src/HistoryTest.cpp HistoryTest get_length_example
          */
        double get_duration() const;

        friend std::ostream& operator<<(std::ostream& os, const History& h);

    private:
        typedef std::pair<double,double> TimeValue;
        typedef std::vector<TimeValue> Container;

        void throw_if_already_added(const size_t idx, const double t, const double val) const;
        size_t find_braketing_position(const double t) const;
        double interpolate_value_in_interval(const size_t idx, const double t) const;
        double get_value(const double tau) const;
        double get_current_time() const;
        void shift_oldest_recorded_instant_if_necessary();
        void add_value_to_history(const double t, const double val);
        void update_oldest_recorded_instant(const double t);

        double Tmax;
        Container L;
        double oldest_recorded_instant;
};

std::ostream& operator<<(std::ostream& os, const History& h);

#endif /* HISTORY_HPP_ */

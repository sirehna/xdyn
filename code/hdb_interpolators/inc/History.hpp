/*
 * History.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#ifndef HISTORY_HPP_
#define HISTORY_HPP_

#include <cstdlib> //size_t
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
        History(const double Tmax //!< Maximum duration to store in history (in seconds)
               );

        /**  \brief Returns the value at t-tau, t being the current instant
          *  \returns Value at t-tau in history
          *  \snippet hdb_interpolator/unit_tests/src/HistoryTest.cpp HistoryTest get_example
          */
        double get(double tau //!< How far back in history do we need to go (in seconds)?
                   ) const;

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

    private:
        typedef std::pair<double,double> TimeValue;
        typedef std::vector<TimeValue> Container;

        History(); // Disabled
        void throw_if_already_added(const size_t idx, const double t) const;
        size_t find_braketing_position(const double t) const;
        double get_value(const size_t idx, const double t) const;

        double Tmax;
        Container L;
};


#endif /* HISTORY_HPP_ */

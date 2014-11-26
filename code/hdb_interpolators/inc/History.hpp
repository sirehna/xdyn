/*
 * History.hpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#ifndef HISTORY_HPP_
#define HISTORY_HPP_

#include <list>

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

    private:
        typedef std::pair<double,double> TimeValue;

        History(); // Disabled
        void throw_if_already_added(const std::list<TimeValue>::const_iterator& it, const double t) const;

        double Tmax;
        std::list<TimeValue> L;
};


#endif /* HISTORY_HPP_ */

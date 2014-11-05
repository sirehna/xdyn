/*
 * SimCsvObserver.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef SIMCSVOBSERVER_HPP_
#define SIMCSVOBSERVER_HPP_

#include <ssc/kinematics.hpp>

#include <ostream>
#include <string>
#include <vector>


class Sim;
extern std::ostream dev_null_buffer; // Litter goes there

/** \author cec
 *  \date Jun 19, 2014, 5:29:09 PM
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/SimCsvObserverTest.cpp SimCsvObserverTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/SimCsvObserverTest.cpp SimCsvObserverTest expected output
 */
class SimCsvObserver
{
    public:
        SimCsvObserver(std::ostream& simulation_stream, //!< All simulation results will be outputted to this stream
                       std::ostream& wave_stream //!< Wave heights calculated on output mesh will be outputted to this stream
                      );
        SimCsvObserver(std::ostream& simulation_stream //!< All simulation results will be outputted to this stream
                      );
        void observe(const Sim& sys, const double t);

    private:
        SimCsvObserver(); // Disabled

        void observe_states(const Sim& sys, const double t);
        void observe_forces(const Sim& sys);
        void observe_waves(const Sim& sys, const double t);
        void initialize_simulation_output_stream(const Sim& sys);
        void initialize_title();
        void initialize_wave_output_stream(const std::vector<ssc::kinematics::Point>& free_surface);

        std::ostream& simulation_stream;
        std::ostream& wave_stream;
        bool initialized;
        std::vector<std::string> bodies;
        std::vector<std::string> forces;
        std::string customize(const std::string& body, const std::string anything) const;
};

#endif /* SIMCSVOBSERVER_HPP_ */

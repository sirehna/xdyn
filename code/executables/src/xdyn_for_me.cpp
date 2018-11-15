/*
 * xdyn_for_me.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#include <ssc/text_file_reader.hpp>
#include <ssc/websocket.hpp>

#include "display_command_line_arguments.hpp"
#include "ConfBuilder.hpp"
#include "ConnexionError.hpp"
#include "HistoryParser.hpp"
#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "parse_XdynForMECommandLineArguments.hpp"
#include "report_xdyn_exceptions_to_user.hpp"
#include "SimStepper.hpp"
#include "XdynForME.hpp"
#include "XdynForMECommandLineArguments.hpp"

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS

// For handling Ctrl+C
#include <unistd.h>
#include <cstdio>
#include <csignal>

volatile sig_atomic_t stop;

void inthand(int);
void inthand(int)
{
    stop = 1;
}


struct SimulationMessage : public ssc::websocket::MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<XdynForME>& xdyn_for_me_) : xdyn_for_me(xdyn_for_me_)
    {
    }
    void operator()(const ssc::websocket::Message& msg)
    {
        COUT(msg.get_payload());
        const std::string input_yaml = msg.get_payload();
        try
        {
            SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, xdyn_for_me->get_Tmax());
            const std::vector<double> dx_dt = xdyn_for_me->calculate_dx_dt(server_inputs);
            std::stringstream ss;

            ss << "{"
               << "\"dx_dt\": "  << dx_dt[0] << ","
               << "\"dy_dt\": "  << dx_dt[1] << ","
               << "\"dz_dt\": "  << dx_dt[2] << ","
               << "\"du_dt\": "  << dx_dt[3] << ","
               << "\"dv_dt\": "  << dx_dt[4] << ","
               << "\"dw_dt\": "  << dx_dt[5] << ","
               << "\"dp_dt\": "  << dx_dt[6] << ","
               << "\"dq_dt\": "  << dx_dt[7] << ","
               << "\"dr_dt\": "  << dx_dt[8] << ","
               << "\"dqr_dt\": " << dx_dt[9] << ","
               << "\"dqi_dt\": " << dx_dt[10] << ","
               << "\"dqj_dt\": " << dx_dt[11] << ","
               << "\"dqk_dt\": " << dx_dt[12]
               << "}";

            const std::string output_yaml = ss.str();;
            msg.send_text(output_yaml);
        }
        catch(const std::exception& e)
        {
            msg.send_text(std::string("{\"error\": \"") + e.what() + "\"}");
        }

    }

    private: TR1(shared_ptr)<XdynForME> xdyn_for_me;
};

void start_server(const XdynForMECommandLineArguments& input_data);
void start_server(const XdynForMECommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<XdynForME> sim_server (new XdynForME(yaml));
    SimulationMessage handler(sim_server);

    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, input_data.port));
    std::cout << "Starting websocket server on " << ADDRESS << ":" << input_data.port << " (press Ctrl+C to terminate)" << std::endl;
    signal(SIGINT, inthand);
    while(!stop){}
    std::cout << "Gracefully stopping the websocket server..." << std::endl;
}

int main(int argc, char** argv)
{
    XdynForMECommandLineArguments input_data;
    int error = 0;
    COUT("");
    try
    {
        COUT("");
        if (argc==1) return fill_input_or_display_help(argv[0], input_data);
        error = get_input_data(argc, argv, input_data);
    }
    catch(boost::program_options::error& e)
    {
      std::cerr << "The command line you supplied is not valid: " << e.what() << std::endl << "Use --help to get the list of available parameters." << std::endl;
      return -1;
    }
    if (error)
    {
        return error;
    }
    COUT("");
    const auto run = [input_data](){
    {
        start_server(input_data);
    }};
    COUT("");
    if (input_data.catch_exceptions)
    {
        COUT("");
        report_xdyn_exceptions_to_user(run, "");
    }
    else
    {
        COUT("");
        run();
    }
    COUT("");
    return 0;
}


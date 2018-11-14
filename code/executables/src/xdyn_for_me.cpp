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



struct SimulationMessage : public ssc::websocket::MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<XdynForME>& xdyn_for_me_) : xdyn_for_me(xdyn_for_me_)
    {
    }
    void operator()(const ssc::websocket::Message& msg)
    {
        COUT(msg.get_payload());
        const std::string input_yaml = msg.get_payload();
        const std::string output_yaml = xdyn_for_me->calculate_dx_dt(input_yaml);
        msg.send_text(output_yaml);
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

    new ssc::websocket::Server(handler, ADDRESS, input_data.port);
}



int main(int argc, char** argv)
{
    XdynForMECommandLineArguments input_data;
    int error = 0;
    try
    {
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
        std::cerr <<"A problem occurred while parsing inputs" << std::endl;
        return error;
    }
    const auto run = [input_data](){
    {
        start_server(input_data);
    }};
    if (input_data.catch_exceptions)
    {

        report_xdyn_exceptions_to_user(run, "");
    }
    else
    {
        run();
    }
    return 0;
}


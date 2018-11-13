/*
 * xdyn_for_me.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#include <ssc/text_file_reader.hpp>
#include <ssc/websocket.hpp>

#include "ConfBuilder.hpp"
#include "HistoryParser.hpp"
#include "InputDataForME.hpp"
#include "ConnexionError.hpp"
#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "SimStepper.hpp"
#include "utilities_for_InputData.hpp"
#include "XdynForME.hpp"

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

void start_server(const InputDataForME& input_data);
void start_server(const InputDataForME& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<XdynForME> sim_server (new XdynForME(yaml));
    SimulationMessage handler(sim_server);

    new ssc::websocket::Server(handler, ADDRESS, input_data.port);
}



int main(int argc, char** argv)
{
    InputDataForME input_data;
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
    if (input_data.catch_exceptions)
    {
        try
        {
            start_server(input_data);
        }
        catch(const InternalErrorException& e)
        {
            std::cerr << "The following error should never arise & is clearly a sign of a bug in the simulator. Please contact the support team." << std::endl
                      << e.what() << std::endl;
            return -1;
        }
        catch(const MeshException& e)
        {
            std::cerr << "A problem was detected with the STL file (mesh): " << e.get_message() << std::endl;
            return -1;
        }
        catch(const ConnexionError& e)
        {
            std::cerr << "This simulation requires X-DYN to connect to a server but there was a problem with that connection: " << e.get_message() << std::endl;
            return -1;
        }
        catch(ssc::exception_handling::Exception& e)
        {
            std::cerr << "The following problem was detected:" << std::endl << e.get_message() << std::endl;
            return -1;
        }
        catch(const YAML::Exception& e)
        {
            std::cerr << "There is a syntax problem with the YAML file line " << e.mark.line+1 << ", column " << e.mark.column+1 << ": " << e.msg << "." << std::endl
                      << "Please note that as all YAML files supplied on the command-line are concatenated, the line number given here corresponds to the line number in the concatenated YAML." << std::endl;
            return -1;
        }
        catch(std::exception& e)
        {
            std::cerr << "Something bad has happened: please send an email to the support team containing the following:" << std::endl
                      << "- Input YAML file(s) + STL (if needed)" << std::endl
                      << "- Command-line arguments" << std::endl
                      << "- The following error message: " << e.what() << std::endl;
            return -1;
        }
    }
    start_server(input_data);
    return 0;
}


#include "report_xdyn_exceptions_to_user.hpp"
#include "utilities_for_InputDataSimServer.hpp"
#include "ssc/websocket/WebSocketServer.hpp"
#include <ssc/text_file_reader.hpp>
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <sstream>

#include "../inc/XdynCommandLineArguments.hpp"
#include "XdynForCS.hpp"

using namespace ssc::websocket;

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234


struct SimulationMessage : public MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<SimServer>& simserver) : sim_server(simserver)
    {
    }
    void operator()(const Message& msg)
    {
        COUT(msg.get_payload());
        const std::string input_yaml = msg.get_payload();
        const std::string output_yaml = sim_server->play_one_step(input_yaml);
        msg.send_text(output_yaml);
    }

    private: TR1(shared_ptr)<SimServer> sim_server;
};

void start_server(const InputDataSimServer& input_data);
void start_server(const InputDataSimServer& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<SimServer> sim_server (new SimServer(yaml, input_data.solver, input_data.initial_timestep));
    SimulationMessage handler(sim_server);

    new ssc::websocket::Server(handler, ADDRESS, input_data.port);
}

int main(int argc, char** argv)
{
    InputDataSimServer input_data;
    if (argc==1) return display_help(argv[0], input_data);
    const int error = get_input_data(argc, argv, input_data);
    if (error)
    {
        std::cerr <<"A problem occurred while parsing inputs"<<std::endl;
        return error;
    }
    if (input_data.empty()) return EXIT_SUCCESS;
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
    return error;
}

#include "report_xdyn_exceptions_to_user.hpp"
#include "ssc/websocket/WebSocketServer.hpp"
#include <ssc/text_file_reader.hpp>
#include <ssc/macros.hpp>

#include TR1INC(memory)
#include <sstream>

#include "HistoryParser.hpp"
#include "parse_XdynForCSCommandLineArguments.hpp"
#include "XdynForCS.hpp"

using namespace ssc::websocket;

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234


#include <boost/algorithm/string/replace.hpp>
std::string escape_newlines(std::string str);
std::string escape_newlines(std::string str)
{
    boost::replace_all(str, "\n", "\\n");
    return str;
}

struct SimulationMessage : public MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<SimServer>& simserver) : sim_server(simserver)
    {
    }
    void operator()(const Message& msg)
    {
        COUT(msg.get_payload());
        const std::string input_yaml = msg.get_payload();
        const auto outputter = [&msg](const std::string& what) {msg.send_text(escape_newlines(std::string("{\"error\": \"") + what + "\"}"));};
        const auto f = [&msg, this, &input_yaml]() {msg.send_text(emit_state_history_json(this->sim_server->play_one_step(input_yaml)));};
        report_xdyn_exceptions_to_user(f, outputter);
    }

    private:
        TR1(shared_ptr)<SimServer> sim_server;
};


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

void start_server(const XdynForCSCommandLineArguments& input_data);
void start_server(const XdynForCSCommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<SimServer> sim_server (new SimServer(yaml, input_data.solver, input_data.initial_timestep));
    SimulationMessage handler(sim_server);
    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, input_data.port));
    std::cout << "Starting websocket server on " << ADDRESS << ":" << input_data.port << " (press Ctrl+C to terminate)" << std::endl;
    signal(SIGINT, inthand);
    while(!stop){}
    std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
}

int main(int argc, char** argv)
{
    XdynForCSCommandLineArguments input_data;
    if (argc==1) return display_help(argv[0], input_data);
    const int error = get_input_data(argc, argv, input_data);
    if (error)
    {
        return error;
    }
    if (input_data.empty()) return EXIT_SUCCESS;
    const auto run = [input_data](){
    {
        start_server(input_data);
    }};
    if (input_data.catch_exceptions)
    {

        report_xdyn_exceptions_to_user(run, [](const std::string& s){std::cerr << s;});
    }
    else
    {
        run();
    }
    return error;
}

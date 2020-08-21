#include "XdynForCS.hpp"
#include "parse_history.hpp"
#include "report_xdyn_exceptions_to_user.hpp"
#include "parse_XdynForCSCommandLineArguments.hpp"

#include <ssc/websocket/WebSocketServer.hpp>
#include <ssc/text_file_reader.hpp>
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <google/protobuf/stubs/common.h>
#include <sstream>

using namespace ssc::websocket;

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234

#include <ssc/check_ssc_version.hpp>
CHECK_SSC_VERSION(8,0)

#include <boost/algorithm/string/replace.hpp>
std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

struct SimulationMessage : public MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<SimServer>& simserver, const bool verbose_) : sim_server(simserver), verbose(verbose_)
    {
    }
    void operator()(const Message& msg)
    {
        const std::string input_json = msg.get_payload();
        if (verbose)
        {
            std::cout << current_date_time() << " Received: " << input_json << std::endl;
        }
        const std::function<void(const std::string&)> quiet_error_outputter = [&msg](const std::string& what) {msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
        const std::function<void(const std::string&)> verbose_error_outputter = [&msg](const std::string& what) {std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
        const auto error_outputter = verbose ? verbose_error_outputter : quiet_error_outputter;
        const std::function<void(void)> quiet_f = [&msg, this, &input_json]() {msg.send_text(encode_YamlStates(this->sim_server->play_one_step(input_json)));};
        const std::function<void(void)> verbose_f = [&msg, this, &input_json]() {const std::string json = encode_YamlStates(this->sim_server->play_one_step(input_json)); std::cout << current_date_time() << " Sending: " << json << std::endl; msg.send_text(json);};
        const std::function<void(void)> f = verbose ? verbose_f : quiet_f;
        report_xdyn_exceptions_to_user(f, error_outputter);
    }

    private:
        TR1(shared_ptr)<SimServer> sim_server;
        const bool verbose;
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
    SimulationMessage handler(sim_server, input_data.verbose);
    std::cout << "Starting websocket server on " << ADDRESS << ":" << input_data.port << " (press Ctrl+C to terminate)" << std::endl;
    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, input_data.port, input_data.show_websocket_debug_information));
    signal(SIGINT, inthand);
    while(!stop){}
    std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
}

int main(int argc, char** argv)
{
    XdynForCSCommandLineArguments input_data;
    if (argc==1) return display_help(argv[0], input_data);
    int error = 0;
    report_xdyn_exceptions_to_user([&error,&argc,&argv,&input_data]{error = get_input_data(argc, argv, input_data);}, [](const std::string& s){std::cerr << s;});
    if (error)
    {
        return error;
    }
    if (input_data.empty() || input_data.show_help) return EXIT_SUCCESS;
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
    google::protobuf::ShutdownProtobufLibrary();
    return error;
}

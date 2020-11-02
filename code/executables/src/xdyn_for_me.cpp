/*
 * xdyn_for_me.cpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#include <iomanip> // std::setprecision

#include "XdynForME.hpp"

#include "display_command_line_arguments.hpp"
#include "HistoryParser.hpp"
#include "parse_XdynForMECommandLineArguments.hpp"
#include "ModelExchangeServiceImpl.hpp"
#include "report_xdyn_exceptions_to_user.hpp"
#include "XdynForMECommandLineArguments.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/websocket.hpp>

#include <google/protobuf/stubs/common.h>

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS

#include <ssc/check_ssc_version.hpp>
CHECK_SSC_VERSION(8,0)

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

#include <boost/algorithm/string/replace.hpp>
std::string replace_newlines_by_spaces(std::string str);
std::string replace_newlines_by_spaces(std::string str)
{
    boost::replace_all(str, "\n", " ");
    return str;
}

struct SimulationMessage : public ssc::websocket::MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<XdynForME>& xdyn_for_me_, const bool verbose_) : xdyn_for_me(xdyn_for_me_), verbose(verbose_)
    {
    }
    void operator()(const ssc::websocket::Message& msg)
    {
        const std::string input_json = msg.get_payload();
        if (verbose)
        {
            std::cout << current_date_time() << " Received: " << input_json << std::endl;
        }
        const std::function<void(const std::string&)> quiet_error_outputter = [&msg](const std::string& what) {msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
        const std::function<void(const std::string&)> verbose_error_outputter = [&msg](const std::string& what) {std::cerr << current_date_time() << " Error: " << what << std::endl; msg.send_text(replace_newlines_by_spaces(std::string("{\"error\": \"") + what + "\"}"));};
        const auto error_outputter = verbose ? verbose_error_outputter : quiet_error_outputter;
        const std::string input_yaml = msg.get_payload();
        const auto f =
                [&input_yaml, this, &msg]()
        {
            SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, xdyn_for_me->get_Tmax());
            const std::vector<double> dx_dt = xdyn_for_me->calculate_dx_dt(server_inputs);
            std::stringstream ss;
            // Set precision to shortest possible representation, without losing precision
            // Cf. https://stackoverflow.com/a/23437425, and, more specifically answer https://stackoverflow.com/a/4462034
            ss << std::defaultfloat << std::setprecision(17);
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
            const std::string output_json = ss.str();
            if (verbose)
            {
                std::cout << current_date_time() << " Sending: " << output_json << std::endl;
            }
            msg.send_text(output_json);
        };
        report_xdyn_exceptions_to_user(f, error_outputter);
    }

    private:
        TR1(shared_ptr)<XdynForME> xdyn_for_me;
        const bool verbose;
};

void start_ws_server(const XdynForMECommandLineArguments& input_data);
void start_ws_server(const XdynForMECommandLineArguments& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<XdynForME> sim_server (new XdynForME(yaml));
    SimulationMessage handler(sim_server, input_data.verbose);
    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, input_data.port, input_data.show_websocket_debug_information));
    std::cout << "Starting websocket server on " << ADDRESS << ":" << input_data.port << " (press Ctrl+C to terminate)" << std::endl;
    signal(SIGINT, inthand);
    while(!stop){}
    std::cout << std::endl << "Gracefully stopping the websocket server..." << std::endl;
}

void start_grpc_server(const XdynForMECommandLineArguments& input_data);
void start_grpc_server(const XdynForMECommandLineArguments& input_data)
{
    std::stringstream ss;
    ss << "0.0.0.0:" << input_data.port;
    const std::string server_address = ss.str();
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    XdynForME xdyn(yaml);
    ModelExchangeServiceImpl service(xdyn);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "gRPC server listening on " << server_address << std::endl;
    server->Wait();
    std::cout << std::endl << "Gracefully stopping the gRPC server..." << std::endl;
}

int main(int argc, char** argv)
{
    XdynForMECommandLineArguments input_data;
    if (argc==1) return fill_input_or_display_help(argv[0], input_data);
    int error = 0;
    report_xdyn_exceptions_to_user([&error,&argc,&argv,&input_data]{error = get_input_data(argc, argv, input_data);}, [](const std::string& s){std::cerr << s;});
    if (error)
    {
        return error;
    }
    if (input_data.show_help)
    {
        return 0;
    }
    const auto run_ws = [input_data](){
    {
        start_ws_server(input_data);
    }};
    const std::function< void(void) > run_grpc = [input_data](){
    {
        start_grpc_server(input_data);
    }};
    const std::function< void(void) > run = input_data.grpc ? run_grpc : run_ws;
    if (input_data.catch_exceptions)
    {
        report_xdyn_exceptions_to_user(run, [](const std::string& s){std::cerr << s;});
    }
    else
    {
        run();
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}


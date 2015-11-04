#include "ssc/websocket/WebSocketServer.hpp"
#include "SimServer.hpp"
#include <sstream>
#include <ssc/macros.hpp>
#include TR1INC(memory)

using namespace ssc::websocket;

struct SimulationMessage : public MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<SimServer>& simserver) : sim_server(simserver)
    {
    }
    void operator()(const Message& msg)
    {

        const std::string input_yaml = msg.get_payload();
        const std::string output_yaml = sim_server->play_one_step(input_yaml);
        msg.send_text(output_yaml);
    }
    private: TR1(shared_ptr)<SimServer> sim_server;
};

int main(int argc, char** argv)
{
    if(argc!=6)
    {
        std::cerr <<"Program expects "<<std::endl;
    }
    else
    {
        double dt=0;
        double Dt=0;
        unsigned short port=0;
        const std::string IP = argv[2];
        std::istringstream(argv[0])>>dt;
        std::istringstream(argv[1])>>Dt;
        std::istringstream(argv[3])>>port;

        const std::string yaml_model = argv[4];
        const std::string solver = argv[5];
        TR1(shared_ptr)<SimServer> sim_server (new SimServer(yaml_model, solver, dt, Dt));
        SimulationMessage message(sim_server);
        new Server(message, IP, port);
    }
}


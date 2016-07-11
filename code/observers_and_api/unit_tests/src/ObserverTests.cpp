/*
 * ObserverTests.cpp
 *
 *  Created on: May 6, 2015
 *      Author: cady
 */

#include <ssc/websocket.hpp>

#include "ObserverTests.hpp"
#include "SimObserver.hpp"
#include "Sim.hpp"
#include "SimTest.hpp"
#include "YamlSimulatorInput.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"
#include "STL_data.hpp"
#include "simulator_api.hpp"
#include <ssc/solver.hpp>
#include "TriMeshTestData.hpp"
#include "parse_output.hpp"
#include "ListOfObservers.hpp"
#include "simulator_api.hpp"

#include <unistd.h> // usleep

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234

struct ListOfStringMessages : public ssc::websocket::MessageHandler
{
    ListOfStringMessages() : messages()
    {
    }
    void operator()(const ssc::websocket::Message& msg)
    {
        messages.push_back(msg.get_payload());
    }

    std::vector<std::string> messages;
};

ObserverTests::ObserverTests() : a(ssc::random_data_generator::DataGenerator(542021))
{
}

ObserverTests::~ObserverTests()
{
}

void ObserverTests::SetUp()
{
}

void ObserverTests::TearDown()
{
}

TEST_F(ObserverTests, can_observe_using_a_websocket)
{
//! [ObserverTests example]
    ListOfStringMessages handler;
    TR1(shared_ptr)<ssc::websocket::Server> w(new ssc::websocket::Server(handler, ADDRESS, WEBSOCKET_PORT));
    {
        const auto yaml = test_data::oscillating_cube_example();
        const auto mesh = test_data::cube();
        Sim sys = get_system(yaml, mesh, 0);
        YamlOutput out;
        out.address = WEBSOCKET_ADDRESS;
        out.port = WEBSOCKET_PORT;
        out.data = {"t", "x(cube)", "theta(cube)"};
        out.format = "ws";
        std::vector<YamlOutput> v(1,out);
        ListOfObservers observer(v);
        ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, 0, 1, 0.1, observer);
        usleep(1000); // So the server thread has enough time to process the data
    }
//! [ObserverTests example]
//! [ObserverTests expected output]
    ASSERT_EQ(11, handler.messages.size());
    ASSERT_EQ("{\"t\":0,\"states\":{\"cube\":{\"theta\":-0,\"x\":0}}}", handler.messages.front());
//! [ObserverTests expected output]
}


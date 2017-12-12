/*
 * HOS.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: cady
 */



#include "HOS.hpp"
#include "hos.pb.h"
#include "zmq.hpp"
#include "YamlHOS.hpp"
#include "InternalErrorException.hpp"
#include "ConnexionError.hpp"
#include <chrono>
#include <unistd.h>


HOSComs::Params* get_params(const YamlHOS& yaml);
HOSComs::Params* get_params(const YamlHOS& yaml)
{
    HOSComs::Params* params = new HOSComs::Params();
    // Grid parameters
    params->set_n1(yaml.n1); // Number of modes per knot on axis X
    params->set_n2(yaml.n2); // Number of modes per knot on axis Y
    params->set_m(yaml.m); // Order of the HOS method (order of the Taylor expansion of the velocity potential & the W velocity
    params->set_p1(yaml.p1); // Anti-aliasing parameter for axis X
    params->set_p2(yaml.p2); // Anti-aliasing parameter for axis Y
    params->set_i_case(3); // We only support running HOS with irregular directional sea-state (linear) from spectrum defined as S(ω,θ)=F(ω).G(θ)

    // Time simulation
    params->set_t_stop(10.0); // Length of the simulation (in seconds)
    params->set_f_out(30); // Output sampling frequency (outputs are written every 1/f_out, bigger than the Runge-Kutta time step)
    params->set_toler(yaml.toler); // Runge-Kutta Cash-Karp tolerance (whether its absolute or relative is controlled by 'err')
    params->set_n(4); // Dommermuth initialization
    params->set_ta(0.0); // Dommermuth initialization

    // Physical constants
    params->set_grav(9.81); // Gravity
    params->set_depth(yaml.depth); // Water depth

    // Irregular waves
    params->set_gamma(yaml.gamma); // JONSWAP shape parameter (used for initialization)
    params->set_beta(yaml.beta); // Directional spreading (Dysthe) parameter
    params->set_tp_real(yaml.tp_real); // JONSWAP spectrum period (used for initialization)
    params->set_hs_real(yaml.hs_real); // Significant wave height of the JONSWAP spectrum (used for initialization)

    // Output files
    params->set_tecplot(11); // Tecplot num version
    params->set_i_out_dim(1); // output  = 1-dim ; 0-nondim
    params->set_i_3d(1); // 3d free surface quantities
    params->set_i_a_3d(0); // 3d modes
    params->set_i_2d(0); // 2d free surface, center line
    params->set_i_prob(0); // wave probes in domain
    params->set_i_sw(1); // Activate reconstruction 1='yes', 0='no'
    params->set_qsize_max(999); // Maximum size of the queue in Modes process

    // Post-processing
    params->set_i_ana(0); // Wave-by-wave analysis (0 = no wave-by-wave analysis, 2 = 2D wave-by-wave analysis, 3 = 3D wave-by-wave analysis)
    params->set_i_card(1); // (0 = no output, 1 = velocity and pressure card output, 2 = velocity and pressure card output in boundary fitted coordinates)
    params->set_t_start(1.0); // Start time of the wave-by-wave analysis
    params->set_x_min(2.0*6.28); // Minimum x in velocity and pressure card
    params->set_x_max(10.0*6.28); // Maximum x in velocity and pressure card
    params->set_y_min(2.0*6.28); // Minimum y in velocity and pressure card
    params->set_y_max(10.0*6.28); // Maximum y in velocity and pressure card
    params->set_z_min(-15.0); // Minimum z in velocity and pressure card
    params->set_z_max(5.0); // Maximum z in velocity and pressure card
    params->set_i_zvect(20); // Number of points in z
    params->set_t_interp(1); // Interpolation 0 ='no' ; 1 ='linear'
    params->set_i_out_post(0); // Output post-process 0 ='no' ; '1 ='yes'

    // Horizontal domain
    params->set_xlen(yaml.xlen); // Number of wavelengths (only used if i_case=8x) or peak wavelengths (i_case=3x) in the domain along X-axis
    params->set_ylen(yaml.ylen); // Number of wavelengths (only used if i_case=8x) or peak wavelengths (i_case=3x) in the domain along Y-axis

    switch (yaml.err) // Type of error used for the temporal integration scheme (abs for absolute, rel for relative
    {
        case YamlHOS::ErrorType::ABSOLUTE:
            params->set_err("abs");
            break;
        case YamlHOS::ErrorType::RELATIVE:
            params->set_err("rel");
            break;
        default:
            break;
    }
    params->set_rf_dealias(0); // Apparently unused
    params->set_address_brokerhos("5555"); // Address used by Fortran code to asynchronously send results
    params->set_i_hdf(0); // Save to HDF5 file (0 = don't save, 1 = save to modesspec.h5)
    return params;
}

class HOS::Impl
{
        zmq::message_t receive()
        {
            zmq::message_t msg;
            socket.recv(&msg);
            return msg;
        }

        template <typename T> T receive()
        {
            auto msg = receive();
            T resp;
            resp.ParseFromArray(msg.data(), msg.size());
            return resp;
        }

    public:
        static Impl& get_instance()
        {
            static Impl instance;
            return instance;
        }

        void connect(const YamlHOS& yaml)
        {
            disconnect_if_necessary();
            timeout_in_nanoseconds = (int)std::floor(yaml.timeout_in_seconds*1E9+0.5);
            cos_theta = cos(yaml.direction_of_propagation);
            sin_theta = sin(yaml.direction_of_propagation);
            socket.connect(yaml.address_brokerHOS);
            set_socket_not_to_wait_at_close_time();
            const int timeout_in_ms = (int)std::floor(timeout_in_nanoseconds/1E6+0.5);
            set_receive_timeout_in_ms(timeout_in_ms);
            try
            {
                set_param(yaml);
            }
            catch (const zmq::error_t& e)
            {
                THROW(__PRETTY_FUNCTION__, ConnexionError, "Unable to set HOS parameters: got error '" << e.what() << "' from ZMQ. Is the HOS server up and running at " << yaml.address_brokerHOS << "?");
            }
            send_cmd("RUN");
            connected = true;
        }

        void disconnect_if_necessary()
        {
            if (connected) {send_cmd("EXIT");sleep(1);}
            connected = false;
        }

        ~Impl()
        {
            disconnect_if_necessary();
            google::protobuf::ShutdownProtobufLibrary();
        }


        double eta(const double x_ned, const double y_ned, const double t)
        {
            if (not(parameters_are_set_on_server))
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Need to call HOS::set_parameters first.");
            }
            const double x_hos =  cos_theta*x_ned - sin_theta*y_ned;
            const double y_hos = -sin_theta*x_ned - cos_theta*y_ned;
            HOSComs::GetMessage message;
            message.set_flagval("GET_SURF");
            message.set_time((float)t);
            message.set_vector_size(2);
            message.add_pts((float)x_hos);
            message.add_pts((float)y_hos);
            try
            {
                auto resp = wait_for_response(message);
                if (not(resp.returnvalues_size()))
                {
                    THROW(__PRETTY_FUNCTION__, InternalErrorException, "Expected a array with 2 elements but got 0: cannot extract the wave elevation from what the HOS server returned.");
                }
                return -resp.returnvalues(1);
            }
            catch (const InternalErrorException& exception)
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calling HOS server 'GET_SURF' (for wave elevation): " << exception.get_message());
            }
            return std::nan("");
        }

        double dynamic_pressure(const double x_ned, const double y_ned, const double z_ned, const double t)
        {
            if (not(parameters_are_set_on_server))
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Need to call HOS::set_parameters first.");
            }
            const double x_hos =  cos_theta*x_ned - sin_theta*y_ned;
            const double y_hos = -sin_theta*x_ned - cos_theta*y_ned;
            const double z_hos = -z_ned;
            HOSComs::GetMessage message;
            message.set_flagval("GET_FLOW");
            message.set_time((float)t);
            message.set_vector_size(3);
            message.add_pts((float)x_hos);
            message.add_pts((float)y_hos);
            message.add_pts((float)z_hos);
            try
            {
                auto resp = wait_for_response(message);
                if (resp.returnvalues_size() < 8)
                {
                    THROW(__PRETTY_FUNCTION__, InternalErrorException, "Expected a array with at least 8 elements but got " << resp.returnvalues_size() << ": cannot extract the dynamic wave pressure from what the HOS server returned.");
                }
                return resp.returnvalues(7);
            }
            catch (const InternalErrorException& exception)
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calling HOS server 'GET_FLOW' (for dynamic pressure): " << exception.get_message());
            }
            return std::nan("");
        }

    private:
        HOSComs::DataMessage wait_for_response(const HOSComs::GetMessage& message)
        {
            auto begin = std::chrono::high_resolution_clock::now();
            send(message.SerializeAsString());
            auto resp = receive<HOSComs::DataMessage>();
            auto end = std::chrono::high_resolution_clock::now();
            int elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            while ((resp.flagval() == "WAIT") and (elapsed_nanoseconds < timeout_in_nanoseconds))
            {
                send(message.SerializeAsString());
                resp = receive<HOSComs::DataMessage>();
                end = std::chrono::high_resolution_clock::now();
                elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            }
            if ((elapsed_nanoseconds >= timeout_in_nanoseconds) and (resp.flagval() == "WAIT"))
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Call to HOS timed out after " << elapsed_nanoseconds/1e9 << " seconds.");
            }
            return resp;
        }

        bool send(const std::string& msg)
        {
            zmq::message_t message(msg.size());
            memcpy(message.data(), msg.data(), msg.size());
            return socket.send (message);
        }

        void set_socket_not_to_wait_at_close_time()
        {
            const int linger = 0;
            socket.setsockopt (ZMQ_LINGER, linger);
        }

        void set_receive_timeout_in_ms(const int timeout_in_ms)
        {
            socket.setsockopt(ZMQ_RCVTIMEO, timeout_in_ms);
        }

        void set_param(const YamlHOS& yaml)
        {
            HOSComs::ParamMessage message;
            message.set_flagval("PARAM");
            // Transfer ownership of "get_params()" pointer to protobuf, which is now in charge of freeing it
            // Cf. https://developers.google.com/protocol-buffers/docs/reference/cpp-generated:
            // "void set_allocated_foo(Bar* bar): Sets the Bar object to the field and frees the previous field value if it exists.
            // If the Bar pointer is not NULL, the message takes ownership of the allocated Bar object"
            message.set_allocated_param(get_params(yaml));
            send(message.SerializeAsString());
            receive();
            restart_all();
            send_cmd("SND_PARAM");
            parameters_are_set_on_server = true;
        }

        void send_cmd(const std::string& flag)
        {
            HOSComs::CmdMessage cmd;
            cmd.set_flagval(flag);
            auto begin = std::chrono::high_resolution_clock::now();
            send(cmd.SerializeAsString());
            auto reply = receive();
            auto end = std::chrono::high_resolution_clock::now();
            int elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());
            while ((rpl == "") and (elapsed_nanoseconds < timeout_in_nanoseconds))
            {
                reply = receive();
                rpl = std::string(static_cast<char*>(reply.data()), reply.size());
                end = std::chrono::high_resolution_clock::now();
                elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
            }
            if ((elapsed_nanoseconds >= timeout_in_nanoseconds) and (rpl == ""))
            {
                THROW(__PRETTY_FUNCTION__, InternalErrorException, "Call to HOS timed out when sending command '" << flag << "', after " << elapsed_nanoseconds/1e9 << " seconds.");
            }
        }

        void restart_all()
        {
            restart_if_stopped("LAUNCHER");
            restart_if_stopped("MODES");
            restart_if_stopped("POST");
        }


        void restart_if_stopped(const std::string& process)
        {
            if (status(process) == "OFF")
            {
                send_cmd(std::string("RESTART_") + process);
            }
        }

        std::string status(const std::string& process)
        {
            HOSComs::CmdMessage cmd;
            cmd.set_flagval(std::string("PROC_STATUS_") + process);
            send(cmd.SerializeAsString());
            auto stats = receive<HOSComs::DataMessage>();
            return stats.ppstat().stat();
        }

        Impl()
        : ctx(1)
        , socket(ctx, ZMQ_REQ)
        , timeout_in_nanoseconds()
        , cos_theta()
        , sin_theta()
        , connected(false)
        , parameters_are_set_on_server(false)
        {

        }

        zmq::context_t ctx;
        zmq::socket_t socket;
        int timeout_in_nanoseconds;
        double cos_theta;
        double sin_theta;
        bool connected;
        bool parameters_are_set_on_server;

};

HOS& HOS::get_instance(const std::pair<std::size_t,std::size_t> output_mesh_size, const ssc::kinematics::PointMatrixPtr& output_mesh)
{
    static HOS instance(output_mesh_size,output_mesh);
    return instance;
}


HOS::HOS(const std::pair<std::size_t,std::size_t> output_mesh_size_,
                const ssc::kinematics::PointMatrixPtr& output_mesh_)
 :
                SurfaceElevationInterface(output_mesh_, output_mesh_size_), pimpl(Impl::get_instance())
{
}

void HOS::set_param(const YamlHOS& yaml)
{
    pimpl.connect(yaml);
}

HOS::~HOS()
{
    pimpl.disconnect_if_necessary();
}

double HOS::dynamic_pressure(const double ,   //!< water density (in kg/m^3)
                             const double ,     //!< gravity (in m/s^2)
                             const double x_ned, //!< x-position in the NED frame (in meters)
                             const double y_ned, //!< y-position in the NED frame (in meters)
                             const double z_ned, //!< z-position in the NED frame (in meters)
                             const double ,   //!< Wave elevation at (x,y) in the NED frame (in meters)
                             const double t      //!< Current time instant (in seconds)
                            ) const
{
    return pimpl.dynamic_pressure(x_ned, y_ned, z_ned, t);
}

ssc::kinematics::Point HOS::orbital_velocity(const double ,   //!< gravity (in m/s^2)
                                                const double ,   //!< x-position in the NED frame (in meters)
                                                const double ,   //!< y-position in the NED frame (in meters)
                                                const double ,   //!< z-position in the NED frame (in meters)
                                                const double ,   //!< z-position in the NED frame (in meters)
                                                const double   //!< Wave elevation at (x,y) in the NED frame (in meters)
                                               ) const
{
    return ssc::kinematics::Point("NED", 0, 0, 0);
}

double HOS::evaluate_rao(const double , //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< Current time instant (in seconds)
                            const std::vector<std::vector<double> >& , //!< Module of the RAO
                            const std::vector<std::vector<double> >&  //!< Phase of the RAO
                             ) const
{
    return 0;
}

double HOS::wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                        const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                        const double t  //!< Current instant (in seconds)
                        ) const
{
    return pimpl.eta(x,y,t);
}

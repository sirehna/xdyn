"""Facilitate the creation of a gRPC force model in Python."""
from difflib import SequenceMatcher
import logging
import force_pb2
import force_pb2_grpc
import time
import grpc
import yaml
from concurrent import futures

import inspect


SERVICE_NAME = "grpc-force"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def similar(first_string, second_string):
    """Return a score between 0 (strings are different) and 1 (identical)."""
    return SequenceMatcher(None, first_string, second_string).ratio()


def closest_match(expected_keys, unknown_key):
    """Give a suggestion for the parameter name."""
    if expected_keys:
        return "\nMaybe you meant: " + \
         max(expected_keys, key=lambda k: similar(unknown_key, k)) + \
         " <-> " + unknown_key + "?"
    return ""


NOT_IMPLEMENTED = "is not implemented in this model."


class Model:
    """Derive from this class to implement a gRPC force model for xdyn."""

    def set_parameters(self, parameters):
        """Initialize the force model with YAML parameters.

        Parameters
        ----------
        parameters : string
            YAML string containing the parameters of this model. Can be empty.

        Returns
        -------
        True if the force model needs wave data, False otherwise.

        """
        raise NotImplementedError(inspect.currentframe().f_code.co_name
                                  + NOT_IMPLEMENTED)

    def required_wave_information(self, t, x, y, z):
        """Give the list of wave data needed by this model.

        Only called if set_parameters returns True.

        Parameters
        ----------
        t : double
            Simulation time (in seconds)
        x : double
            Projection on axis X of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame
        y : double
            Projection on axis Y of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame
        z : double
            Projection on axis Z of the NED frame of the vector between the
            origin of the NED frame and the origin of the BODY frame

        Returns
        -------
        dict
            Should contain the following fields:
            - elevations (list of dict): Each element should contain
                - x (double) Points at which the force model requires wave
                  elevations (projection on X-axis, in meters)
                - y (double) Points at which the force model requires wave
                  elevations (projection on Y-axis, in meters)
                - t (double) Time at which the force model requires wave
                  elevations (in seconds)
            - dynamic_pressures (list of dict): Each element should contain
                - x (double) Points at which the force model requires dynamic
                  pressures (projection on X-axis, in meters)
                - y (double) Points at which the force model requires dynamic
                  pressures (projection on Y-axis, in meters)
                - z (double) Points at which the force model requires dynamic
                  pressures (projection on Z-axis, in meters)
                - t (double) Time at which the force model requires dynamic
                  pressures (in seconds)
            - orbital_velocities (list of dict): Each element should contain
                - x (double) Points at which the force model requires orbital
                  velocities (projection on X-axis, in meters)
                - y (double) Points at which the force model requires orbital
                  velocities (projection on Y-axis, in meters)
                - z (double) Points at which the force model requires orbital
                  velocities (projection on Z-axis, in meters)
                - t (double) Time at which the force model requires orbital
                  velocities (in seconds)
            - spectrum (dict or None): If it's a dict, should contain:
                - x (double) Projection on the X axis of the NED frame of the
                  position at which we want the linearized wave spectrum
                - y (double) Projection on the Y axis of the NED frame of the
                  position at which we want the linearized wave spectrum
                - t (double) Simulation time at which we want the linearized
                  wave spectrum
            - angular_frequencies_for_rao (bool): True if the force model
              requires the angular frequencies at which the wave spectrum is
              discretized
            - directions_for_rao (bool): True if the force mode requires the
              incidences at which the wave spectrum is discretized

        """
        raise NotImplementedError(inspect.currentframe().f_code.co_name
                                  + NOT_IMPLEMENTED)

    def force(self, states, commands, wave_information):
        """Calculate the force & torque.

        Parameters
        ----------
        - states : dict
            Should contain the following fields:
            - t (list of doubles): dates at which the state values are given.
              max(t)-min(t) <= max_history_length returned by set_parameters.
            - x (list of doubles): Value of x for each t. x[0] is the
              current (at t) projection on axis X of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - y (list of doubles): Value of y for each t. y[0] is the
              current (at t) projection on axis Y of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - z (list of doubles): Value of z for each t. z[0] is the
              current (at t) projection on axis Z of the NED frame of the
              vector between the origin of the NED frame and the origin of the
              BODY frame.
            - u (list of doubles): Value of u for each t. u[0] is the current
              (at t) projection on axis X of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - v (list of doubles): Value of v for each t. v[0] is the current
              (at t) projection on axis Y of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - w (list of doubles): Value of w for each t. w[0] is the current
              (at t) projection on axis Z of the NED frame of the vector of the
              ship's speed relative to the ground (BODY/NED).
            - p (list of doubles): Value of p for each t. p[0] is the
              current (at t) of the projection on axis X of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - q (list of doubles): Value of q for each t. q[0] is the
              current (at t) of the projection on axis Y of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - r (list of doubles): Value of r for each t. r[0] is the
              current (at t) of the projection on axis Z of the NED frame of
              the vector of the ship's rotation speed relative to the ground
              (BODY/NED).
            - qr (list of doubles): Value of qr for each t. qr[0] is the
              current (at t) value of the real part of the quaternion defining
              the rotation from the NED frame to the ship's BODY frame.
            - qi (list of doubles): Value of qi for each t. qi[0] is the
              current (at t) value of the first imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qj (list of doubles): Value of qj for each t. qj[0] is the
              current (at t) value of the second imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - qk (list of doubles): Value of qk for each t. qk[0] is the
              current (at t) value of the third imaginary part of the
              quaternion defining the rotation from the NED frame to the ship's
              BODY frame.
            - phi (list of doubles): Value of phi for each t. phi[0] is the
              current value (at t) of one of the three Euler angles, the
              meaning of which depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - theta (list of doubles): Value of theta for each t. theta[0] is
              the current value (at t) of one of the three Euler angles, the
              meaning of which depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - psi (list of doubles): Value of psi for each t. psi[0] is the
              current value (at t) of one of the three Euler angles, the
              meaning of which depends on the angle convention chosen in the
              'rotations convention' section of xdyn's input file. See xdyn's
              documentation for details.
            - rotations_convention (string): Angle convention chosen in xdyn's
              YAML file. Use it to check the convention is what you are
              expecting! Format: ["psi", "theta'", "phi''"].
        - commands (dict): contains the current (at t) values of all the
          commands available to all the controlled forces of the simulation.
          The name of the commands is: model_name(command_name) e.g.
          PropRudd(rpm)
        - wave_information (dict): wave information requested by the force
          model (rpc method 'required_wave_information'). Should contain the
          following fields:
            - elevations (dict):
              Should contain the following fields:
            - dynamic_pressures (dict):
              Should contain the following fields:
            - orbital_velocities (dict): Should contain the following fields:
              - vx (list of doubles): Projection on the X-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vy (list of doubles): Projection on the Y-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
              - vz (list of doubles): Projection on the Z-axis of the
                Earth-centered, Earth-fixed North-East-Down reference frame of
                the velocity of each wave partical relative to the ground. In
                meters per second. Same size and ordering as input from rpc
                'required_wave_information' (orbital_velocities).
            - spectrum (dict): Should contain the following fields:
              - si (list of floats): Discretized spectral density for each
                omega (should therefore be the same size as omega).
                In s m^2/rad.
              - dj (list of floats): Spatial spreading for each psi (should
                therefore be the same size as psi. In 1/rad.
              - omega (list of floats): Angular frequencies the spectrum was
                discretized at. In rad/s.
              - psi (list of floats): Directions between 0 & 2pi the spatial
                spreading was discretized at. In rad.
              - k (list of floats): Discretized wave number for each
                frequency (should therefore be the same size as omega).
                In rad/m.
              - phase (list of dict): Random phases, for each
                (direction,frequency) couple (but time invariant), should
                have the same size as psi. In radian.
                Each element is a dict containing the following fields:
                - phase (list of floats): Random phase. Should have the same
                  size as omega in parent dict. In radian.
            - angular_frequencies_for_rao (list of floats):
                Angular frequencies the spectrum was discretized at (in rad/s).
                Used, for example, when interpolating the wave RAOs.
            - directions_for_rao (list of floats):
                Wave incidences the spectrum was discretized at (in rad).
                0° is for waves coming from the North.
                90° for waves coming from the East.
                Used, for example, when interpolating the wave RAOs.


        Returns
        -------
        dict
            Should contain the following fields:
            - Fx (double) Projection of the force acting on "BODY" on the
              X-axis of the body frame (in N).
            - Fy (double) Projection of the force acting on "BODY" on the
              Y-axis of the body frame (in N).
            - Fz (double) Projection of the force acting on "BODY" on the
              Z-axis of the body frame (in N).
            - Mx (double) Projection of the torque acting on "BODY" on the
              X-axis of the body frame (in N.m).
            - My (double) Projection of the torque acting on "BODY" on the
              Y-axis of the body frame (in N.m).
            - Mz (double) Projection of the torque acting on "BODY" on the
              Z-axis of the body frame (in N.m).
            - extra_observations (dict string -> double): Anything we wish
              to serialize. Specific to each force model. Not taken into
              account in the numerical integration & not available to other
              force or environment models.

      """
        raise NotImplementedError(inspect.currentframe().f_code.co_name
                                  + NOT_IMPLEMENTED)


_ONE_DAY_IN_SECONDS = 60 * 60 * 24


class ForceServicer(force_pb2_grpc.ForceServicer):
    """Implements the gRPC methods defined in waves.proto."""

    def __init__(self, model):
        """Constructor.

        Parameters
        ----------
        model : Model
            Implements the scalar force model to use.

        """
        self.model = model

    def set_parameters(self, request, context):
        """Set the parameters of self.model.

        Parameters
        ----------
        request : SetParameterRequest
            Defined in waves.proto.

        Returns
        -------
        dict
            Should contain the following fields:
            - error_message (string): empty if everything went OK.

        """
        LOGGER.info('Received parameters: %s', request.parameters)
        try:
            self.model.set_parameters(request.parameters)
        except KeyError as exception:
            match = closest_match(list(yaml.safe_load(request.parameters)),
                                  str(exception).replace("'", ""))
            context.set_details("Unable to find key "
                                + str(exception)
                                + " in the YAML. " + match)
            context.set_code(grpc.StatusCode.INVALID_ARGUMENT)
            return force_pb2.SetParameterResponse(error_message=
                                                  repr(exception))
        except Exception as exception:
            context.set_details(repr(exception))
            context.set_code(grpc.StatusCode.INVALID_ARGUMENT)
            return force_pb2.SetParameterResponse(error_message=
                                                  repr(exception))
        return force_pb2.SetParameterResponse(error_message='')

    def force(self, request, context):
        try:
            required_wave_information =  \
                self.model.required_wave_information(request.t, request.x,
                                                     request.y, request.z)
        except NotImplementedError as exception:
            context.set_details(repr(exception))
            context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        except Exception as exception:
            context.set_details(repr(exception))
            context.set_code(grpc.StatusCode.UNKNOWN)
        response = force_pb2.ForceResponse()
        response.x[:] = request.x
        response.y[:] = request.y
        response.z[:] = z_s
        response.t = request.t

    def required_wave_information(self, request, context):
        LOGGER.info('required_wave_information')
        response = force_pb2.RequiredWaveInformationResponse()
        if self.wave_information_required:
            try:
                required_wave_information =  \
                    self.model.required_wave_information(request.t, request.x,
                                                         request.y, request.z)
            except NotImplementedError as exception:
                context.set_details(repr(exception))
                context.set_code(grpc.StatusCode.UNIMPLEMENTED)
            except Exception as exception:
                context.set_details(repr(exception))
                context.set_code(grpc.StatusCode.UNKNOWN)
            response = waves_pb2.XYZTGrid()
            response.x[:] = request.x
            response.y[:] = request.y
            response.z[:] = z_s
            response.t = request.t
        return response


def serve(model):
    """Launch the gRPC server."""
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    force_pb2_grpc.add_ForceServicer_to_server(
        ForceServicer(model), server)
    server.add_insecure_port('[::]:9002')
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)

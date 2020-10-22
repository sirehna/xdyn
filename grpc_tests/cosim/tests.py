"""Integration tests for gRPC cosimulation."""

import math
import logging
import os
import unittest
import grpc
from cosimulation_pb2 import CosimulationRequestEuler
import cosimulation_pb2_grpc

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class Cosim:
    """Launch cosimulation steps. Wrapper arround gRPC."""

    def __init__(self):
        xdyn_server_url = os.environ['xdyn_server_url']
        xdyn_channel = grpc.insecure_channel(xdyn_server_url)
        self.xdyn_stub = cosimulation_pb2_grpc.CosimulationStub(xdyn_channel)
        self.request = CosimulationRequestEuler()

    def step(self, state, Dt):
        """Run a cosimulation step."""
        self.request.states.t[:] = [state['t']]
        self.request.states.x[:] = [state['x']]
        self.request.states.y[:] = [state['y']]
        self.request.states.z[:] = [state['z']]
        self.request.states.u[:] = [state['u']]
        self.request.states.v[:] = [state['v']]
        self.request.states.w[:] = [state['w']]
        self.request.states.p[:] = [state['p']]
        self.request.states.q[:] = [state['q']]
        self.request.states.r[:] = [state['r']]
        self.request.states.phi[:] = [state['phi']]
        self.request.states.theta[:] = [state['theta']]
        self.request.states.psi[:] = [state['psi']]
        self.request.Dt = Dt
        res = self.xdyn_stub.step_euler_321(self.request)
        return {'t': res.all_states.t,
                'x': res.all_states.x,
                'y': res.all_states.y,
                'z': res.all_states.z,
                'u': res.all_states.u,
                'v': res.all_states.v,
                'w': res.all_states.w,
                'p': res.all_states.p,
                'q': res.all_states.q,
                'r': res.all_states.r,
                'qr': res.all_states.qr,
                'qi': res.all_states.qi,
                'qj': res.all_states.qj,
                'qk': res.all_states.qk,
                'phi': res.all_states.phi,
                'theta': res.all_states.theta,
                'psi': res.all_states.psi}


class Tests(unittest.TestCase):
    """All unit tests."""

    cosim = Cosim()

    def test_can_run_a_single_cosimulation(self):
        """Make sure we can at least instantiate the gRPC cosimulation."""
        state = {'t': 2,
                 'x': 0,
                 'y': 0,
                 'z': 0,
                 'u': 0,
                 'v': 0,
                 'w': 0,
                 'p': 0,
                 'q': 0,
                 'r': 0,
                 'phi': 0.5*math.pi,
                 'theta': 0,
                 'psi': 0}
        res = self.cosim.step(state, 3)
        print(res)

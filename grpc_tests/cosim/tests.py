"""Integration tests for gRPC cosimulation."""

import logging
import os
import unittest
import grpc
import cosimulation_pb2
from cosimulation_pb2 import CosimulationRequest
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
        self.request = CosimulationRequest()

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
        self.request.states.qr[:] = [state['qr']]
        self.request.states.qi[:] = [state['qi']]
        self.request.states.qj[:] = [state['qj']]
        self.request.states.qk[:] = [state['qk']]
        self.request.Dt = Dt
        return self.xdyn_stub.step(self.request)


class Tests(unittest.TestCase):
    """All unit tests."""

    cosim = Cosim()

    def test_can_run_a_single_cosimulation(self):
        """Make sure we can at least instatiate the gRPC cosimulation."""
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
                 'qr': 0,
                 'qi': 0,
                 'qj': 0,
                 'qk': 0}
        self.cosim.step(state, 3)

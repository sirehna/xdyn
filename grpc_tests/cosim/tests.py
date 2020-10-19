"""Integration tests for gRPC cosimulation."""

import logging
import unittest
import grpc
import os
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
        self.request.states = cosimulation_pb2.States()

    def step(self, state, Dt):
        """Run a cosimulation step."""
        states = cosimulation_pb2.States()
        states.t[:] = [state['t']]
        states.x[:] = [state['x']]
        states.y[:] = [state['y']]
        states.z[:] = [state['z']]
        states.u[:] = [state['u']]
        states.v[:] = [state['v']]
        states.w[:] = [state['w']]
        states.p[:] = [state['p']]
        states.q[:] = [state['q']]
        states.r[:] = [state['r']]
        states.qr[:] = [state['qr']]
        states.qi[:] = [state['qi']]
        states.qj[:] = [state['qj']]
        states.qk[:] = [state['qk']]
        self.request.states = states
        self.request.Dt = Dt
        return self.xdyn_stub.step(self.request)


class Tests(unittest.TestCase):

    cosim = Cosim()

    def test_can_run_a_single_cosimulation():
        state = {'t' = 2,
                'x' = 0,
                'y' = 0,
                'z' = 0,
                'u' = 0,
                'v' = 0,
                'w' = 0,
                'p' = 0,
                'q' = 0,
                'r' = 0,
                'qr' = 0,
                'qi' = 0,
                'qj' = 0,
                'qk' = 0}
        self.cosim.step(state, 3)


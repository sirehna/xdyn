"""Integration tests for gRPC model exchange."""
# pylint: disable=C0111, invalid-name, old-style-class

import logging
import os
import unittest
import grpc
from model_exchange_pb2 import ModelExchangeRequestEuler
import model_exchange_pb2_grpc

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class XDyn:
    """Launch model exchange steps. Wrapper arround gRPC."""

    def __init__(self):
        xdyn_server_url = os.environ['xdyn_server_url']
        xdyn_channel = grpc.insecure_channel(xdyn_server_url)
        self.xdyn_stub = model_exchange_pb2_grpc.ModelExchangeStub(
            xdyn_channel)
        self.request = ModelExchangeRequestEuler()

    def dx_dt(self, state):
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
        res = self.xdyn_stub.dx_dt_euler_321(self.request)
        return {'t': res.d_dt.t,
                'x': res.d_dt.x,
                'y': res.d_dt.y,
                'z': res.d_dt.z,
                'u': res.d_dt.u,
                'v': res.d_dt.v,
                'w': res.d_dt.w,
                'p': res.d_dt.p,
                'q': res.d_dt.q,
                'r': res.d_dt.r,
                'qr': res.d_dt.qr,
                'qi': res.d_dt.qi,
                'qj': res.d_dt.qj,
                'qk': res.d_dt.qk,
                'phi': res.d_dt.phi,
                'theta': res.d_dt.theta,
                'psi': res.d_dt.psi}


EPS = 1E-6


class Tests(unittest.TestCase):
    """All unit tests."""

    xdyn = XDyn()
    d_dt = {}
    initial_state = {}

    def setUp(self):
        self.initial_state = {'t': 2,
                              'x': 1,
                              'y': 2,
                              'z': 3,
                              'u': 4,
                              'v': 5,
                              'w': 6,
                              'p': 0,
                              'q': 0,
                              'r': 0,
                              'phi': 0,
                              'theta': 0,
                              'psi': 0}
        self.d_dt = self.xdyn.dx_dt(self.initial_state)

    def test_can_call_the_model_over_grpc(self):
        """Make sure the derivatives resul are correct."""
        assert abs(self.d_dt['x'] - self.initial_state['u']) < EPS
        assert abs(self.d_dt['y'] - self.initial_state['v']) < EPS
        assert abs(self.d_dt['u']) < EPS
        assert abs(self.d_dt['v']) < EPS
        assert abs(self.d_dt['w']-9.81) < EPS
        assert abs(self.d_dt['p']) < EPS
        assert abs(self.d_dt['q']) < EPS
        assert abs(self.d_dt['r']) < EPS
        assert abs(self.d_dt['r']) < EPS
        assert abs(self.d_dt['qr']) < EPS
        assert abs(self.d_dt['qi']) < EPS
        assert abs(self.d_dt['qj']) < EPS
        assert abs(self.d_dt['qk']) < EPS
        assert abs(self.d_dt['z'] - self.initial_state['w']) < EPS
        assert abs(self.d_dt['phi']) < EPS
        assert abs(self.d_dt['theta']) < EPS
        assert abs(self.d_dt['psi']) < EPS

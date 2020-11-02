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


EPS = 1E-6


class Tests(unittest.TestCase):
    """All unit tests."""

    cosim = Cosim()
    res = {}

    def setUp(self):
        state = {'t': 2,
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
        self.res = self.cosim.step(state, 3)

    def test_can_run_a_single_cosimulation(self):
        """Make sure the cosimulation results are correct."""
        expected_z = [0.0, 0.049049963199999999999, 0.19619999999999999, 0.44144999999999995, 0.7847999999999998, 1.2262499999999998, 1.7658, 2.40345, 3.1391999999999998, 3.9730499999999997, 4.904999999999999, 5.9350499999999995, 7.063199999999999, 8.289449999999999, 9.613799999999998, 11.036249999999997,
                      12.556799999999997, 14.175449999999998, 15.892199999999994, 17.707049999999988, 19.619999999999983, 21.63104999999998, 23.740199999999977, 25.947449999999975, 28.25279999999997, 30.656249999999964, 33.15779999999996, 35.75744999999995, 38.455199999999934, 41.251049999999935, 44.14499999999993]
        for i in range(len(self.res['t'])):
            t = self.res['t'][i]
            assert abs(self.res['z'][i] - expected_z[i] - 3-t*6) < EPS
            assert abs(self.res['x'][i]-t*4-1) < EPS
            assert abs(self.res['y'][i]-t*5-2) < EPS
            assert abs(self.res['u'][i]-4) < EPS
            assert abs(self.res['v'][i]-5) < EPS
            assert abs(self.res['p'][i]) < EPS
            assert abs(self.res['q'][i]) < EPS
            assert abs(self.res['r'][i]) < EPS

    def test_check_time_vector(self):
        """Time vector should have the right size & hold correct values."""
        t_ = 0
        assert len(self.res['t']) == 31
        for t in self.res['t']:
            assert abs(t - t_) < EPS
            t_ += 0.1

    def test_states_length(self):
        """States should all have the same size."""
        assert len(self.res['t']) == len(self.res['x'])
        assert len(self.res['t']) == len(self.res['y'])
        assert len(self.res['t']) == len(self.res['z'])
        assert len(self.res['t']) == len(self.res['u'])
        assert len(self.res['t']) == len(self.res['v'])
        assert len(self.res['t']) == len(self.res['w'])
        assert len(self.res['t']) == len(self.res['p'])
        assert len(self.res['t']) == len(self.res['q'])
        assert len(self.res['t']) == len(self.res['r'])
        assert len(self.res['t']) == len(self.res['qr'])
        assert len(self.res['t']) == len(self.res['qi'])
        assert len(self.res['t']) == len(self.res['qj'])
        assert len(self.res['t']) == len(self.res['qk'])
        assert len(self.res['t']) == len(self.res['phi'])
        assert len(self.res['t']) == len(self.res['theta'])
        assert len(self.res['t']) == len(self.res['psi'])

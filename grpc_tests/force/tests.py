"""Integration tests for gRPC wave models."""

import logging

from websocket import create_connection

SERVICE_NAME = "xdyn-client"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


def run(state):
    """Run a cosimulation step."""
    import json
    ws = create_connection("ws://xdyn:9002")
    ws.send(json.dumps(state))
    result = ws.recv()
    ws.close()
    return json.loads(result)


def test_should_get_the_right_number_of_results_from_xdyn_in_cosim_mode():
    state = {"Dt": 2,
             "states": [{"t": 0, "x": 0, "y": 8, "z": 12, "u": 1, "v": 0,
                         "w": 0, "p": 0, "q": 1, "r": 0, "qr": 1, "qi": 0,
                         "qj": 0, "qk": 0}],
             "commands": {"parametric oscillator(omega)": 3}}
    res = run(state)
    assert len(res) == 21


def test_extra_observations():
    state = {"Dt": 2,
             "states": [{"t": 0, "x": 0, "y": 8, "z": 12, "u": 1, "v": 0,
                         "w": 0, "p": 0, "q": 1, "r": 0, "qr": 1, "qi": 0,
                         "qj": 0, "qk": 0}],
             "commands": {"parametric oscillator(omega)": 3}}
    results = run(state)
    for result in results:
        assert 'k(TestShip)' in result['extra_observations']
        assert result['extra_observations']['k(TestShip)'] == 2

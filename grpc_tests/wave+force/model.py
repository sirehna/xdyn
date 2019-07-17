"""Submarine hydrostatic effort"""

import yaml
import force
import logging
SERVICE_NAME = "sm_rao_qtf.py"

logging.basicConfig(
    format='%(asctime)s,%(msecs)d ['
    + SERVICE_NAME
    + '] - %(levelname)-4s [%(filename)s:%(lineno)d] %(message)s',
    datefmt='%d-%m-%Y:%H:%M:%S')
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(logging.INFO)


class RaoQtf(force.Model):
    """
    """
    def __init__(self):
        """Initialize parameters from 'set_parameters' to None."""
        # self.rho = 1025
        pass

    def set_parameters(self, parameters, body_name):
        """TODO"""
        # param = yaml.safe_load(parameters)
# 
        # self.HDB = param['HDB']
        return {'max_history_length': 0, 'needs_wave_outputs': True,
                'commands': [],
                'frame': body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0, 'theta': 0, 'psi': 0}

    # def required_wave_information(self, _, _, _, _):
    # def required_wave_information(self, t, x, y, z):
    def required_wave_information(self, t, x, y, z):
        LOGGER.info(z)
        LOGGER.info('required_wave_information')
        return {'spectrum': {'t':t, 'x':x, 'y': y},
                'angular_frequencies_for_rao' : True,
                'directions_for_rao': True}

    def force(self, states, _, wave_information):
        """Force model."""

        LOGGER.info('force')
        # LOGGER.info(states)
        LOGGER.info(wave_information)

        Fx = 0.0
        Fy = 0.0
        Fz = 0.0
        Mx = 0.0
        My = 0.0
        Mz = 0.0

        res = {'Fx': Fx,
               'Fy': Fy,
               'Fz': Fz,
               'Mx': Mx,
               'My': My,
               'Mz': Mz,
               'extra_observations':{'HF': 666.0}}
               # 'extra_observations': {'HF': 666.0}}

        return res


if __name__ == '__main__':
    force.serve(RaoQtf())

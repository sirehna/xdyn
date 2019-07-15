"""Damped harmonic oscillator model."""

import yaml
import force


class HarmonicOscillator(force.Model):
    """Restoring force F proportional to the displacement x."""

    def __init__(self):
        """Initialize parameters from 'set_parameters' to None."""
        self.k = 0
        self.c = 0

    def set_parameters(self, parameters, body_name):
        """Parameter k is stiffness and c is damping."""
        param = yaml.safe_load(parameters)
        self.k = param['k']
        self.c = param['c']
        return {'max_history_length': 0, 'needs_wave_outputs': False, 'frame': body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0, 'theta': 0, 'psi': 0}

    def force(self, states, _, __):
        """Force model."""
        return {'Fx': -self.k*states.x[0] - self.c*states.u[0],
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': {}}


if __name__ == '__main__':
    force.serve(HarmonicOscillator())

"""Damped harmonic oscillator model."""

import yaml
import force


class HarmonicOscillator(force.Model):
    """Restoring force F proportional to the displacement x."""

    def __init__(self, parameters, body_name):
        """Initialize parameters from gRPC's set_parameters."""
        param = yaml.safe_load(parameters)
        self.k = param['k']
        self.c = param['c']
        self.body_name = body_name

    def get_parameters(self, body_name):
        """Parameter k is stiffness and c is damping."""
        return {'max_history_length': 0, 'needs_wave_outputs': False,
                'frame': self.body_name, 'x': 0, 'y': 0, 'z': 0, 'phi': 0,
                'theta': 0, 'psi': 0, 'required_commands': ['omega']}

    def force(self, states, commands, __):
        """Force model."""
        omega = commands['omega']
        return {'Fx': omega*(-self.k*states.x[0] - self.c*states.u[0]),
                'Fy': 0,
                'Fz': 0,
                'Mx': 0,
                'My': 0,
                'Mz': 0,
                'extra_observations': {'k': 2}}


if __name__ == '__main__':
    force.serve(HarmonicOscillator)

## Tutoriel 7 : utilisation d'un modèle de houle distant

Ce tutoriel explique comment utiliser un modèle de houle externe
dans xdyn.

Nous utiliserons Docker compose pour lancer le client (xdyn) et le
serveur de houle. Ceci n'est pas obligatoire (on peut se passer de
Docker et Docker Compose pour faire fonctionner l'ensemble), mais
l'utilisation de Docker simplifie grandement la mise en oeuvre.

### Vue d'ensemble

Pour ce tutoriel, on a besoin :

- d'un modèle de houle
- d'une mise en donnée xdyn

Le modèle de houle peut être implémenté en Python. Afin de simplifier
sa mise en oeuvre, on peut utiliser le dépôt
[https://gitlab.sirehna.com/sirehna/demo_docker_grpc](https://gitlab.sirehna.com/sirehna/demo_docker_grpc) qui contient déjà un
exemple de serveur de houle en Python.

### Mise en donnée xdyn

Dans un fichier YAML (nommé `grpc.yml` dans cet exemple) on écrit :

```yaml
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1026, unit: kg/m^3}
    nu: {value: 1.18e-6, unit: m^2/s}
environment models:
  - model: grpc
    url: waves-server:50051
    Hs: 5
    Tp: 15
    gamma: 1.2
    waves propagating to: 0
    omega: [1,2,3]
bodies: # All bodies have NED as parent frame
  - name: cube
    mesh: cube.stl
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: 0.5, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
    initial position of body frame relative to NED:
        frame: NED
        x: {value:  0, unit: m}
        y: {value: 0, unit: m}
        z: {value: 0.25, unit: m}
        phi: {value: 0, unit: deg}
        theta: {value: 2, unit: deg}
        psi: {value: 0, unit: rad}
    initial velocity of body frame relative to NED:
        frame: cube
        u: {value: 0, unit: m/s}
        v: {value: 0, unit: m/s}
        w: {value: 0, unit: m/s}
        p: {value: 0, unit: rad/s}
        q: {value: 0, unit: deg/s}
        r: {value: 0, unit: rad/s}
    dynamics:
        hydrodynamic forces calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        centre of inertia:
            frame: cube
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0.4, unit: m}
        rigid body inertia matrix at the center of gravity and projected in the body frame:
            row 1: [83.33,0,0,0,0,0]
            row 2: [0,83.33,0,0,0,0]
            row 3: [0,0,83.33,0,0,0]
            row 4: [0,0,0,83.33,0,0]
            row 5: [0,0,0,0,83.33,0]
            row 6: [0,0,0,0,0,83.33]
        added mass matrix at the center of gravity and projected in the body frame:
            row 1: [0,0,0,0,0,0]
            row 2: [0,0,0,0,0,0]
            row 3: [0,0,0,0,0,0]
            row 4: [0,0,0,0,0,0]
            row 5: [0,0,0,0,0,0]
            row 6: [0,0,0,0,0,0]
    external forces:
      - model: gravity
      - model: non-linear hydrostatic (fast)
```

### Ecriture du modèle de houle

Dans un fichier Python (nommé `airy.py` dans cet exemple) on écrit :

```python evaluate=False, results='hidden'
"""Airy wave model. As implemented in xdyn."""

import math
import yaml
import numpy as np
import waves


def pdyn_factor(k, z, eta):
    return 0 if (eta != 0 and z < eta) else math.exp(-k * z)


class Airy(waves.AbstractWaveModel):
    def __init__(self):
        self.psi0 = None
        self.jonswap_parameters = {'sigma_a': 0.07, 'sigma_b': 0.09}
        self.directional_spectrum = {}

    def set_parameters(self, parameters):
        param = yaml.safe_load(parameters)
        self.jonswap_parameters['t_p'] = param['Tp']
        self.jonswap_parameters['gamma'] = param['gamma']
        self.directional_spectrum['omega'] = param['omega']
        self.directional_spectrum['psi'] = \
            [param['waves propagating to']*math.pi/180]
        self.jonswap_parameters['hs_square'] = param['Hs']*param['Hs']
        self.jonswap_parameters['omega0'] = 2*math.pi/param['Tp']
        self.jonswap_parameters['coeff'] = 1-0.287*math.log(param['gamma'])
        self.directional_spectrum['si'] = [self.jonswap(omega) for omega in
                                           param['omega']]
        self.directional_spectrum['dj'] = [1]
        self.directional_spectrum['psi'] = [1]
        self.directional_spectrum['k'] = [omega*omega/9.81 for omega in
                                          param['omega']]
        phases = np.random.uniform(low=0,
                                   high=2*math.pi,
                                   size=(len(param['omega']),))
        self.directional_spectrum['phase'] = phases

    def jonswap(self, omega):
        sigma_a = self.jonswap_parameters['sigma_a']
        sigma_b = self.jonswap_parameters['sigma_b']
        omega0 = self.jonswap_parameters['omega0']
        hs_square = self.jonswap_parameters['hs_square']
        coeff = self.jonswap_parameters['coeff']
        gamma = self.jonswap_parameters['coeff']
        sigma = sigma_a if omega <= omega0 else sigma_b
        ratio = omega0/omega
        alpha = ratio*ratio*ratio*ratio
        awm_5 = coeff*5.0/16.0*alpha/omega*hs_square
        bwm_4 = 1.25*alpha
        kappa = (omega-omega0)/(sigma*omega0)
        return awm_5*math.exp(-bwm_4)*math.pow(gamma,
                                               math.exp(-0.5*kappa*kappa))

    def elevation(self, x, y, t):
        zeta = 0
        dir_spec = self.directional_spectrum
        psi = dir_spec['psi'][0]
        for s_i, k, omega, phase in zip(dir_spec['si'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi) + y * math.sin(psi))
            zeta -= s_i * math.sin(-omega*t + k_x_cos_psi_y_sin_psi + phase)
        return zeta

    def dynamic_pressure(self, x, y, z, t):
        dir_spec = self.directional_spectrum
        eta = self.elevation(x, y, t)
        acc = 0
        psi = dir_spec['psi'][0]
        for s_i, k, omega, phase in zip(['si'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi)
                                         + y * math.sin(psi))
            acc -= s_i * pdyn_factor(k, z, eta)*math.sin(-omega*t
                                                         + k_x_cos_psi_y_sin_psi
                                                         + phase)
        return 1000*9.81*acc

    def orbital_velocity(self, x, y, z, t):
        dir_spec = self.directional_spectrum
        eta = self.elevation(x, y, t)
        v_x = 0
        v_y = 0
        v_z = 0
        psi = dir_spec['psi'][0]
        for s_i, k, omega, phase in zip(['si'],
                                        dir_spec['k'],
                                        dir_spec['omega'],
                                        dir_spec['phase']):
            pdyn_factor = self.pdyn_factor(k, z, eta)
            pdyn_factor_sh = pdyn_factor
            k_x_cos_psi_y_sin_psi = k * (x * math.cos(psi)
                                         + y * math.sin(psi))
            theta = -omega * t + k_x_cos_psi_y_sin_psi + phase
            cos_theta = math.cos(theta)
            sin_theta = math.sin(theta)
            a_k_omega = s_i * k / omega
            a_k_omega_pdyn_factor_sin_theta = a_k_omega * pdyn_factor \
                * sin_theta
            v_x += a_k_omega_pdyn_factor_sin_theta * math.cos(psi)
            v_y += a_k_omega_pdyn_factor_sin_theta * math.sin(psi)
            v_z += a_k_omega * pdyn_factor_sh * cos_theta

        return {'vx': v_x, 'vy': v_y, 'vz': v_z}

    def angular_frequencies_for_rao(self):
        return self.directional_spectrum['omegas']

    def directions_for_rao(self):
        return self.directional_spectrum['psis']

    def spectrum(self, x, y, t):
        return self.directional_spectrum


if __name__ == '__main__':
    waves.serve(Airy())
```

### Lancement de la simulation

On commence par récupérer l'exemple de modèle de houle :

```bash
git clone git@gitlab.sirehna.com:sirehna/demo_docker_grpc.git
```

On écrit ensuite un fichier `docker-compose.yml` :

```yaml
version: '3'
services:
  waves-server:
    build: waves_grpc/python_server
    entrypoint: ["/bin/bash", "/entrypoint.sh", "/work/airy.py"]
    working_dir: /work
    volumes:
    - .:/work
    - ./waves_grpc:/proto
  xdyn:
    image: sirehna/xdyn
    working_dir: /data
    entrypoint: xdyn grpc.yml --dt 0.1 --tend 1 -o tsv
    volumes:
    - .:/data
    depends_on:
    - waves-server
```

On peut alors lancer la simulation comme suit :

```bash
docker-compose up
```

### Sans Docker

Si l'on utilise pas Docker, il faut lancer le serveur de houle manuellement:

```shell
python3 airy.py
```

Puis il faut éditer le fichier YAML d'entrée de xdyn en remplaçant :

```yaml
environment models:
  - model: grpc
    url: waves-server:50051
```

par

```yaml
environment models:
  - model: grpc
    url: localhost:50051
```

On peut alors lancer xdyn normalement :

```bash
./xdyn grpc.yml --dt 0.1 --tend 1 -o tsv
```

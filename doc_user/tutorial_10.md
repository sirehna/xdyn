## Tutoriel 10 : utilisation d'un modèle d'effort distant

Ce tutoriel explique comment utiliser un modèle d'effort externe
dans xdyn.

Nous utiliserons Docker compose pour lancer le client (xdyn) et le
serveur (le modèle d'effort). Ceci n'est pas obligatoire (on peut se passer de
Docker et Docker Compose pour faire fonctionner l'ensemble), mais
l'utilisation de Docker simplifie grandement la mise en oeuvre.

### Vue d'ensemble

Pour ce tutoriel, on a besoin :

- d'un modèle d'effort (implémenté en Python dans cet exemple)
- d'une mise en donnée xdyn (un fichier YAML)

### Mise en donnée xdyn

Dans un fichier YAML (nommé `tutorial_10_gRPC_force_model.yml` dans cet exemple) on écrit :

```python echo=False, results='raw', name='tutorial_10_load_yaml'
yaml_data = load_yaml('tutorial_10_gRPC_force_model.yml')
```

```python echo=False, results='raw', name='tutorial_10_print_yaml'
print_yaml(yaml_data)
```

La connexion au modèle d'effort distant est définie dans la section suivante :


```python echo=False, results='raw', name='tutorial_10_print_yaml'
print_yaml(yaml_data, 'bodies/0/controlled forces')
```

- `model: grpc` indique à xdyn qu'il s'agit d'un modèle d'effort distant
- `url: force-model:9902` donne l'adresse réseau à laquelle le modèle d'effort peut être atteint.
  L'utilisation de `docker-compose` nous permet ici de spécifier une adresse égale au nom du modèle
- `name: parametric oscillator` est un nom arbitraire que l'utilisateur donne dans son fichier
  YAML afin de pouvoir faire correspondre d'éventuelles commandes (section `commands` du fichier YAML) à ce modèle d'effort.

Toutes les autres lignes sont envoyées au modèle d'effort en tant que paramètre, sans être interprétées par xdyn.
Dans le cas présent, le modèle n'a qu'un seul paramètre appelé `k` dont la valeur est donnée une fois pour toutes
au début de la simulation à la ligne `k: 2`. 


### Ecriture du modèle d'effort

Dans un fichier Python (nommé `force.py` dans cet exemple) on écrit :

```python evaluate=False, results='hidden'
"""Airy wave model. As implemented in xdyn."""

import math
import yaml
import numpy as np
import grpcforce


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
    entrypoint: xdyn tutorial_09_gRPC_wave_model.yml --dt 0.1 --tend 1 -o tsv
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
./xdyn tutorial_09_gRPC_wave_model.yml --dt 0.1 --tend 1 -o tsv
```

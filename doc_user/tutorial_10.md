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
Dans le cas présent, le modèle a deux paramètres $`k`$ et $`c`$ dont la valeur est donnée une fois pour toutes
au début de la simulation. 


### Ecriture du modèle d'effort

Il s'agit ici d'un modèle d'[oscillateur harmonique amorti](https://en.wikipedia.org/wiki/Harmonic_oscillator#Damped_harmonic_oscillator):

```math
F_x = -k\cdot x - c\cdot u
F_y = 0
F_z = 0
M_x = 0
M_y = 0
M_z = 0
```

Dans un fichier Python (nommé `harmonic_oscillator.py` dans cet exemple) on écrit :

```python evaluate=False, results='hidden'
"""Damped harmonic oscillator model."""

import yaml
import grpcforce


class HarmonicOscillator(grpcforce.Model):
    def __init__(self):
        self.k = None
        self.c = None

    def model_needs_wave_outputs(self):
        return False

    def set_parameters(self, parameters):
        param = yaml.safe_load(parameters)
        self.k = param['k']
        self.c = param['c']

    def force(self, t, states, _, _):
        force = {'Fx': -self.k*states.x(0) - self.c*states.u(0), 'Fy': 0, 'Fz': 0, 'Mx': 0, 'My': 0, 'Mz': 0}
        return {'forces': forces, 'extra outputs': {}}


if __name__ == '__main__':
    grpcforce.serve(HarmonicOscillator())
```

### Lancement de la simulation

On commence par récupérer l'exemple de modèle de houle :

```bash
git clone git@gitlab.sirehna.com:root/xdyn.git
```

On écrit ensuite un fichier `docker-compose.yml` :

```yaml
version: '3'
services:
  force-model:
    build: xdyn/grpc_force_python_server
    entrypoint: ["/bin/bash", "/entrypoint.sh", "/work/harmonic_oscillator.py"]
    working_dir: /work
    volumes:
    - .:/work
    - ./xdyn:/proto
  xdyn:
    image: sirehna/xdyn
    working_dir: /data
    entrypoint: xdyn tutorial_10_gRPC_force_model.yml --dt 0.1 --tend 1 -o tsv
    volumes:
    - .:/data
    depends_on:
    - force-model
```

On peut alors lancer la simulation comme suit :

```bash
docker-compose up
```

### Sans Docker

Si l'on n'utilise pas Docker, il faut lancer le serveur de houle manuellement:

```shell
python3 harmonic_oscillator.py
```

Puis il faut éditer le fichier YAML d'entrée de xdyn en remplaçant :

```yaml
controlled forces:
  - model: grpc
    url: force-model:9002
```

par

```yaml
controlled forces:
  - model: grpc
    url: localhost:50051
```

On peut alors lancer xdyn normalement :

```bash
./xdyn tutorial_10_gRPC_force_model.yml --dt 0.1 --tend 1 -o tsv
```

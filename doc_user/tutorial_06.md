## Tutoriel 6 : propulsion

Jusqu'ici nous n'avons simulé que des efforts environnementaux. Dans ce
tutoriel, nous simulons un propulseur.

### Description du problème

Le navire évolue dans un environnement sans houle. Il est soumis aux quatre
efforts suivants :

- La pesanteur
- Les efforts hydrostatiques (rapides et non exacts)
- L'amortissement visqueux
- Un effort de propulsion

### Écriture du fichier de configuration du simulateur.

Les deux seuls changements par rapport au tutoriel 2 sont l'ajout d'une section
[`controlled forces`](#efforts-commandés) et un
[fichier supplémentaire de commandes](#syntaxe-du-fichier-de-commande)
que l'on nomme `controlled_forces_commands.yml`.

On commence par définir les [caractéristiques du propulseur](#efforts-commandés) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controlled forces:
  - name: port side propeller
    model: wageningen B-series
    position of propeller frame:
        relative to: mesh(body 1)
        x: {value: -4, unit: m}
        y: {value: 0, unit: m}
        z: {value: 2, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: -10, unit: deg}
        psi: {value: 0, unit: deg}
    wake coefficient w: 0.9
    relative rotative efficiency eta: 1
    thrust deduction factor t: 0.7
    rotation: clockwise
    number of blades: 3
    blade area ratio AE/A0: 0.5
    diameter: {value: 2, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les commandes sont définies à la racine du YAML:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
commands:
  - name: port side propeller
    t: [0,1,3,10]
    rpm: {unit: rad/s, values: [3, 30, 30, 40]}
    P/D: [0.7,0.7,0.7,0.7]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En définitive, le fichier d'entrée est :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1026, unit: kg/m^3}
environment models:
  - model: waves
    discretization:
       n: 128
       omega min: {value: 0.1, unit: rad/s}
       omega max: {value: 6, unit: rad/s}
       energy fraction: 0.999
    spectra:
      - model: airy
        depth: {value: 100, unit: m}
        seed of the random data generator: 0
        directional spreading:
           type: dirac
           waves propagating to: {value: 90, unit: deg}
        spectral density:
           type: jonswap
           Hs: {value: 5, unit: m}
           Tp: {value: 15, unit: m}
           gamma: 1.2
      - model: airy
        depth: {value: 100, unit: m}
        seed of the random data generator: 10
        directional spreading:
           type: cos2s
           s: 2
           waves propagating to: {value: 90, unit: deg}
        spectral density:
           type: dirac
           omega0: {value: 0.05, unit: rad/s}
           Hs: {value: 15, unit: m}
    output:
        frame of reference: NED
        mesh:
            xmin: {value: 1, unit: m}
            xmax: {value: 5, unit: m}
            nx: 5
            ymin: {value: 1, unit: m}
            ymax: {value: 2, unit: m}
            ny: 2

commands:
  - name: port side propeller
    t: [0,1,3,10]
    rpm: {unit: rad/s, values: [3, 30, 30, 40]}
    P/D: [0.7,0.7,0.7,0.7]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_06_propulsion.yml --dt 0.1 --tend 20
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Résultats
![Vitesse d'avance (axe x)](images/tutorial_06_propulsion_u.svg)


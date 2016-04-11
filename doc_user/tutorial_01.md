# Tutoriels

## Tutoriel 1: balle en chute libre

Ce tutoriel vise à illustrer le fonctionnement du simulateur. En premier lieu,
nous décrirons le problème physique modélisé, puis sa traduction en YAML,
l'exécution de la simulation, puis le tracé des résultats.

### Description du problème
Le problème physique considéré dans cet exemple est celui du calcul de
l'élévation d'une masse ponctuelle soumise uniquement à l'accélération de la
pesanteur.

### Écriture du fichier de configuration du simulateur.

Le fichier d'entrée du simulateur est au format [YAML](http://www.yaml.org) (une extension du format
JSON).

On commence par définir les conventions de rotation (décrite
[ici](##rotations)):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi, theta', phi'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Puis l'on donne des [constantes
environnementales](##constantes-environnementales) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aucun modèle d'environnement (houle, vent...) n'est nécessaire pour cette
simulation :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
environment models: []
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On définit la position du repère "body" par rapport au maillage :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: -10, unit: m}
        phi: {value: 1, unit: rad}
        theta: {value: 3, unit: rad}
        psi: {value: 2, unit: rad}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les conditions initiales sont décrites comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    initial position of body frame relative to NED:
        frame: NED
        x: {value: 4, unit: m}
        y: {value: 8, unit: m}
        z: {value: 12, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
    initial velocity of body frame relative to NED:
        frame: ball
        u: {value: 1, unit: m/s}
        v: {value: 0, unit: m/s}
        w: {value: 0, unit: m/s}
        p: {value: 0, unit: rad/s}
        q: {value: 0, unit: rad/s}
        r: {value: 0, unit: rad/s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les données dynamiques comprennent la masse, la matrice d'inertie, les inerties ajoutées
et la position du centre d'inertie :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    dynamics:
        centre of inertia:
            frame: ball
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0.5, unit: m}
        mass: {value: 1000, unit: tonne}
        rigid body inertia matrix at the center of buoyancy projected in the body frame:
            frame: ball
            row 1: [1E6,0,0,0,0,0]
            row 2: [0,1E6,0,0,0,0]
            row 3: [0,0,1E6,0,0,0]
            row 4: [0,0,0,1E6,0,0]
            row 5: [0,0,0,0,1E6,0]
            row 6: [0,0,0,0,0,1E6]
        added mass matrix at the center of buoyancy projected in the body frame:
            frame: ball
            row 1: [0,0,0,0,0,0]
            row 2: [0,0,0,0,0,0]
            row 3: [0,0,0,0,0,0]
            row 4: [0,0,0,0,0,0]
            row 5: [0,0,0,0,0,0]
            row 6: [0,0,0,0,0,0]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Seule la gravité agit sur le solide :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    external forces:
      - model: gravity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En définitive, on obtient le fichier suivant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1000, unit: kg/m^3}
environment models: []

bodies: ## All bodies have NED as parent frame
  - name: ball
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: -10, unit: m}
        phi: {value: 1, unit: rad}
        theta: {value: 3, unit: rad}
        psi: {value: 2, unit: rad}
    initial position of body frame relative to NED:
        frame: NED
        x: {value: 4, unit: m}
        y: {value: 8, unit: m}
        z: {value: 12, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
    initial velocity of body frame relative to NED:
        frame: ball
        u: {value: 1, unit: m/s}
        v: {value: 0, unit: m/s}
        w: {value: 0, unit: m/s}
        p: {value: 0, unit: rad/s}
        q: {value: 0, unit: rad/s}
        r: {value: 0, unit: rad/s}
    dynamics:
        centre of inertia:
            frame: ball
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0.5, unit: m}
        mass: {value: 1000, unit: tonne}
        rigid body inertia matrix at the center of buoyancy projected in the body frame:
            frame: ball
            row 1: [1E6,0,0,0,0,0]
            row 2: [0,1E6,0,0,0,0]
            row 3: [0,0,1E6,0,0,0]
            row 4: [0,0,0,1E6,0,0]
            row 5: [0,0,0,0,1E6,0]
            row 6: [0,0,0,0,0,1E6]
        added mass matrix at the center of buoyancy projected in the body frame:
            frame: ball
            row 1: [0,0,0,0,0,0]
            row 2: [0,0,0,0,0,0]
            row 3: [0,0,0,0,0,0]
            row 4: [0,0,0,0,0,0]
            row 5: [0,0,0,0,0,0]
            row 6: [0,0,0,0,0,0]
    external forces:
      - model: gravity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Lancement de la simulation

La simulation peut s'exécuter comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En exécutant cette commande, les résultats s'affichent directement sur la
sortie standard. On peut bien sûr les rediriger vers un fichier en exécutant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 > out.csv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ou en spécifiant directement le fichier de sortie :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -o out.csv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut également changer l'instant initial (étant entendu que les conditions
initiales définies dans le fichier YAML s'appliquent à cet instant initial,
quel qu'il soit, et non pas à t = 0) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 10 --tstart 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut choisir le solveur :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -s rkck
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


La liste de toutes les options est disponible en exécutant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut chaîner les post-traitements (tracés) à la suite de la simulation :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.01 --tend 1 | python plot.py "tutorial_01" 0 3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Résultats

![Élévation au cours du temps](images/tutorial_01.svg)


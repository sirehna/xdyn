## Tutoriel 3 : génération de houle sur un maillage

Le simulateur a vocation à représenter le comportement de solides dans un
environnement fluide, mais il peut aussi servir à simuler un environnement,
sans aucun solide. Ce peut être intéressant par exemple pour générer des champs
de vague afin de tester des algorithmes de prédiction de mouvement sur houle.
Ce tutoriel explique comment utiliser le simulateur pour ce type de simulation.

### Description du problème

Dans cet exemple, nous simulerons une houle d'Airy constituée de la somme de
deux spectres directionnels :

- l'un de densité spectrale JONSWAP et mono-directionnel
- l'autre monochromatique ayant une dispersion en cos2s

On suppose en outre avoir 100 m de fond.

On se limite dans cet exemple à deux spectres, mais le simulateur permet d'en
sommer autant qu'on le souhaite (on n'est limité que par la mémoire de la
machine et par le temps disponible).

### Écriture du fichier de configuration du simulateur.

La section `environment models` est nettement plus fournie que pour les
tutoriels précédents.

On commence par définir la discrétisation. Actuellement, le nombre de
pulsations est égal au nombre de directions :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
discretization:
   n: 128
   omega min: {value: 0.1, unit: rad/s}
   omega max: {value: 6, unit: rad/s}
   energy fraction: 0.999
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On va donc sommer 128 pulsations et 128 directions, soit 16384 points.
Cependant, la discrétisation spatiale des spectres monochromatiques et des
dispersions monodirectionnelles est réduite à un point. On spécifie en outre
que l'on veut représenter 99.9% de l'énergie totale, les autres composantes
n'étant pas retenues.

Le premier spectre est défini de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour le second spectre, on écrit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: airy
  depth: {value: 100, unit: m}
  seed of the random data generator: 10
  directional spreading:
     type: cos2s
     s: 2
     waves propagating to: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On définit les sorties de la façon suivante (#sorties):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
output:
    frame of reference: NED
    mesh:
        xmin: {value: 1, unit: m}
        xmax: {value: 5, unit: m}
        nx: 5
        ymin: {value: 1, unit: m}
        ymax: {value: 2, unit: m}
        ny: 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En définitive, l'environnement est défini de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme on ne simule pas de corps, le fichier d'entrée se réduit à :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi, theta', phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1026, unit: kg/m^3}
	nu: {value: 1.18e-6, unit: m^2/s}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./xdyn tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.h5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le fichier de résultat est `tutorial_03_results.h5`.

### Résultats

On obtient un fichier hdf5 qui peut être ouvert avec différents logiciels comme HDFView.
Dans le groupe "outputs", on trouve un groupe "waves" qui contient quatre jeux de données nommés t, x, y et z.

- t donne les pas de temps de la simulation
- x donne les coordonnées selon x des points où l'élévation est calculée. Chaque ligne correspond à un pas de temps.
- y donne les coordonnées selon y des points où l'élévation est calculée. Chaque ligne correspond à un pas de temps.
- z donne l'élévation aux points définis par x et y. Chaque tranche correspond à un pas de temps.

La description de ce fichier est faite [dans la documentation des fichiers YAML](#sorties).

On peut aussi simuler les vagues dans un autre référentiel que le référentiel
NED (par exemple le référentiel associé à un solide) et l'on obtient alors des
coordonnées changeantes au cours du temps (et donc des valeurs `x` et `y` à
chaque pas de temps). Pour cela, il faut bien évidemment définir un solide à
simuler.


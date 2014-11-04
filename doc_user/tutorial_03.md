# Tutoriel 3 : génération de houle sur un maillage

Le simulateur a vocation à représenter le comportement de solides dans un
environnement fluide, mais il peut aussi servir à simuler un environnement,
sans aucun solide. Ce peut être intéressant par exemple pour générer des champs
de vague afin de tester des algorithmes de prédiction de mouvement sur houle.
Ce tutoriel explique comment utiliser le simulateur pour ce type de simulation.

## Description du problème
Dans cet exemple, nous simulerons une houle d'Airy constituée de la somme de
deux spectres directionnels :

- l'un de densité spectrale JONSWAP et mono-directionnel
- l'autre monochromatique ayant une dispersion en cos2s

On suppose en outre avoir 100 m de fond.

On se limite dans cet exemple à deux spectres, mais le simulateur permet d'en
sommer autant qu'on le souhaite (on n'est limité que par la mémoire de la
machine).

## Ecriture du fichier de configuration du simulateur.

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
     waves coming from: {value: 90, unit: deg}
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
     waves coming from: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On définit les sorties de la façon suivante :

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
           waves coming from: {value: 90, unit: deg}
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
           waves coming from: {value: 90, unit: deg}
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
           waves coming from: {value: 90, unit: deg}
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
           waves coming from: {value: 90, unit: deg}
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

## Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_03_waves.yml --dt 1 --tend 1 -w tutorial_03_results.yml
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le fichier de résultat est `tutorial_03_results.yml`.

## Résultats

On obtient le fichier suivant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
waves:
  x: [1,2,3,4,5,1,2,3,4,5]
  y: [1,1,1,1,1,2,2,2,2,2]
  timesteps:
    - t: 0
      z: [3.7318,3.73178,3.73177,3.73176,3.73174,3.64687,3.64686,3.64684,3.64683,3.64682]
     - t: 1
       z: [4.20712,4.20709,4.20706,4.20703,4.207,4.25937,4.25934,4.25931,4.25928,4.25925]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On peut aussi simuler les vagues dans un autre référentiel que le référentiel
NED (par exemple le référentiel associé à un solide) et l'on obtient alors des
coordonnées changeantes au cours du temps (et donc des valeurs `x` et `y` à
chaque pas de temps). Pour cela, il faut bien évidemment définir un solide à
simuler.


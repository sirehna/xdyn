# Description

Le présent programme est un simulateur de corps solides indéformables dans un
environnement fluide. Il s'agit d'un outil en ligne de commande, c'est-à-dire
qu'on le lance depuis un terminal (ou une invite de commande MS DOS).

## Entrées et sorties

Le simulateur a été construit pour permettre une utilisation modulaire en ligne
de commande. Pour ce faire, les différents domaines à configurer sont séparés :
la description du problème physique est faite dans un ou plusieurs fichiers
d'entrée et la description de la simulation (pas de temps, solveur...) est
faite en ligne de commande. De cette manière, on peut facilement lancer des
simulations du même problème avec des solveurs différents et sur des durées
différentes, sans toucher au(x) fichier(s) de configuration du problème.

Le fichier de description du problème peut être séparé en autant de fichiers
que l'on veut, ce qui permet par exemple de versionner la configuration de
l'environnement physique séparément de celle du ou des solides.

En sortie, le simulateur génère un fichier de résultat au format CSV contenant
tous les états du ou des solides. Il peut également générer un fichier YAML
contenant des hauteurs de houle sur un maillage. La figure suivante résume les
entrées et sorties du simulateur :

![](images/simulator_overview.svg)

## Structure de la documentation

La documentation "utilisateur" du simulateur est composée de cinq parties :

- Le présent fichier, qui donne une vue d'ensemble du simulateur ainsi que la
  documentation des exécutables `sim` et `postprocessing`,
- [Des tutoriels](tutorials.html) permettant d'appréhender le fonctionnement de
  l'outil sur des cas concrets,
- [Une documentation du fichier d'entrée](documentation_yaml.html) donnant le
  paramétrage de tous les modèles connus par le simulateur,
- [Une documentation des modèles physiques](modeles_reperes_et_conventions.html) avec des références vers la
  documentation du fichier d'entrée, pour la définition des phénomènes physiques
  modélisés,
- [Une documentation du solveur](solver.html) qui détaille l'algorithme
  d'intégration numérique utilisé.

Parallèlement à cette documentation utilisateur, il existe également une
documentation d'implémentation décrivant l'architecture logicielle et
détaillant les API pour faciliter la maintenance du code (correctifs et
ajouts de fonctionnalités).

# Ligne de commande

## Liste des arguments

~~~~~~~~~~~~~~~~~~~~ {.bash}
APPEL: sim <yaml file> [-h] [-y ARG] [-s ARG] [dt ARG] [--tstart ARG]
[--tend ARG] [-w ARG]
Options:
  -h [ --help ]              Afficher le message d'aide (en anglais)
  -y [ --yml ] arg           Nom du (ou des) fichier(s) YAML (le flag est facultatif)
  -s [ --solver ] arg (=rk4) Nom du solveur : euler,rk4,rkck for Euler,
                             Runge-Kutta 4 et Runge-Kutta-Cash-Karp
                             respectivement.
  --dt arg                   Pas de temps initial (pour les solveurs à pas
                             variable) ou pas de temps fixe (pour les solveurs
                             à pas fixe)
  --tstart arg (=0)          Date correspondant au début de la simulation
                             (en secondes)
  --tend arg                 Dernier instant calculé
  -w [ --waves ] arg         Nom du fichier YAML de sortie pour la houle (le
                             cas échéant). Cf. section 'output' du fichier YAML
                             d'entrée
  -c [ --commands ] arg      Nom du fichier contenant les commandes à appliquer
                             à chaque instant. Cf. section 'Efforts commandés'
~~~~~~~~~~~~~~~~~~~~

Les résultats sont écrits (au format CSV) vers la sortie standard. On peut les
rediriger vers un fichier en utilisant le caractère `>` (sous MS-DOS, MinGW et
Linux) comme décrit dans la section ci-dessous.

Le paramètre `-w` ne sera pas décrit ici mais dans [le fichier de description
des modèles](documentation_yaml.html#waves).
Le paramètre `-c` ne sera pas décrit ici mais dans [le fichier de description
des modèles](documentation_yaml.html#efforts-commandés).

## Exemples

### Simulation avec un solveur Runge-Kutta d'ordre 4 en commençant à t=0

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.1 --tend 1
~~~~~~~~~~~~~~~~~~~~

Par défaut, les sorties se font sur la sortie standard (le terminal). Ceci
permet de chaîner les traitements (pipe UNIX), par exemple :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.1 --tend 1 | python plot.py test 0 3
~~~~~~~~~~~~~~~~~~~~

La commande précédente lance la simulation et génère un tracé (format SVG) à
l'aide du script python de post-traitement livré avec le simulateur.

On peut bien sûr choisir de rediriger les sorties vers un fichier :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml --dt 0.1 --tend 1 > out.csv
~~~~~~~~~~~~~~~~~~~~

### Simulation avec un solveur Euler en commençant à t=1

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_01_falling_ball.yml -s euler --dt 0.1 --tstart 1 --tend 1.2
~~~~~~~~~~~~~~~~~~~~

# Outil de post-traitement

Les données CSV générées par le simulateur ne contiennent que les états et les
efforts, c'est-à-dire le minimum d'information pour reconstituer toute la
simulation. Comme le simulateur n'utilise que des quaternions en interne, ce ne
sont que les quaternions qui figurent dans les sorties.

Comme il est plus facile de raisonner physiquement sur des angles, on peut
utiliser l'outil `postprocessing` pour effectuer la transformation.

Cet outil a besoin des données YAML fournies au simulateur (afin de reconstituer les
matrices de changement de base), d'une description des données souhaitées (au
format YAML) et du CSV généré par le simulateur.
Les fichiers YAML sont passés en ligne de commande mais le CSV est lu depuis
l'entrée standard, ce qui permet de chaîner le post-traitement avec la
simulation en utilisant un pipe `|`.

L'outil de post-traitement écrit des données CSV sur la sortie standard (ce
qui, une nouvelle fois, permet de chaîner les traitements). Ces sorties
comprennent toutes celles du simulateur plus celles décrites dans la section
`outputs` qui n'est utilisée que par l'outil `postprocessing`. Voici un exemple
d'une telle section :

~~~~~~~~~~~~~~~~~~~~ {.yaml}
outputs:
    angles:
      - frame: Anthineas
        relative to frame: NED
        projected in frame: Anthineas
        axes: [x,y,z]
~~~~~~~~~~~~~~~~~~~~

Le repère indiqué dans `frame` doit être décrit dans le fichier YAML.

On lance ensuite :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./postprocessing f1.yml f2.yml < out.csv
~~~~~~~~~~~~~~~~~~~~

ou bien, ce qui revient au même,

~~~~~~~~~~~~~~~~~~~~ {.bash}
cat out.csv | ./postprocessing f1.yml f2.yml
~~~~~~~~~~~~~~~~~~~~

voire carrément :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim f1.yml --dt 0.1 --tend 20 | ./postprocessing f1.yml f2.yml > out.csv
~~~~~~~~~~~~~~~~~~~~

On peut même réaliser des graphiques en une seule étape :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim f1.yml --dt 0.1 --tend 20 | ./postprocessing f1.yml f2.yml | python postprocessing.py "test" 0 10
~~~~~~~~~~~~~~~~~~~~


# Documentations des données d'entrées du simulateur

Les données d'entrées du simulateur se basent sur un format
de fichiers [YAML](http://www.yaml.org/) qui fonctionne par clef-valeur.

Ce format présente l'avantage d'être facilement lisible et éditable. Des
parsers YAML existent pour de nombreux langages de programmation.

## Vue d'ensemble

### Liste des sections

Le fichier YAML comprend quatre sections de haut niveau :

- La section `rotations convention` définit la convention d'angles utilisée,
- `environmental constants` donne les valeurs de la gravité et la densité de l'eau,
- Les modèles environnementaux figurent dans `environment models`
- `bodies` décrit les corps simulés.

### Remarques sur les unités

On retrouve fréquemment dans le fichier YAML des lignes du type :

~~~~~~~~~~~~~~ {.yaml}
clef: {value: 65456, unit: km}
~~~~~~~~~~~~~~

Les unités ne sont pas vérifiées par le système : le parser se contente de
convertir toutes les entrées en unité du système international. Ainsi, on
aurait tout aussi bien pu écrire :

~~~~~~~~~~~~~~ {.yaml}
clef: {value: 65456, unit: kW}
~~~~~~~~~~~~~~

et obtenir exactement le même résultat.

### Exemple complet

~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi,theta',phi'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1025, unit: kg/m^3}
environment models:
  - model: waves
    discretization:
       n: 10
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
            nx: 2
            ymin: {value: 1, unit: m}
            ymax: {value: 2, unit: m}
            ny: 2
# Fixed frame: NED
bodies: # All bodies have NED as parent frame
  - name: Anthineas
    mesh: anthineas.stl
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 9.355, unit: m}
        y: {value: 0, unit: m}
        z: {value: -3.21, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
    initial position of body frame relative to NED:
        frame: NED
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: -5, unit: m}
        phi: {value: 0, unit: deg}
        theta: {value: -.0058, unit: rad}
        psi: {value: 0, unit: deg}
    initial velocity of body frame relative to NED:
        frame: body 1
        u: {value: 0, unit: m/s}
        v: {value: 0, unit: m/s}
        w: {value: 0, unit: m/s}
        p: {value: 0, unit: rad/s}
        q: {value: 0, unit: rad/s}
        r: {value: 0, unit: rad/s}
    dynamics:
        centre of inertia:
            frame: body 1
            x: {value: 0.258, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0.432, unit: m}
        mass: {value: 253.31, unit: tonne} # Caution: 'ton' is the british ton which is 907.185 kg
        rigid body inertia matrix at the center of buoyancy projected in the body frame:
            frame: body 1
            row 1: [253310,0,0,0,0,0]
            row 2: [0,253310,0,0,0,0]
            row 3: [0,0,253310,0,0,0]
            row 4: [0,0,0,1.522e6,0,0]
            row 5: [0,0,0,0,8.279e6,0]
            row 6: [0,0,0,0,0,7.676e6]
        added mass matrix at the center of buoyancy projected in the body frame:
            frame: body 1
            row 1: [3.519e4,0,0,0,0,0]
            row 2: [0,3.023e5,0,0,0,0]
            row 3: [0,0,1.980e5,0,0,0]
            row 4: [0,0,0,3.189e5,0,0]
            row 5: [0,0,0,0,8.866e6,0]
            row 6: [0,0,0,0,0,6.676e6]
    external forces:
      - model: gravity
      - model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~

## Rotations et constantes environnementales

### Rotations

La convention de rotation permet de retrouver la matrice de rotation d'un
repère par rapport à un autre, étant donné un triplet $(\phi, \theta, \psi)$.
La convention utilisée est décrite dans la section `rotations` :

~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi,theta',phi'']
~~~~~~~~~~~~~~

Cette ligne s'interprète de la façon suivante : étant donné un triplet $(\phi,
\theta, \psi)$, on construit les matrices de rotation en effectuant d'abord une
rotation d'angle $\psi$ autour de l'axe Z, ensuite une rotation d'angle
$\theta$ autour de l'axe Y du repère précédemment transformé, puis une rotation
d'angle $\phi$ autour de l'axe X du repère ainsi obtenu.

Si l'on avait noté :

~~~~~~~~~~~~~~ {.yaml}
rotations convention: [z,y',x'']
~~~~~~~~~~~~~~

on aurait d'abord une rotation d'angle $\phi$ autour de l'axe Z, puis une
rotation d'angle $\theta$ autour du nouvel axe Y, puis une rotation $\psi$
autour du nouvel axe X.

Des apostrophes sont utilisés pour indiquer des compositions de rotations
par rapport au nouveau système d'axes, et donc une composition interne.
Ainsi `[x,y',z'']` désignera une rotation autour X, suivie d'une rotation autour
du  nouvel axe Y, appelé Y' et terminée par une rotation autour du nouvel axe Z,
appelé Z''. La double apostrophe fait référence au deuxième repère utilisée
pour la composition de rotation.

La liste `rotations convention` comporte toujours trois éléments. Le deuxième élément est
toujours différent du premier. Le troisième est soit différent des deux autres,
soit égal au premier.

La convention des angles aéronautiques, fréquemment (et abusivement) dénotée
"angles d'Euler" (lacet $\psi$, tangage $\theta$, roulis $\phi$), se définit de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi, theta', phi'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


La convention d'orientation utilisée dans le logiciel
[Paraview](http://www.paraview.org) est donnée par :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations convention: [psi,phi',theta'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour plus de détails sur les conventions d'angles et d'axes, se référer à [la
documentation détaillée](modeles_reperes_et_conventions.html#conventions-dorientations).

Une attitude sera décrite de la manière suivante, avec les champs

- `frame` le nom du repère dans laquelle l'attitude est exprimée,
- `x` ,`y` ,`z`: le triplet de position où chaque position est
   définie par le dictionnaire des clés `value` et `unit`,
- `phi` ,`theta` ,`psi`, le triplet d'orientation dont l'interprétation en
termes de matrices de rotations dépend de la convention d'orientation choisie

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    frame: NED
    x: {value: 0, unit: m}
    y: {value: 0, unit: m}
    z: {value: 0, unit: m}
    phi: {value: 0, unit: rad}
    theta: {value: 0, unit: rad}
    psi: {value: 0, unit: rad}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


### Constantes environnementales

L'accélération de la pesanteur (dénotée par `g`) et la densité volumique de
l'eau (`rho`) sont des
constantes qui interviennent dans plusieurs modèles physiques. Par conséquent,
plutôt que d'être renseignées au niveau de chaque modèle et risquer ainsi des
incohérence, elles figurent dans la section `environmental constants` qui a la
forme suivante :

~~~~~~~~~~~~~~ {.yaml}
environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1025, unit: kg/m^3}
~~~~~~~~~~~~~~

Comme expliqué dans [une section
précédente](documentation_yaml.html#remarques-sur-les-unit%C3%A9s), les
dimensions physiques ne sont pas vérifiées et simplement converties en unités
du système international.

## Modèles environnementaux

La section `environment` définit les modèles d'environnement pour la simulation
à effectuer. Elle permet de prendre en compte des modèles de houle et de vent.
Elle peut être vide (voir [l'exemple de la chute libre](tutorials.html#tutoriel-1-balle-en-chute-libre)).

### Modèles de houle

#### Absence de houle
Pour simuler une surface libre parfaitement horizontale, on opère de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    environment:
      - model: no waves
        constant sea elevation in NED frame: {value: 0, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`model: no waves` indique que l'on souhaite une surface libre horizontale et
`constant sea elevation in NED frame` représente l'élévation de la surface
libre dans le repère NED.

#### Houle

On peut définir une houle comme étant une somme de plusieurs spectres
directionnels, c'est-à-dire un spectre de puissance et une dispersion spatiale.
Ces spectres doivent être discrétisés. Actuellement, la discrétisation spatiale
est la même que la discrétisation fréquentielle.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
discretization:
   n: 128
   omega min: {value: 0.1, unit: rad/s}
   omega max: {value: 6, unit: rad/s}
   energy fraction: 0.999
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `n` : nombre de points (nombre de fréquences ou nombr de directions)
- `omega min` : pulsation minimale (incluse)
- `omega max` : pulsation maximale (incluse)
- `energy fraction` : les produits de spectre de puissance et d'étalement
directionnel $S_i\cdot D_j$ sont classés par ordre décroissant. On calcule la
somme cumulative et l'on s'arrête lorsque l'énergie accumulée vaut `energy
fraction` de l'énergie totale.

Les spectres directionnels sont définis de la façon suivante :

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

- `model` : actuellement, ne peut valoir qu'`airy`.
- `depth` : profondeur (distance entre le fond et la surface). 0 pour
l'approximation "profondeur infinie". Utilisé pour le calcul du nombre d'onde.
- `seed of the random data generator` : germe utilisé pour la génération des
phases aléatoires.
- `directional spreading` : étalement directionnel. Cf. infra.
- `spectral density` : densité spectrale de puissance. Cf. infra.

#### Etalements directionnels

##### Dirac

Lorsque cet étalement est choisi, la houle est mono-directionnelle.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
     type: dirac
     waves coming from: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La direction de propagation est donnée par `waves coming from`.
Cet étalement est documenté [ici](modeles_reperes_et_conventions.html#dirac-1).

##### cos2s

L'étalement est donné par :
$$\psi\mapsto \cos^{2s}{\psi-\psi_0}$$

où $\psi_0$ est la direction de propagation.

Cet étalement est paramétré de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
   type: cos2s
   s: 2
   waves coming from: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`waves coming from` donne la direction de propagation $\psi_0$.

Cet étalement est documenté [ici](modeles_reperes_et_conventions.html#cos2s).

#### Spectres de puissance

##### Dirac

Lorsque ce spectre est choisi, on obtient une houle monochromatique.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
   type: dirac
   Hs: {value: 5, unit: m}\n"
   omega0: {value: 15, unit: rad/s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La hauteur de houle est donnée par `Hs` et sa pulsation par `omega0`.
Ce spectre est documenté [ici](modeles_reperes_et_conventions.html#dirac).

##### JONSWAP

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: jonswap
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
     gamma: 1.2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce spectre est documenté [ici](modeles_reperes_et_conventions.html#jonswap).

##### Bretschneider

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: bretschneider
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce spectre est documenté
[ici](modeles_reperes_et_conventions.html#bretschneider).

##### Pierson-Moskowitz
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: pierson-moskowitz
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce spectre est documenté
[ici](modeles_reperes_et_conventions.html#pierson-moskowitz).

#### Sorties

On peut sortir les hauteurs de houle calculées sur un maillage (défini dans un
repère fixe ou mobile). En fait, on peut même choisir de ne faire qu'une
simulation de houle, sans corps, tel que décrit dans le [tutoriel 3](tutorials.html#tutoriel-3-g%C3%A9n%C3%A9ration-de-houle-sur-un-maillage).

On définit un maillage (cartésien) sur lequel sera calculé la houle. Par exemple :

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

- `frame of reference` : nom du repère dans lequel sont exprimées les coordonnées des points du maillage.
- `xmin`, `xmax`, `nx` : définition de la discrétisation de l'axe x. Les
  valeurs vont de `xmin` (inclus) à `xmax` (inclus) et il y a `nx` valeurs au
  total.
- `ymin`, `ymax`, `ny` : comme pour x.

Dans l'exemple précédent, les coordonnées sont données dans le repère NED. Le
maillage comporte 10 points :
(1,1),(1,2),(2,1),(2,2),(3,1),(3,2),(4,1),(4,2),(5,1),(5,2).

Les sorties sont écrites dans le fichier spécifié après le flag `-w` en
argument de l'exécutable `sim`.

On obtient deux résultats différents, suivant que le repère dans lequel ils
sont exprimés est mobile ou fixe par rapport au repère NED. En effet, si le
repère est fixe, il est inutile de répéter les coordonnées `x` et `y`.

Dans le cas d'un repère fixe, on obtient une sortie de la forme :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
waves:
  x: [1,2,3,4,5,1,2,3,4,5]
  y: [1,1,1,1,1,2,2,2,2,2]
  timesteps:
    - t: 0
    - z: [-4.60386,-4.60388,-4.6039,-4.60392,-4.60393,-4.6553,-4.65531,-4.65533,-4.65535,-4.65537]
    - t: 1
    - z: [-3.60794,-3.60793,-3.60793,-3.60792,-3.60791,-3.68851,-3.6885,-3.6885,-3.68849,-3.68849]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`x` et `y` désignent les coordonnées (exprimée en mètres) dans le repère choisi
(ici il s'agit du NED) des points du maillage.
`t` désigne l'instant auquel les hauteurs de houle ont été calculées.
`z` est la hauteur de houle, c'est-à-dire la distance entre un point de
coordonnées (x,y,0) et le même point situé sur la surface libre. Une valeur
positive dénote une houle en-dessous de z=0 (creux) et une valeur négative une
valeur au-dessus de z=0 (bosse).

Si le repère de sortie est mobile, on obtient plutôt un résultat de la forme :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
waves:
  timesteps:
    - t: 0
      x: [1,2,3,4,5,1,2,3,4,5]
      y: [1,1,1,1,1,2,2,2,2,2]
    - z: [-4.60386,-4.60388,-4.6039,-4.60392,-4.60393,-4.6553,-4.65531,-4.65533,-4.65535,-4.65537]
    - t: 1
      x: [2,4,5,6,7,2,4,5,6,7]
      y: [1,1,1,1,1,2,2,2,2,2]
    - z: [-3.60794,-3.60793,-3.60793,-3.60792,-3.60791,-3.68851,-3.6885,-3.6885,-3.68849,-3.68849]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Définition des corps simulés

Le simulateur est multi-corps en ce sens que plusieurs corps peuvent être
simulés en même temps. Actuellement, aucun effort d'interaction ni de liaison
cinématique ne sont implémentés.

La section `bodies` du fichier YAML contient une liste de corps, chacun
commençant par un tiret :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
bodies:
    - name: ....
      ...
    - name: ....
      ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chaque corps comprend :

 - un nom (section `name`)
 - éventuellement un maillage (section `mesh`)
 - la position du repère body par rapport au maillage (section `position of
   body frame relative to mesh`)
 - ses conditions initiales (sections `initial position of body frame relative
   to NED` et `initial velocity of body frame relative to NED`)
 - des données définissant son comportement dynamique (section `dynamics`)
 - la liste des efforts auxquels il est soumis (sections `external forces` et `controlled forces`).

### Exemple complet

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
bodies: # All bodies have NED as parent frame
  - name: Anthineas
    mesh: anthineas.stl
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 9.355, unit: m}
        y: {value: 0, unit: m}
        z: {value: -3.21, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
    initial position of body frame relative to NED:
        frame: NED
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: -5, unit: m}
        phi: {value: 0, unit: deg}
        theta: {value: -.0058, unit: rad}
        psi: {value: 0, unit: deg}
    initial velocity of body frame relative to NED:
        frame: body 1
        u: {value: 0, unit: m/s}
        v: {value: 0, unit: m/s}
        w: {value: 0, unit: m/s}
        p: {value: 0, unit: rad/s}
        q: {value: 0, unit: rad/s}
        r: {value: 0, unit: rad/s}
    dynamics:
        hydrodynamic forces calculation point in body frame:
            x: {value: 0.696, unit: m}
            y: {value: 0, unit: m}
            z: {value: 1.418, unit: m}
        centre of inertia:
            frame: body 1
            x: {value: 0.258, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0.432, unit: m}
        mass: {value: 253.31, unit: tonne} # Caution: 'ton' is the british ton which is 907.185 kg
        rigid body inertia matrix at the center of buoyancy projected in the body frame:
            frame: body 1
            row 1: [253310,0,0,0,0,0]
            row 2: [0,253310,0,0,0,0]
            row 3: [0,0,253310,0,0,0]
            row 4: [0,0,0,1.522e6,0,0]
            row 5: [0,0,0,0,8.279e6,0]
            row 6: [0,0,0,0,0,7.676e6]
        added mass matrix at the center of buoyancy projected in the body frame:
            frame: body 1
            row 1: [3.519e4,0,0,0,0,0]
            row 2: [0,3.023e5,0,0,0,0]
            row 3: [0,0,1.980e5,0,0,0]
            row 4: [0,0,0,3.189e5,0,0]
            row 5: [0,0,0,0,8.866e6,0]
            row 6: [0,0,0,0,0,6.676e6]
    external forces:
      - model: gravity
      - model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Nommage du solide

Le nom du solide a son importance puisqu'en définissant un solide, on définit
implicitement le repère qui lui est attaché (le repère "body", cf.
[documentation des repères](modeles_reperes_et_conventions.html#Repère navire
(mobile ou "body"))).

On peut ensuite y faire référence, notamment pour les post-traitements.



### Utilisation d'un maillage

Pour les efforts intégrés sur la carène (par exemple, les efforts
hydrostatiques non-liénaires et les efforts de Froude-Krylov), il est
nécessaire de définir un maillage.

#### Définition du fichier de maillage

La section `mesh` est optionnelle. Si l'on choisit de l'utiliser, elle doit
contenir un nom de fichier
[STL](http://fr.wikipedia.org/wiki/Fichier_de_st%C3%A9r%C3%A9olithographie)
contenant le maillage surfacique du navire au format ASCII. Ce chemin doit être
donné relativement à l'endroit où l'on lance le simulateur.

Par exemple, si l'exécutable du simulateur est dans le répertoire `A/B/C`, que
le maillage `m.stl` est dans `A` et qu'on lance l'exécutable depuis `B`, on
écrira :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
mesh: ../m.stl
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Passage du repère maillage au repère body

L'origine du repère "body" (qui est le repère dans lequel est réalisé le bilan
des efforts) est spécifiée par rapport repère du maillage. En pratique, $x,y,z$
peuvent définir la position du centre de gravité dans le repère maillage et
$\phi,\theta,\psi$ définissent la rotation permettant de passer du repère
maillage au repère body (suivant la convention choisie dans la [section
`rotations`](#rotations)).

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

### Champs `dynamics`

La section `dynamics` permet de décrire l'inertie du solide. Elle est composée
de cinq sous-sections :

- `hydrodynamic forces calculation point in body frame` est le [point de calcul des efforts hydrodynamiques](modeles_reperes_et_conventions.html#rep%C3%A8re-de-calcul-hydrodynamique)
- `centre of inertia` (si le repère "body" n'est pas au centre de masse)
- `mass` contenant la masse du corps considéré
- `rigid body inertia matrix at the center of buoyancy projected in the body
  frame` définissant la matrice d'inertie
- `added mass matrix at the center of buoyancy projected in the body frame`
  pour les masses ajoutées.

#### Point de calcul des efforts hydrodynamiques

Cette section contient simplement les [coordonnées du point de calcul des efforts hydrodynamiques](modeles_reperes_et_conventions.html#rep%C3%A8re-de-calcul-hydrodynamique), exprimées dans le repère body :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
hydrodynamic forces calculation point in body frame:
    x: {value: 0.696, unit: m}
    y: {value: 0, unit: m}
    z: {value: 1.418, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Position du centre d'inertie

La section `centre of inertia` a l'allure suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
centre of inertia:
    frame: body 1
    x: {value: 0, unit: m}
    y: {value: 0, unit: m}
    z: {value: 0, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le champs `frame` doit contenir soit NED, soit un nom de solide, soit
`mesh(<nom de solide>)` suivant le repère choisi pour exprimer les coordonnées
du centre de gravité.

#### Définition de la masse du solide

La section `mass` contient la masse du solide, utilisée notamment pour le
modèle de gravité :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
mass: {value: 1000, unit: tonne}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Attention : si l'on spécifie `ton`, le système utilisera la tonne britannique
qui vaut 907.185 kg.

#### Matrice d'inertie

La matrice d'inertie n'est pas normalisée et l'on n'effectue pas de changement
de repère (ce qui explique l'absence du champs `frame` pour cette section).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rigid body inertia matrix at the center of buoyancy projected in the body frame:
    frame: body 1
    row 1: [253310,0,0,0,0,0]
    row 2: [0,253310,0,0,0,0]
    row 3: [0,0,253310,0,0,0]
    row 4: [0,0,0,1.522e6,0,0]
    row 5: [0,0,0,0,8.279e6,0]
    row 6: [0,0,0,0,0,7.676e6]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Inerties ajoutées

La matrice de masse ajoutée est définie de la même façon :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
added mass matrix at the center of buoyancy projected in the body frame:
    frame: body 1
    row 1: [3.519e4,0,0,0,0,0]
    row 2: [0,3.023e5,0,0,0,0]
    row 3: [0,0,1.980e5,0,0,0]
    row 4: [0,0,0,3.189e5,0,0]
    row 5: [0,0,0,0,8.866e6,0]
    row 6: [0,0,0,0,0,6.676e6]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}

Elle figure dans la section `dynamics` et non dans la section `external forces`
(bien qu'il s'agisse d'un modèle effort, proportionnel à l'accélération)
parce que ce modèle d'effort fait l'objet d'un traitement particulier : il
figure dans le membre de gauche de l'équation fondamentale de la dynamique
$$M\ddot{X} = \sum F_i$$ pour des raisons de stabilité numérique (l'effort
dépend des accélérations qui doivent justement être calculées par la résolution
de l'équation fondamentale de la dynamique).

## Efforts extérieurs

Les efforts extérieurs sont donnés dans la section `external forces` sous forme
de liste de modèles ayant ou non des paramètres. La seule clef commune à tous
les modèles d'effort est `model` : chaque modèle possède sinon sa propre
paramétrisation (éventuellement aucune paramétrisation). Voici un exemple de
section `external forces` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
  - model: gravity
  - model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Gravité

Pour soumettre un solide à la gravité, on écrit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: gravity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La valeur de $g$ utilisée est celle définie dans la section [`environmental
constants`](documentation_yaml.html#constantes-environnementales) et la masse est celle figurant dans
la section [`mass`](documentation_yaml.html#d%C3%A9finition-de-la-masse-du-solide).

Un exemple de simulation de solide soumis uniquement à la gravité (chute libre)
est disponible dans les [tutoriels](tutorials.html#tutoriel-1-balle-en-chute-libre).

La documentation du modèle figure
[ici](modeles_reperes_et_conventions.html#efforts-de-gravit%C3%A9).

### Hydrostatique non-linéaire

On dispose de deux modèles d'efforts hydrostatiques :

- un modèle rapide, appelé `non-lineair hydrostatic (fast)`,
- un modèle plus précis, nommé `non-lineair hydrostatic (exact)`

Pour utiliser le modèle rapide, on écrit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

et pour le modèle précis :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: non-linear hydrostatic (exact)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un exemple de simulation de solide soumis aux efforts hydrostatiques
(oscillations en immersion) est disponible dans les
[tutoriels](tutorials.html#tutoriel-2-oscillations-en-immersion).

La documentation du modèle figure
[ici](modeles_reperes_et_conventions.html#efforts-hydrostatiques-non-lin%C3%A9aires).

### Efforts d'excitation de Froude-Krylov

La dérivation des [efforts d'excitation de Froude-Krylov](modeles_reperes_et_conventions.html#houle-dairy) est décrite (ici)[(modeles_reperes_et_conventions.html#calcul-des-efforts-dexcitation].

Pour l'utiliser, on insère la ligne suivante dans la section `external forces` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: non-linear Froude-Krylov
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Amortissement linéaire

La paramétrisation des efforts d'amortissement linéaires est faite par une matrice renseignée de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: linear damping
  damping matrix at the center of gravity projected in the body frame:
      frame: body 1
      row 1: [ 0, 0,     0,      0,      0, 0]
      row 2: [ 0, 0,     0,      0,      0, 0]
      row 3: [ 0, 0, 1.9e5,      0,      0, 0]
      row 4: [ 0, 0,     0, 1.74e4,      0, 0]
      row 5: [ 0, 0,     0,      0, 4.67e6, 0]
      row 6: [ 0, 0,     0,      0,      0, 0]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette matrice est la matrice $D_l$ décrit dans [la
documentation](modeles_reperes_et_conventions.html#efforts-damortissement-visqueux).

### Amortissement quadratique

La paramétrisation des efforts d'amortissement quadratiques est faite par une matrice renseignée de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: quadratic damping
  damping matrix at the center of gravity projected in the body frame:
      frame: body 1
      row 1: [ 0, 0,     0,      0,      0, 0]
      row 2: [ 0, 0,     0,      0,      0, 0]
      row 3: [ 0, 0, 1.9e5,      0,      0, 0]
      row 4: [ 0, 0,     0, 1.74e4,      0, 0]
      row 5: [ 0, 0,     0,      0, 4.67e6, 0]
      row 6: [ 0, 0,     0,      0,      0, 0]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette matrice est la matrice $((d_{ij}))$ décrit dans [la
documentation](modeles_reperes_et_conventions.html#efforts-damortissement-visqueux).

### Résistance à l'avancement

Les efforts de résistance à l'avancement est renseignée en fonction de la
vitesse d'avance (axe longitudinal uniquement), c'est-à-dire la projection
suivant l'axe $x$ du repère body de la vitesse du navire par rapport au repère
NED. L'interpolation est faite en utilisant des splines cubiques.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: resistance curve
  speed: {unit: knot, values: [0,1,2,3,4,5,15,20]}
  resistance: {unit: MN, values: [0,1,4,9,16,25,225,400]}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cet effort est orienté suivant l'axe $-x$ du repère body.
Le modèle est décrit
[ici](modeles_reperes_et_conventions.html#r%C3%A9sistance-%C3%A0-lavancement).

## Efforts commandés

Les efforts contrôlés correspondent aux efforts de propulsion, de safran et de
foil. Ils sont décrits dans la section `controlled forces`. Les seules clefs
YAML communes à tous les efforts commandés sont `name` (qui est un identifiant
choisi par l'utilisateur) et `model` (qui est une chaîne servant à identifier
le type de modèle utilisé).

La provenance des commandes (où le simulateur lit-il les commandes à chaque pas
de temps) doit être spécifiée lors de l'appel de l'exécutable en
utilisant le flag `--commands` décrit dans la [documentation de l'interface
utilisateur](introduction.html#liste-des-arguments). Les commandes ne sont pas
directement renseignées dans le fichier YAML pour laisser la possibilité à
l'utilisateur de les fournir par un autre biais : il est ainsi prévu de les
lire directement depuis un socket afin de pouvoir s'interfacer avec une
interface graphique ou un pilote.

Voici un exemple de section `efforts commandés` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controlled forces:
  - name: port side propeller
    model: wageningen B-series
    position of propeller frame:
        frame: mesh(body 1)
        x: {value: -4, unit: m}
        y: {value: -2, unit: m}
        z: {value: 2, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: -10, unit: deg}
        psi: {value: -1, unit: deg}
    wake coefficient w: 0.9
    relative rotative efficiency eta: 1
    thrust deduction factor t: 0.7
    rotation: clockwise
    number of blades: 3
    blade area ratio AE/A0: 0.5
    diameter: {value: 2, unit: m}
  - name: starboard propeller
    model: wageningen B-series
    position of propeller frame:
        frame: mesh(body 1)
        x: {value: -4, unit: m}
        y: {value: 2, unit: m}
        z: {value: 2, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: -10, unit: deg}
        psi: {value: 1, unit: deg}
    wake coefficient w: 0.9
    relative rotative efficiency eta: 1
    thrust deduction factor t: 0.7
    rotation: anti-clockwise
    number of blades: 3
    blade area ratio AE/A0: 0.5
    diameter: {value: 2, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Syntaxe du fichier de commande

Le fichier de commande spécifie de manière statique les commandes reçues par
les modèles d'efforts commandés. Il est statique, c'est-à-dire que les
commandes à chaque instant sont connues lors du lancement de la simulation. Son
nom est passé à l'exécutable de simulation en utilisant le flag `-c` (ou
`--commands`).

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- name: port side propeller
  t: [1,3,10]
  rpm: {unit: rpm, values: [3000, 3000, 4000]}
  P/D: [0.7,0.7,0.8]
- name: starboard propeller
  t: [1,3,10]
  rpm: {unit: rpm, values: [3000, 3000, 4000]}
  P/D: [0.7,0.7,0.8]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La valeur renseigné dans `name` doit correspondre à l'identifiant utilisé dans
la section `controlled forces`. Pour chaque effort contrôlé (identifié par
`name`), on donne une liste d'instants (en secondes) puis, pour chaque
commande, les valeurs à ces instants. Il doit donc y avoir, pour chaque
commande, autant de valeurs qu'il y a d'instants et il faut spécifier au moins
deux instants distincts. Entre deux instants, les valeurs des commandes sont
interpolées linéairement. On peut définir autant de clef qu'on le souhaite :
les clefs inutilisées sont simplement ignorées.

Au-delà de la dernière valeur de temps renseignée, la dernière valeur de chaque
commande est maintenue. Avant la première valeur de temps, on utilise la première
valeur de chaque commande. Ainsi, pour l'exemple présenté ci-dessus, pour toute
valeur de $t\geq 10$, alors rpm=4000. Pour $t\leq 1$, rpm=3000

### Wageningen B-series

L'utilisation de ce modèle est présentée dans [la section
tutoriels](tutorials.html#tutoriel-6-propulsion).
Voici un exemple de configuration possible :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controlled forces:
  - name: port side propeller
    model: wageningen B-series
    position of propeller frame:
        frame: mesh(body 1)
        x: {value: -4, unit: m}
        y: {value: -2, unit: m}
        z: {value: 2, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: -10, unit: deg}
        psi: {value: -1, unit: deg}
    wake coefficient w: 0.9
    relative rotative efficiency eta: 1
    thrust deduction factor t: 0.7
    rotation: clockwise
    number of blades: 3
    blade area ratio AE/A0: 0.5
    diameter: {value: 2, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `name` : Nom du composant. Défini par l'utilisateur. Doit correspondre à
celui renseigné dans le fichier de [commandes
attendues](documentation_yaml.html#syntaxe-du-fichier-de-commande).
- `model` : Nom du modèle. Doit être `wageningen B-series` pour utiliser ce
modèle.
- `position of propeller frame` : Définition du [repère de
l'hélice](modeles_reperes_et_conventions.html#expression-des-efforts).
- `frame` : repère dans lequel sont exprimés `x`,`y`,`z`,`phi`,`theta` et `psi`.
- `x`,`y`,`z` : projection de la position du centre de poussée de l'hélice par rapport au centre du repère attaché au maillage et projeté sur ce dernier.
- `phi`,`theta`,`psi` : Définition de la rotation permettant de passer du
repère attaché au maillage au [repère attaché à
l'hélice](modeles_reperes_et_conventions.html#expression-des-efforts), en suivant la
[convention d'angle choisie](documentation_yaml.html#rotations).
- `wake coefficient` : [coefficient de
sillage](modeles_reperes_et_conventions.html#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
traduisant la perturbation de l'écoulement par la coque du navire. Entre 0 et
1.
- `relative rotative efficiency` : [rendement
d'adaptation](modeles_reperes_et_conventions.html#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
- `thrust deduction factor t` : [coefficient de
succion](modeles_reperes_et_conventions.html#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
- `rotation` définition du sens de rotation pour générer une poussée positive.
Utilisé pour calculer le signe du moment généré par l'hélice sur le navire. Les
valeurs possibles sont `clockwise` et `anti-clockwise`. Si on choisit
`clockwise`, l'hélice tournera dans le sens horaire (en se plaçant à l'arrière
du navire et en regardant vers la proue) et génèrera un moment négatif sur le navire (dans le repère de l'hélice). Voir [la
documentation](file:///home/cady/simulator/doc_user/modeles_reperes_et_conventions.html#expression-des-efforts).
- `number of blades` : nombre de pales de l'hélice.
- `blade area ratio AE/A0` : [fraction de
surface](modeles_reperes_et_conventions.html#expression-des-coefficients-k_t-et-k_q) de l'hélice.
- `diameter` : diamètre de l'hélice

La documentation de ce modèle figure
[ici](modeles_reperes_et_conventions.html#hélices-wageningen-série-b).

Les [commandes
attendues](documentation_yaml.html#syntaxe-du-fichier-de-commande) pour ce
modèle sont :

- La vitesse de rotation de l'hélice, toujours positive pour ce modèle, définie
par `rpm`.
- Le ratio "pas sur diamètre", défini par `P/D`.

Voici un exemple de fichier de commande :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- name: port side propeller
  t: [0,1,3,10]
  rpm: {unit: rpm, values: [2500, 3000, 3000, 4000]}
  P/D: [0.7,0.7,0.7,0.7]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


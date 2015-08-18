# Reperes et conventions

## Repères

Afin de connaître et décrire l'attitude d'un ou plusieurs corps dans l'espace,
il est nécessaire de les placer par rapport à un repère de référence.

### Repère de référence

Le repère `NED` (North-East-Down) est utilisé comme repère de référence,
avec un point de référence $O$ et une base pointant les directions Nord-Est-Bas.
Il sert à exprimer les déplacements des corps de la simulation.

### Repère navire (mobile ou "body")

Le repère navire correspond au repère attaché au navire lors de la simulation.
Le point de référence de ce repère correspond généralement au centre de carène
du navire.
Les axes du repère navire sont les suivants
- $X$ vers l'avant
- $Y$ sur tribord
- $Z$ vers le bas

![](images/ShipFrame.svg "Repère navire")

### Repère NED local

Lors des exports de champs de vagues, ni le repère NED ni le repère navire
ne sont parfaitement adaptés : en effet, si le maillage sur lequel on
calcule la houle est lié à NED, le navire finira par sortir de cette zone
lorsqu'il se déplacera. Si l'on calcule les hauteurs de
vague dans le repère navire, l'aire de la grille vue dans le repère NED
va varier en fonction de l'attitude du navire et, à la limite, pour un
navire vertical ($\theta=\pi/2$ par exemple), la projection de la grille
est un segment.

On définit donc un NED "local", c'est-à-dire un repère centré au même point
que le repère navire mais simplement translaté par rapport à NED :

![](images/local_ned.svg "Repère NED local (plan X,Y)")


![](images/local_ned2.svg "Repère NED local (plan X,Z)")

Ce repère est nommé "NED(body)". Ainsi, si le navire s'appelle "nav1", le
repère NED local sera "NED(nav1)".

### Attitude navire

L'attitude d'un corps permet de connaître sa position et son orientation par
rapport à un repère. La position est donnée par le triplet $\left(X,Y,Z\right)$
et l'orientation par un triplet de trois angles $\left(\phi,\theta,\psi\right)$. L'interprétation de ce triplet en termes de rotations autour des axes $x$,$y$,$z$ dépend de la convention d'orientation choisie.
L'orientation peut également être exprimée de manière différente notamment avec des
quaternions (c'est d'ailleurs ainsi qu'elle est exprimée dans le code du simulateur).

## Conventions d'orientations

Cette section présente les notations utilisées pour définir l'orientation
d'un élément dans l'espace à partir d'un triplet de trois angles $(\phi,\theta,\psi)$.


### Définition d'une orientation

Pour définir la composition de rotations donnant l'orientation
d'un élément dans l'espace à partir
d'un triplet de trois angles $\left(\phi,\theta,\psi\right)$,
plusieurs éléments doivent être définis:

- une convention d'angles ou d'axes. Elle permet de définir
  si ce sont les angles ou les axes qui évoluent pour la
  notation utilisée.
- une composition des rotations interne ou externe. Elle définit
  si la composition des rotations s'effectue par rapport à un
  système d'axes fixes ou alors par rapport au système d'axes
  nouvellement modifiés.
- un ordre dans lequel est appliqué les rotations.
  Il permet de définir complètement la composition de rotations.


### Enumération des conventions possibles

Si on choisit une convention d'angles, alors chaque angle du triplet définit
respectivement une rotation autour d'un axe $X$, $Y$ ou $Z$.
Les axes ne peuvent être répétés.
Il est possible de définir 6 conventions d'angles, qui correspondent à
la permutation des trois axes: $XYZ$ ,$XZY$ ,$YXZ$ ,$YZX$ ,$ZXY$ ,$ZYX$.
Par exemple la rotation $R_{YZX}$ appliquée au triplet
$\left(\phi,\theta,\psi\right)$ s'interprétera comme une rotation de
$R_{Y}\left(\theta\right)$, suivie de la rotation $R_{Z}\left(\psi\right)$,
et terminée par la rotation $R_{X}\left(\phi\right)$.

Si on choisit une convention d'axes, alors on modifie l'ordre des axes
sur lesquels appliquer successivement les rotations.
Des répétitions des axes sont alors possibles, si elles ne se suivent pas.
Par exemple, $XYX$ sera valide, mais pas $XXY$.
Par exemple, une convention ZXY définit une composition de rotations
Il est possible de définir 12 conventions d'axes:
$XYX$, $XYZ$, $XZX$, $XZY$, $YXY$, $YXZ$,
$YZX$, $YZY$, $ZXY$, $ZXZ$, $ZYX$, $ZYZ$.
Par exemple la rotation $R_{YZX}$ appliquée au triplet
$\left(\phi,\theta,\psi\right)$ s'interprétera comme une rotation de
$R_{Y}\left(\phi\right)$, suivie de la rotation $R_{Z}\left(\theta\right)$, et
terminée par la rotation $R_{X}\left(\psi\right)$.

Avec ces conventions d'angles et d'axes, il existe déjà 18 combinaisons.
Ce nombre est doublé du fait que la composition de rotations peut être interne
(intrinsic) ou externe(extrinsic).
Si les rotations sont composées par rapport au repère fixe, on parle de
composition externe. Si les rotations sont composées par rapport aux repères
nouvellement créés, on parle de composition interne. C'est cette dernière qui
est utilisée dans la majorité des cas. Au total, ce sont ainsi 36 conventions
qu'il est possible définir.

### Synthèse des différents conventions

Le tableau suivant présente les 36 conventions possibles :

+----+-------+-------------+---------------+-----------------------------------+------------------------+
| id | Order |  Convention |   Composition |       Matrice de rotation         |     Remarques          |
+====+=======+=============+===============+===================================+========================+
|  1 | angle |  x y z      |    Extrinsic  | $R_Z(\psi).R_Y(\theta).R_X(\phi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  2 | angle |  x z y      |    Extrinsic  | $R_Y(\theta).R_Z(\psi).R_X(\phi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  3 | angle |  y x z      |    Extrinsic  | $R_Z(\psi).R_X(\phi).R_Y(\theta)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  4 | angle |  y z x      |    Extrinsic  | $R_X(\phi).R_Z(\psi).R_Y(\theta)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  5 | angle |  z x y      |    Extrinsic  | $R_Y(\theta).R_X(\phi).R_Z(\psi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  6 | angle |  z y x      |    Extrinsic  | $R_X(\phi).R_Y(\theta).R_Z(\psi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  7 | angle |  x y' z''   |    Intrinsic  | $R_X(\phi).R_Y(\theta).R_Z(\psi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  8 | angle |  x z' x''   |    Intrinsic  | $R_X(\phi).R_Z(\psi).R_Y(\theta)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
|  9 | angle |  y x' z''   |    Intrinsic  | $R_Y(\theta).R_X(\phi).R_Z(\psi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 10 | angle |  y z' x''   |    Intrinsic  | $R_Y(\theta).R_Z(\psi).R_X(\phi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 11 | angle |  z x' y''   |    Intrinsic  | $R_Z(\psi).R_X(\phi).R_Y(\theta)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 12 | angle |  z y' x''   |    Intrinsic  | $R_Z(\psi).R_Y(\theta).R_X(\phi)$ |                        |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 13 | axis  |  x y x      |    Extrinsic  | $R_X(\psi).R_Y(\theta).R_X(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 14 | axis  |  x y z      |    Extrinsic  | $R_Z(\psi).R_Y(\theta).R_X(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 15 | axis  |  x z x      |    Extrinsic  | $R_X(\psi).R_Z(\theta).R_X(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 16 | axis  |  x z y      |    Extrinsic  | $R_Y(\psi).R_Z(\theta).R_X(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 17 | axis  |  y x y      |    Extrinsic  | $R_Y(\psi).R_X(\theta).R_Y(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 18 | axis  |  y x z      |    Extrinsic  | $R_Z(\psi).R_X(\theta).R_Y(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+


+----+-------+-------------+---------------+-----------------------------------+------------------------+
| id | Order |  Convention |   Composition |       Matrice de rotation         |     Remarques          |
+====+=======+=============+===============+===================================+========================+
| 19 | axis  |  y z x      |    Extrinsic  | $R_X(\psi).R_Z(\theta).R_Y(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 20 | axis  |  y z y      |    Extrinsic  | $R_Y(\psi).R_Z(\theta).R_Y(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 21 | axis  |  z x y      |    Extrinsic  | $R_Y(\psi).R_X(\theta).R_Z(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 22 | axis  |  z x z      |    Extrinsic  | $R_Z(\psi).R_X(\theta).R_Z(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 23 | axis  |  z y x      |    Extrinsic  | $R_X(\psi).R_Y(\theta).R_Z(\phi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 24 | axis  |  z y z      |    Extrinsic  | $R_Z(\psi).R_Y(\theta).R_Z(\phi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 25 | axis  |  x y' x''   |    Intrinsic  | $R_X(\phi).R_Y(\theta).R_X(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 26 | axis  |  x y' z''   |    Intrinsic  | $R_X(\phi).R_Y(\theta).R_Z(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 27 | axis  |  x z' x''   |    Intrinsic  | $R_X(\phi).R_Z(\theta).R_X(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 28 | axis  |  x z' y''   |    Intrinsic  | $R_X(\phi).R_Z(\theta).R_Y(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 29 | axis  |  y x' y''   |    Intrinsic  | $R_Y(\phi).R_X(\theta).R_Y(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 30 | axis  |  y x' z''   |    Intrinsic  | $R_Y(\phi).R_X(\theta).R_Z(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 31 | axis  |  y z' x''   |    Intrinsic  | $R_Y(\phi).R_Z(\theta).R_X(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 32 | axis  |  y z' y''   |    Intrinsic  | $R_Y(\phi).R_Z(\theta).R_Y(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 33 | axis  |  z x' y''   |    Intrinsic  | $R_Z(\phi).R_X(\theta).R_Y(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 34 | axis  |  z x' z''   |    Intrinsic  | $R_Z(\phi).R_X(\theta).R_Z(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 35 | axis  |  z y' x''   |    Intrinsic  | $R_Z(\phi).R_Y(\theta).R_X(\psi)$ | Euler                  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+
| 36 | axis  |  z y' z''   |    Intrinsic  | $R_Z(\phi).R_Y(\theta).R_Z(\psi)$ | Cardan - Tait - Bryan  |
+----+-------+-------------+---------------+-----------------------------------+------------------------+


où les matrices de rotation autour des trois axes $X$, $Y$ et $Z$ s'écrivent

$R_X \left( \alpha \right) = \left[\begin{array}{ccc}
1 & 0 & 0\\
0 & +\cos \left( \alpha \right) & -\sin \left( \alpha \right)\\
0 & +\sin \left( \alpha \right) & +\cos \left( \alpha \right)
\end{array}\right]$

$R_Y \left( \alpha \right) = \left[\begin{array}{ccc}
+\cos\left( \alpha \right) & 0 & +\sin \left( \alpha \right) \\
0 & 1 & 0 \\
-\sin\left( \alpha \right) & 0 & +\cos \left( \alpha \right) \\
\end{array}\right]$

$R_Z \left( \alpha \right) = \left[\begin{array}{ccc}
+\cos \left( \alpha \right) & -\sin \left( \alpha \right) & 0 \\
+\sin \left( \alpha \right) & +\cos \left( \alpha \right) & 0 \\
0 & 0 & 1 \\
\end{array}\right]$

## Conventions couramment utilisées

Parmi l'ensemble des conventions possibles, certaines sont plus utilisées que
d'autres.

### Convention des angles aéronautiques

La convention des angles aéronautiques (convention de type 2 de la norme AFNOR)
exprimée par le triplet (Roulis, Tangage, Lacet) régulièrement utilisée est
référencée id=12 dans le tableau ci-dessus.

Elle se comprend de la manière suivante, on effectue une rotation de
l'angle de lacet autour de l'axe $Z$, suivie d'une rotation de
l'angle d'assiette autour du nouvel axe $Y'$ suivie d'une rotation de
l'angle de roulis autour du nouvel axe $X''$.

Si on exprime ce triplet de la manière suivante (Lacet, Roulis, Tangage),
on obtient id=33 dans le tableau ci-dessus.

La composition de rotations de ces deux conventions est
représentée sur la figure suivante
![](images/AnglesEuler.svg "Composition de rotations [z,y',x'']")


### Convention Paraview

La convention d'orientation utilisée dans le logiciel
[Paraview](http://www.paraview.org) est identifiée par id=11 dans le tableau
ci-dessus.

Cette composition de rotation se comprend comme une rotation $\psi$ autour de
l'axe $Z$, suivie d'une rotation $\theta$ autour du nouvel axe $X'$ et
finalement d'une rotation $\phi$ autour du nouvel axe $Y''$.


## Quaternions

L'utilisation des angles d'Euler pose deux problèmes principaux :

- Le blocage de Cardan, d'une part, qui se manifeste par la perte d'un degré de
  liberté lorsque les axes de deux des trois rotations définissant la
  transformation du repère NED au repère navire sont confondus,
- La périodicité des angles, d'autre part, qui introduit des discontinuités dans
  les états.

Une manière usuelle de contourner ces problèmes est d'utiliser des quaternions
qui, au prix de l'ajout d'un état supplémentaire permettent de définir les
rotations sans ambiguïté et de façon unique, quelle que soit la convention
d'angle adoptée.

## Etats navires

Le simulateur est multi-corps en ce sens que plusieurs corps peuvent être
simulés en même temps. Actuellement, aucun effort d'interaction ni de liaison
cinématique ne sont implémentés.

Chaque corps possède des états, permettant de reconstituer exactement son
mouvement. Ces états sont les suivants :

- La position du corps par rapport à l'origine du NED projetée dans le repère
  corps est notée $p^n = [x,y,z]^T$ et est exprimée en mètres.
- L'attitude du corps est notée $\Theta = [\phi,\theta,\psi]^T$ et est définie au
  paragraphe précédent. En pratique, on utilise plutôt des quaternions $q = [q_r,
  q_i, q_j, q_k]$ en interne dans le simulateur pour l'intégration des équations
  du mouvement, mais les raisonnements présentés dans cette documentation se
  transposent aisément.
- La vitesse de translation du corps par rapport au repère fixe NED, projetée
  dans le repère du corps (ou "body") est notée $v^b = [u,v,w]^T$ et s'exprime en m/s.
- Le vecteur vitesse de rotation du repère "body" par rapport au repère NED,
  projeté dans le repère "body", est noté $\omega_{nb}^b = [p,q,r]^T$
  et s'exprime en rad/s.
- Les efforts appliqués au navire et projetés dans le repère "body" sont notés :
  $f^b = [X,Y,Z]^T$. Ils s'expriment en N.
- Les moments appliqués au navire et projetés dans le repère "body" sont notés :
  $m^b = [K,M,N]^T$. Ils s'expriment en N.m.

## Définition des corps simulés

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
 - la position du repère body par rapport au maillage (section
  `position of body frame relative to mesh`)
 - ses conditions initiales (sections
   `initial position of body frame relative to NED` et
   `initial velocity of body frame relative to NED`)
 - des données définissant son comportement dynamique (section `dynamics`)
 - la liste des efforts auxquels il est soumis (sections `external forces` et
   `controlled forces`).
 - de façon facultative, des états forcés.

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
    forced dof:
       from CSV:
         - state: u
           t: T
           value: PS
           interpolation: spline
           filename: test.csv
       from YAML:
         - state: p
           t: [4.2]
           value: [5]
           interpolation: piecewise constant
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Nommage du solide

Le nom du solide a son importance puisqu'en définissant un solide, on définit
implicitement le repère qui lui est attaché (le repère "body", cf.
[documentation des repères](#Repère navire
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

- `hydrodynamic forces calculation point in body frame` est le [point de calcul des efforts hydrodynamiques](#rep%C3%A8re-de-calcul-hydrodynamique)
- `centre of inertia` (si le repère "body" n'est pas au centre de masse)
- `mass` contenant la masse du corps considéré
- `rigid body inertia matrix at the center of buoyancy projected in the body
  frame` définissant la matrice d'inertie
- `added mass matrix at the center of buoyancy projected in the body frame`
  pour les masses ajoutées.

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Elle figure dans la section `dynamics` et non dans la section `external forces`
(bien qu'il s'agisse d'un modèle effort, proportionnel à l'accélération)
parce que ce modèle d'effort fait l'objet d'un traitement particulier : il
figure dans le membre de gauche de l'équation fondamentale de la dynamique
$$M\ddot{X} = \sum F_i$$ pour des raisons de stabilité numérique (l'effort
dépend des accélérations qui doivent justement être calculées par la résolution
de l'équation fondamentale de la dynamique).

Il est également possible d'extrapoler les masses ajoutées à pulsation infinie
à partir d'un fichier HDB. Pour cela, on écrit (pour lire depuis le fichier
`anthineas.hdb`) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
added mass matrix at the center of buoyancy projected in the body frame:
    from hdb file: anthineas.hdb
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans ce cas, il ne faut pas spécifier les clefs `frame` et `row` (le programme
lance une exception si on le fait).
Comme le fichier STL, le chemin du fichier HDB est relatif à l'endroit d'où on
lance l'exécutable.

### Forçage de degrés de liberté

Il est possible de forcer les valeurs des degrés de liberté suivant : U, V, W,
P, Q, R.

Pour forcer les degrés de liberté, on ajoute la section (facultative) suivante
à la section `body` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
blocked dof:
   from CSV:
     - state: u
       t: T
       value: PS
       interpolation: spline
       filename: test.csv
   from YAML:
     - state: p
       t: [4.2]
       value: [5]
       interpolation: piecewise constant
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Soit les états sont donnés directement dans le fichier YAML, soit ils sont lus
depuis un fichier CSV.
- `name`: nom de l'état à forcer. u, v, w, p, q ou r.

Si les valeurs des états sont dans le YAML :

- `value`: Valeur de l'état forcé pour chaque instant
   (dans l'exemple ci-dessus, u=5 pour t $\geq$ 4.2).
- `t`: instants auxquels est défini l'état
- `interpolation`: type d'interpolation à réaliser. `piecewise constant`,
  `linear` ou `spline`.

Si les valeurs sont lues depuis un fichier CSV :

- `filename`: nom du fichier contenant les valeurs forcées. Le chemin du
  fichier s'entend relativement à l'endroit d'où est lancé le simulateur.
- `value`: nom de la colonne contenant les valeurs à lire
- `t`: nom de la colonne contenant le temps
- `interpolation`: type d'interpolation à réaliser. `piecewise constant`,
  `linear` ou `spline`.

Pour les valeurs de t hors de l'intervalle [tmin,tmax], l'état est supposé
libre (non forcé).

## Repère de calcul hydrodynamique

Les efforts d'[amortissement
visqueux](#efforts-damortissement-visqueux)
et de [résistance à
l'avancement](#r%C3%A9sistance-%C3%A0-lavancement)
sont calculés dans un repère appelé **repère de calcul hydrodynamique**, qui
est un repère translaté par rapport au repère body. Le centre de ce repère est
un point défini (dans le repère body) de la façon suivante :

- Son abscisse $x$ est celle du centre de la surface résultant de la projection
du maillage sur le plan $(x,z)$
- Son ordonnée $y$ vaut zéro
- Son altitude $z$ est celle du centre de la surface résultant de la projection
du maillage sur le plan $(x,y)$

Ce point est, en général, distinct du centre de gravité et du centre de volume.
Il est défini dans la section `dynamics/hydrodynamic forces calculation point
in body frame` du fichier YAML :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
hydrodynamic forces calculation point in body frame:
    x: {value: 0.696, unit: m}
    y: {value: 0, unit: m}
    z: {value: 1.418, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


On note ${}^{\mbox{local}}T_{\mbox{body}}$ la transformation permettant de
convertir des coordonnées dans le repère body en coordonnées du même point
exprimées dans le repère de calcul hydrodynamique.
${}^{\mbox{local}}T_{\mbox{NED}}$ est celle permettant de convertir des coordonnées
dans le repère NED  en coordonnées du même point exprimées dans le repère de
calcul hydrodynamique.

Il convient de distinguer ce repère de celui utilisé dans la base de données
hydrodynamiques (fichiers HDB de Diodore), utilisé pour l'expression des
matrices d'amortissement de radiation, les RAO d'effort (diffraction) et les
masses ajoutées.


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
documentation détaillée](#conventions-dorientations).

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



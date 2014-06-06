# Guide utilisateur

Le simulateur léger de comportement de navires a été développé par DCNS Research pour l'IRT Jules Verne.

Ce simulateur permet de réaliser de nombreuses simulations au stade d'avant-projet,
permettant d'appréhender le comportement à la mer du navire muni de ses actionneurs,
en incluant certains aspects non-linéaires de ce comportement,
et intégrant des fonctionnalités de manœuvrabilité.

# Documentations des données d'entrées du simulateur

Les données d'entrées du simulateur se base sur un format de fichiers [YAML](http://www.yaml.org/)

Ce format présente l'avantage d'être facilement lisible et éditable par 
l'homme et facilement interprétable par la machine.

## Repère

Afin de connaître et décrire l'attitude d'un ou plusieurs corps dans l'espace, il est
nécessaire de les placer par rapport à un repère de référence.

### Repère de référence

Le repère `NED` (North-East-Down) est utilisé comme repère de référence,
avec un point de référence $O$ et une base pointant les directions Nord-Est-Bas.
Il sert à exprimer les déplacements des corps de la simulation.

### Repère navire

Le repère navire correspond au repère attaché au navire lors de la simulation.
Le point de référence de ce repère correspond généralement au centre de carène du navire.
Les axes du repère navire sont les suivants
- $X$ vers l'avant
- $Y$ sur tribord
- $Z$ vers le bas

![](images/ShipFrame.svg "Repère navire")

## Attitude navire

L'attitude d'un corps permet de connaître sa position et son orientation par rapport à un repère.
La position est donnée par le triplet $\left(X,Y,Z\right)$ et l'orientation
par un triplet de trois angles $\left(\phi,\theta,\psi\right)$.
Cette dernière peut être exprimée de manière différente notamment avec des quaternions.
Une attitude sera décrite de la manière suivante, avec les champs

- `frame` le nom du repère dans laquelle l'attitude est exprimée,
- `x` ,`y` ,`z`: le triplet de position où chaque position est
   définie par le dictionnaire des clés `value` et `unit`,
- `phi` ,`theta` ,`psi`, le triplet d'orientation où chaque angle
  est défini par le dictionnaire des clés `value` et `unit`,

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    frame: NED
    x: {value: 0, unit: m}
    y: {value: 0, unit: m}
    z: {value: 0, unit: m}
    phi: {value: 0, unit: rad}
    theta: {value: 0, unit: rad}
    psi: {value: 0, unit: rad}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Convention d'orientations

Cette section présente les notations utilisées pour définir l'orientation
d'un élément dans l'espace à partir d'un triplet de trois angles.

Si on souhaite utiliser classiquement la convention des angles aéronautiques
exprimée par le triplet (Roulis, Tangage, Lacet) alors on utilisera
les entrées suivantes. Si on souhaite paramétrer cette convention, il
est conseillé de lire la section complète.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    rotations:
        order by: angle
        convention: [z,y',x'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
  Il permet de définir complétement la composition de rotations.

Si on choisit une convention d'angles, alors chaque angle du triplet définit
respectivement une rotation autour d'un axe $X$, $Y$ ou $Z$.
Les axes ne peuvent être répetés.
Il est possible de définir 6 conventions d'angles, qui correspondent à
la permutation des trois axes: $XYZ$ ,$XZY$ ,$YXZ$ ,$YZX$ ,$ZXY$ ,$ZYX$.
Par exemple la rotation $R_{YZX}$ appliquée au triplet $\left(\phi,\theta,\psi\right)$
s'interprétera comme une rotation de $R_{Y}\left(\theta\right)$,
suivie de la rotation $R_{Z}\left(\psi\right)$, et terminée par la
rotation $R_{X}\left(\phi\right)$.

Si on choisit une convention d'axes, alors on modifie l'ordre des axes
sur lesquels appliquer successivement les rotations.
Des répétitions des axes sont alors possibles, si elles ne se suivent pas.
Par exempe, $XYX$ sera valide, mais pas $XXY$.
Par exemple, une convention ZXY définit une composition de rotations
Il est possible de définir 12 conventions d'axes:
$XYX$, $XYZ$, $XZX$, $XZY$, $YXY$, $YXZ$, $YZX$, $YZY$, $ZXY$, $ZXZ$, $ZYX$, $ZYZ$
Par exemple la rotation $R_{YZX}$ appliquée au triplet $\left(\phi,\theta,\psi\right)$
s'interprétera comme une rotation de $R_{Y}\left(\phi\right)$,
suivie de la rotation $R_{Z}\left(\theta\right)$, et terminée par la
rotation $R_{X}\left(\psi\right)$.

Avec ces conventions d'angles et d'axes, il existe déjà 18 combinaisons.
Ce nombre est doublé du fait que la composition de rotations peut être interne
(intrinsic) ou externe(extrinsic).
Si les rotations sont composées par rapport au repère fixe, on parle de composition externe.
Si les rotations sont composées par rapport aux repères nouvellement créés, on parle de composition interne.
C'est cette dernière qui est utilisée dans la majorité des cas.
Au total, ce sont ainsi 36 conventions qu'il est possible définir.

L'information de convention d'angles ou d'axes est stockée dans la variable `order by`.
Les informations de composition interne/externe et d'ordre des rotations
sont stockées dans la variable `convention`.
Des apostrophes sont utilisés pour indiquer des compositions de rotations
par rapport au nouveau système d'axes, et donc une composition interne.
Ainsi $X Y' Z''$ désignera une rotation autour X, suivie d'une rotation autour du
nouvel axe Y, appelé Y' et terminée par une rotation autour du nouvel axe Z, appelé Z''.
La double apostrophe fait référence au deuxième repère utilisé pour la composition
de rotation.

Le tableau suivant présente ces 36 conventions:

| id| Order | Convention  | Composition | Matrice de rotation                | Remarques                |
| :-| :-----| :-----------| :-----------| :----------------------------------| :------------------------|
|  1|angle  | x y z       | Extrinsic   | $R_Z(\psi).R_Y(\theta).R_X(\phi)$  |                          |
|  2|angle  | x z y       | Extrinsic   | $R_Y(\theta).R_Z(\psi).R_X(\phi)$  |                          |
|  3|angle  | y x z       | Extrinsic   | $R_Z(\psi).R_X(\phi).R_Y(\theta)$  |                          |
|  4|angle  | y z x       | Extrinsic   | $R_X(\phi).R_Z(\psi).R_Y(\theta)$  |                          |
|  5|angle  | z x y       | Extrinsic   | $R_Y(\theta).R_X(\phi).R_Z(\psi)$  |                          |
|  6|angle  | z y x       | Extrinsic   | $R_X(\phi).R_Y(\theta).R_Z(\psi)$  |                          |
|  7|angle  | x y' z''    | Intrinsic   | $R_X(\phi).R_Y(\theta).R_Z(\psi)$  |                          |
|  8|angle  | x z' x''    | Intrinsic   | $R_X(\phi).R_Z(\psi).R_Y(\theta)$  |                          |
|  9|angle  | y x' z''    | Intrinsic   | $R_Y(\theta).R_X(\phi).R_Z(\psi)$  |                          |
| 10|angle  | y z' x''    | Intrinsic   | $R_Y(\theta).R_Z(\psi).R_X(\phi)$  |                          |
| 11|angle  | z x' y''    | Intrinsic   | $R_Z(\psi).R_X(\phi).R_Y(\theta)$  |                          |
| 12|angle  | z y' x''    | Intrinsic   | $R_Z(\psi).R_Y(\theta).R_X(\phi)$  |                          |
| 13|axis   | x y x       | Extrinsic   | $R_X(\psi).R_Y(\theta).R_X(\phi)$  | Euler                    |
| 14|axis   | x y z       | Extrinsic   | $R_Z(\psi).R_Y(\theta).R_X(\phi)$  | Cardan - Tait - Bryan    |
| 15|axis   | x z x       | Extrinsic   | $R_X(\psi).R_Z(\theta).R_X(\phi)$  | Euler                    |
| 16|axis   | x z y       | Extrinsic   | $R_Y(\psi).R_Z(\theta).R_X(\phi)$  | Cardan - Tait - Bryan    |
| 17|axis   | y x y       | Extrinsic   | $R_Y(\psi).R_X(\theta).R_Y(\phi)$  | Euler                    |
| 18|axis   | y x z       | Extrinsic   | $R_Z(\psi).R_X(\theta).R_Y(\phi)$  | Cardan - Tait - Bryan    |
| 19|axis   | y z x       | Extrinsic   | $R_X(\psi).R_Z(\theta).R_Y(\phi)$  | Euler                    |
| 20|axis   | y z y       | Extrinsic   | $R_Y(\psi).R_Z(\theta).R_Y(\phi)$  | Cardan - Tait - Bryan    |
| 21|axis   | z x y       | Extrinsic   | $R_Y(\psi).R_X(\theta).R_Z(\phi)$  | Euler                    |
| 22|axis   | z x z       | Extrinsic   | $R_Z(\psi).R_X(\theta).R_Z(\phi)$  | Cardan - Tait - Bryan    |
| 23|axis   | z y x       | Extrinsic   | $R_X(\psi).R_Y(\theta).R_Z(\phi)$  | Euler                    |
| 24|axis   | z y z       | Extrinsic   | $R_Z(\psi).R_Y(\theta).R_Z(\phi)$  | Cardan - Tait - Bryan    |
| 25|axis   | x y' x''    | Intrinsic   | $R_X(\phi).R_Y(\theta).R_X(\psi)$  | Euler                    |
| 26|axis   | x y' z''    | Intrinsic   | $R_X(\phi).R_Y(\theta).R_Z(\psi)$  | Cardan - Tait - Bryan    |
| 27|axis   | x z' x''    | Intrinsic   | $R_X(\phi).R_Z(\theta).R_X(\psi)$  | Euler                    |
| 28|axis   | x z' y''    | Intrinsic   | $R_X(\phi).R_Z(\theta).R_Y(\psi)$  | Cardan - Tait - Bryan    |
| 29|axis   | y x' y''    | Intrinsic   | $R_Y(\phi).R_X(\theta).R_Y(\psi)$  | Euler                    |
| 30|axis   | y x' z''    | Intrinsic   | $R_Y(\phi).R_X(\theta).R_Z(\psi)$  | Cardan - Tait - Bryan    |
| 31|axis   | y z' x''    | Intrinsic   | $R_Y(\phi).R_Z(\theta).R_X(\psi)$  | Euler                    |
| 32|axis   | y z' y''    | Intrinsic   | $R_Y(\phi).R_Z(\theta).R_Y(\psi)$  | Cardan - Tait - Bryan    |
| 33|axis   | z x' y''    | Intrinsic   | $R_Z(\phi).R_X(\theta).R_Y(\psi)$  | Euler                    |
| 34|axis   | z x' z''    | Intrinsic   | $R_Z(\phi).R_X(\theta).R_Z(\psi)$  | Cardan - Tait - Bryan    |
| 35|axis   | z y' x''    | Intrinsic   | $R_Z(\phi).R_Y(\theta).R_X(\psi)$  | Euler                    |
| 36|axis   | z y' z''    | Intrinsic   | $R_Z(\phi).R_Y(\theta).R_Z(\psi)$  | Cardan - Tait - Bryan    |

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

### Conventions couramment utilisées

Parmi l'ensemble des conventions possibles, certaines sont plus utilisées que d'autres.

La convention des angles aéronautiques (convention de type 2 de la norme AFNOR)
exprimée par le triplet (Roulis, Tangage, Lacet) régulièrement utilisée s'écrit (id=12)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    rotations:
        order by: angle
        convention: [z,y',x'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Elle se comprend de la manière suivante, on effectue une rotation de
l'angle de lacet autour de l'axe $Z$, suivie d'une rotation de
l'angle d'assiette autour du nouvel axe $Y'$ suivie d'une rotation de
l'angle de roulis autour du nouvel axe $X''$.

Si on exprime ce triplet de la manière suivante (Lacet, Roulis, Tangage),
l'orientation sera décrite comme qui suit (id=33)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    rotations:
        order by: axis
        convention: [z,y',x'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La composition de rotations de ces deux conventions est
représentée sur la figure suivante
![](images/AnglesEuler.svg "Composition de rotations [z,y',x'']")


La convention d'orientation utilisée dans le logiciel
[Paraview](http://www.paraview.org) est la suivante (id=11)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    rotations:
        order by: angle
        convention: [z,x',y'']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Cette composition de rotation se comprend comme une rotation $\psi$ autour de l'axe $Z$,
suivie d'une rotation $\theta$ autour du nouvel axe $X'$ et finalement d'une rotation $\phi$
autour du nouvel axe $Y''$.

## Environnement

La section `environment` définit les modèles d'environnement pour la simulation à effectuer.
Elle permet de prendre en compte des modèles de houle et de vent.
En l'absence de ces modèles, les champs `model: no waves` et `model: no wind` pourront être utilisés.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    environment:
      - model: no waves
        frame: NED
        z: {value: 0, unit: m}
      - model: no wind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Définition d'un maillage de navire

Deux champs sont nécessaires pour définir un maillage servant à l'évaluation des efforts hydrostatiques nonlinéaires

- `mesh` qui contient le nom du fichier [STL](http://fr.wikipedia.org/wiki/Fichier_de_st%C3%A9r%C3%A9olithographie)
   contenant le maillage surfacique du navire au format ASCII
- `position of body frame relative to mesh` qui définit l'attitude relative du centre

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    mesh: anthineas.stl
    position of body frame relative to mesh:
        frame: mesh
        x: {value: 10, unit: m}
        y: {value: 0, unit: m}
        z: {value: 0, unit: m}
        phi: {value: 0, unit: rad}
        theta: {value: 0, unit: rad}
        psi: {value: 0, unit: rad}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Définition des objets à simuler

La section `bodies` permet de définir l'ensemble des caractéristiques du corps dont on souhaite simuler le comportement.
Il faut au moins un élément dans cette section pour effectuer une simulation.
Pour chacun de ces éléments, différents champs sont à renseigner

- `name` le nom du corps
- `mesh` le nom du fichier de maillage surfacique
- `position of body frame relative to mesh` l'attitude exprimée dans le repère NED
- `initial position of body frame relative to NED` l'attitude initiale exprimée dans le repère NED
- `initial velocity of body frame relative to NED` les vitesses initiales exprimées dans le repère NED
- `dynamics` l'ensemble des éléments dynamiques décrivant le corps
- `external forces` l'ensemble des torseurs d'efforts s'appliquant sur le corps

### Champs `dynamics`
- `centre of inertia` 
- `mass` contenant la masse du corps considéré
- `inertia matrix divided by total mass at the center of gravity projected in the body frame`

## Gestion des degrés de libertés

Le champs `blocked degrees of freedom body/NED->body:` permet
de gérer les degrés de liberté lors de la simuation.

## Gestion des données exportées

Le simulateur permet d'exporter l'ensemble des grandeurs manipulées lors de la simulation, à savoir

- `positions`: les positions 
- `angles`: les angles 
- `linear velocities`: les vitesses linéaires de points
- `angular velocities`: les vitesses angulaires
- `linear accelerations`: les accélérations linéaires de points
- `angular accelerations`: les accélérations angulaires
- `forces and torques`: les torseurs d'efforts

## Définition de points

La section `points` permet de créer un ou plusieurs points définis
par un nom et une position dans un repère.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    points:
    - name: P
      frame: body 1
      x: {value: 10, unit: m}
      y: {value: 10, unit: m}
      z: {value: 20, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Exemple complet

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    rotations:
        order by: angle
        convention: [x,y',z'']
    environment:
      - model: no waves
        frame: NED
        z: {value: 0, unit: m}
      - model: no wind
    # Fixed frame: NED
    bodies: # All bodies have NED as parent frame
      - name: body 1
        mesh: anthineas.stl
        position of body frame relative to mesh:
            frame: mesh
            x: {value: 10, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0, unit: m}
            phi: {value: 0, unit: rad}
            theta: {value: 0, unit: rad}
            psi: {value: 0, unit: rad}
        initial position of body frame relative to NED:
            frame: NED
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0, unit: m}
            phi: {value: 0, unit: rad}
            theta: {value: 0, unit: rad}
            psi: {value: 0, unit: rad}
        initial velocity of body frame relative to NED:
            frame: body
            u: {value: 0, unit: m/s}
            v: {value: 0, unit: m/s}
            w: {value: 0, unit: m/s}
            p: {value: 0, unit: rad/s}
            q: {value: 0, unit: rad/s}
            r: {value: 0, unit: rad/s}
        dynamics:
            centre of inertia:
                frame: body 1
                x: {value: 0, unit: m}
                y: {value: 0, unit: m}
                z: {value: 0, unit: m}
            mass: {value: 1000, unit: tonne} # Caution: 'ton' is the british ton which is 907.185 kg
            inertia matrix divided by total mass at the center of gravity projected in the body frame:
                frame: body 1
                row 1: [1,0,0,0,0,0]
                row 2: [0,1,0,0,0,0]
                row 3: [0,0,1,0,0,0]
                row 4: [0,0,0,1,0,0]
                row 5: [0,0,0,0,1,0]
                row 6: [0,0,0,0,0,1]
        external forces:
          - model: gravity
            g: {value: 9.81, unit: m/s^2}
          - model: non-linear hydrostatic
            rho: {value: 1000, unit: kg/m^3}
    points:
      - name: P
        frame: body 1
        x: {value: 10, unit: m}
        y: {value: 10, unit: m}
        z: {value: 20, unit: m}
      - name: O
        frame: NED
        x: {value: 0, unit: m}
        y: {value: 0, unit: m}
        z: {value: 0, unit: m}
    blocked degrees of freedom body/NED->body:
       - body: body 1
         blocked: [x, theta]
    outputs:
        positions:
          - point: O
            relative to frame: NED
            projected in frame: body 1
            axes: [x,z]
        angles:
          - frame: body 1
            relative to frame: NED
            projected in frame: body 1
            axes: [x,y,z]
        linear velocities:
          - point: P
            relative to frame: NED
            projected in frame: body 1
            axes: [x,y]
          - point: P
            relative to frame: NED
            projected in frame: NED
            axes: [z]
        angular velocities:
          - frame: body 1
            relative to frame: NED
            projected in frame: body 1
            axes: [x,y]
        linear accelerations:
          - point: P
            relative to frame: NED
            projected in frame: body 1
            axes: [x,y]
          - point: P
            relative to frame: NED
            projected in frame: NED
            axes: [z]
        angular accelerations:
          - frame: body 1
            relative to frame: NED
            projected in frame: body 1
            axes: [x,y]
        forces and torques:
          - type: non-linear hydrostatic
            point name: centre of inertia
            point frame: body 1
            projected in frame: NED
            axes: [X, Z, K, N]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

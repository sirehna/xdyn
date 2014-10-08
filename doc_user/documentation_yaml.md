# Guide utilisateur

Le simulateur léger de comportement de navires a été développé par DCNS Research
pour l'IRT Jules Verne.

Ce simulateur permet de réaliser de nombreuses simulations au stade
d'avant-projet, permettant d'appréhender le comportement à la mer du navire muni
de ses  actionneurs, en incluant certains aspects non-linéaires de ce
comportement, et intégrant des fonctionnalités de manœuvrabilité.

# Documentations des données d'entrées du simulateur

Les données d'entrées du simulateur se basent sur un format
de fichiers [YAML](http://www.yaml.org/)

Ce format présente l'avantage d'être facilement lisible et éditable par
l'homme et facilement interprétable par la machine.

## Environnement

La section `environment` définit les modèles d'environnement pour la simulation
à effectuer. Elle permet de prendre en compte des modèles de houle et de vent.
En l'absence de ces modèles, les champs `model: no waves` et `model: no wind`
pourront être utilisés.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    environment:
      - model: no waves
        frame: NED
        z: {value: 0, unit: m}
      - model: no wind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Définition d'un maillage de navire

Deux champs sont nécessaires pour définir un maillage servant à l'évaluation des
efforts hydrostatiques nonlinéaires.

- `mesh` qui contient le nom du fichier
   [STL](http://fr.wikipedia.org/wiki/Fichier_de_st%C3%A9r%C3%A9olithographie)
   contenant le maillage surfacique du navire au format ASCII
- `position of body frame relative to mesh` qui définit l'attitude

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

La section `bodies` permet de définir l'ensemble des caractéristiques du corps
dont on souhaite simuler le comportement.
Il faut au moins un élément dans cette section pour effectuer une simulation.
Pour chacun de ces éléments, différents champs sont à renseigner

- `name` le nom du corps
- `mesh` le nom du fichier de maillage surfacique
- `position of body frame relative to mesh` l'attitude exprimée dans le repère
   NED
- `initial position of body frame relative to NED` l'attitude initiale exprimée
   dans le repère NED
- `initial velocity of body frame relative to NED` les vitesses initiales
   exprimées dans le repère NED
- `dynamics` l'ensemble des éléments dynamiques décrivant le corps
- `external forces` l'ensemble des torseurs d'efforts s'appliquant sur le corps

### Champs `dynamics`

Le champs `dynamics` est composé de trois éléments:

- `centre of inertia`
- `mass` contenant la masse du corps considéré
- `inertia matrix divided by total mass at the center of gravity projected in the body frame`

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    dynamics:
        centre of inertia:
            frame: body 1
            x: {value: 0, unit: m}
            y: {value: 0, unit: m}
            z: {value: 0, unit: m}
        mass: {value: 1000, unit: tonne}
        inertia matrix divided by total mass at the center of gravity projected in the body frame:
            frame: body 1
            row 1: [1,0,0,0,0,0]
            row 2: [0,1,0,0,0,0]
            row 3: [0,0,1,0,0,0]
            row 4: [0,0,0,1,0,0]
            row 5: [0,0,0,0,1,0]
            row 6: [0,0,0,0,0,1]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Gestion des degrés de libertés

Le champs `blocked degrees of freedom body/NED->body:` permet
de gérer les degrés de liberté lors de la simuation.

## Gestion des données exportées

Le simulateur permet d'exporter l'ensemble des grandeurs manipulées lors de la
simulation, à savoir:

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

# Tutoriel 2 : oscillations en immersion

Ce tutoriel vise à illustrer l'utilisation des modèles hydrostatiques et à
comparer succinctement les modèles `non-linear hydrostatic (exact)` et
`non-linear hydrostatic (fast)`.

## Description du problème
Dans cet example, nous considérons un navire,
(l'Anthineas)[http://www.marinetraffic.com/en/ais/details/ships/228367000/vessel:ANTHINEAS],
soumis uniquement à la pesanteur et aux efforts hydrostatiques, sans
amortissement. Le navire est lâché sans vitesse initiale au-dessus de la
surface libre (supposée plane) et va donc réaliser des oscillations
non-amorties en immersion.

## Ecriture du fichier de configuration du simulateur.

Nous documentons ici uniquement les changements par rapport au
[tutoriel 1](tutoriels.html#tutoriel-1-balle-en-chute-libre).

L'environnement est défini de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
environment models:
  - model: no waves
    constant sea elevation in NED frame: {value: 0, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme décrit dans le [documentation du fichier
d'entrée](documentation_yaml.html#absence-de-houle), ceci signifie que la
surface libre est parfaitement plane et horizontale, à la hauteur $z=0$ dans le
repère NED.

Par rapport au [tutoriel 1](tutorials.html#tutoriel-1-balle-en-chute-libre), la
position du repère "body" par rapport au maillage est ici importante puisque
l'on [fournit un fichier STL](documentation_yaml.html#d%C3%A9finition-du-fichier-de-maillage) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On décrit dans les conditions initiales le fait que le bateau est lâché à 5 m
au-dessus du niveau de l'eau (l'axe z du repère NED étant orienté vers le bas,
des valeurs négatives correspondent à des points au-dessus de la surface libre)
:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les données dynamiques comprennent la masse, la matrice d'inertie, les inerties ajoutées
et la position du centre d'inertie :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

On utilise dans un premier temps le [modèle hydrostatique
approché](modeles_reperes_et_conventions.html#calcul-du-moment) dont la
documentation est décrite
[ici](documentation_yaml.html#hydrostatique-non-lin%C3%A9aire) :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
  - model: gravity
  - model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En définitive, on obtient le fichier suivant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
rotations:
    order by: angle
    convention: [z,y',x'']

environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1025, unit: kg/m^3}
environment models:
  - model: no waves
    constant sea elevation in NED frame: {value: 0, unit: m}

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_z" 0 3
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_z" 0 3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Résultats

Voici les résultats avec le [modèle hydrostatique
approché](modeles_reperes_et_conventions.html#calcul-du-moment) on obtient :

![Elévation au cours du temps](images/tutorial_02_fast_hydrostatic_z.svg)

Pour utiliser le [modèle hydrostatique
précis](modeles_reperes_et_conventions.html#calcul-du-moment), il suffit de
modifier la section `external forces` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
  - model: gravity
  - model: non-linear hydrostatic (exact)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

![Elévation au cours du temps](images/tutorial_02_exact_hydrostatic_z.svg)

On peut également représenter les déplacements suivant l'axe $y$ en exécutant :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.bash}
./sim tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_fast_hydrostatic_y" 0 2
./sim tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 | python plot.py "tutorial_02_exact_hydrostatic_y" 0 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

![Embardée (modèle rapide)](images/tutorial_02_fast_hydrostatic_y.svg)
![Embardée (modèle précis)](images/tutorial_02_exact_hydrostatic_y.svg)

# Modèles d'environnement

La section `environment` définit les modèles d'environnement pour la simulation
à effectuer. Elle permet de prendre en compte des modèles de houle et de vent.
Elle peut être vide (#tutoriel-1-balle-en-chute-libre)).

Les modèles de houle interviennent pour le calcul des [efforts
hydrostatiques](#efforts-hydrostatiques-non-lin%C3%A9aires)
(par truchement de l'élévation de la surface libre),
d'une part, et les [efforts de Froude-Krylov](#calcul-des-efforts-dexcitation)
d'autre part (par le biais de la pression dynamique).


## Constantes environnementales

L'accélération de la pesanteur (dénotée par `g`) et la densité volumique de
l'eau (`rho`) sont des
constantes qui interviennent dans plusieurs modèles physiques. Par conséquent,
plutôt que d'être renseignées au niveau de chaque modèle et risquer ainsi des
incohérences, elles figurent dans la section `environmental constants` qui a la
forme suivante :

~~~~~~~~~~~~~~ {.yaml}
environmental constants:
    g: {value: 9.81, unit: m/s^2}
    rho: {value: 1025, unit: kg/m^3}
~~~~~~~~~~~~~~

Comme expliqué dans [une section
précédente](#remarques-sur-les-unit%C3%A9s), les
dimensions physiques ne sont pas vérifiées et simplement converties en unités
du système international.


## Simulation sans houle

Pour simuler une surface libre parfaitement plane, on opère de la façon
suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
    environment:
      - model: no waves
        constant sea elevation in NED frame: {value: 0, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`model: no waves` indique que l'on souhaite une surface libre horizontale et
`constant sea elevation in NED frame` représente l'élévation de la surface
libre dans le repère NED.

Dans ce cas, les efforts d'excitation (Froude-Krylov et radiation) seront nuls.

## Houle d'Airy

On peut définir une houle comme étant une somme de plusieurs spectres
directionnels, c'est-à-dire un spectre de puissance et une dispersion spatiale.
Pour dériver l'expression générale d'une houle composée de plusieurs spectres,
on commence par le cas d'une houle monochromatique et monodirectionnelle.

### Expression du potentiel de vitesse de la houle

Soit $V(x,y,z,t)=(u,v,w)$ la vitesse du fluide au point de coordonnées $(x,y,z)$
(dans le repère NED) et à l'instant $t$.

On suppose l'eau non visqueuse, incompressible, homogène et isotrope et l'on
considère un écoulement irrotationnel.
Supposer l'écoulement irrotationnel implique (d'après le lemme
de Poincaré) que la vitesse dérive d'un potentiel que l'on appelle
$\phi:(x,y,z,t)\mapsto\phi(x,y,z,t)$. Par définition, la vitesse en tout
point de l'écoulement est donc donnée par :

$$V(x,y,z,t) = \textrm{grad}{\phi(x,y,z,t)}$$

La pression $p$ vérifie l'équation de Bernoulli :

$$p + \rho g z -\rho\frac{\partial\phi}{\partial t} +
\frac{\rho}{2} V\cdot V = C(t)$$

où $C:t\mapsto C(t)$ est une fonction du temps arbitraire, donc en particulier
$C(t)=p_0$ (pression atmosphérique à la surface) :

$$p_0 + \rho g z -\rho\frac{\partial\phi}{\partial t} +
\frac{\rho}{2} V\cdot V = p_0$$

soit

$$g z -\frac{\partial\phi}{\partial t} + \frac{1}{2} V\cdot V = 0$$

Il s'agit de la première condition de surface libre.

On peut définir la fonction $F(x,y,z,t)=z-\eta(x,y,t)$

Pour une particule sur la surface libre, $F(x,y,z,t)=0$ ce qui implique que sa
dérivéee particulaire est nulle :

$$\frac{DF}{Dt} = \frac{\partial F}{\partial t} + V\cdot \nabla F = 0$$

soit

$\frac{\partial\eta}{\partial t} + \frac{\phi}{\partial x}\frac{\partial
\eta}{\partial x} + \frac{\phi}{\partial x}\frac{\partial\eta}{\partial y} -
\frac{\partial\phi}{\partial z} = 0$ sur $z=\eta(x,y,t)$.

C'est la deuxième condition de surface libre.

En linéarisant ces deux conditions de surface libre, on obtient :

$$\frac{\partial \eta}{\partial t} = \frac{\partial\phi}{\partial z}$$
$$g\eta - \frac{\partial\phi}{\partial t} = 0$$

Par ailleurs, l'eau étant supposée incompressible, $$\nabla\cdot V=
\frac{\partial^2\phi}{\partial x^2} +  \frac{\partial^2\phi}{\partial y^2} +
\frac{\partial^2\phi}{\partial z^2} = 0$$

Il s'agit d'une équation de Laplace dont la solution s'obtient par la méthode de
séparation des variables :

$$\phi(x,y,z,t) = \frac{g\eta_a}{\omega}\frac{\cosh(k\cdot(h-z))}
{\cosh(k\cdot h)}\cos(k\cdot(x\cdot
\cos(\gamma)+ y\cdot \sin(\gamma))-\omega\cdot t+\phi)$$

$h$ est la profondeur du fluide (hauteur du sol à la surface libre)
$\eta_a=2\sqrt{S(\omega)}$ est l'amplitude (en m)
$x,y,z$ sont les coordonnées du point considéré, exprimées dans le repère NED.
$k$ est le nombre d'onde, traduisant la périodicité spatiale et vérifie la
relation de dispersion :

$$\omega^2 = g\cdot k \cdot \tanh(k\cdot h)$$

qui, en profondeur infinie ($k\cdot h > 3$) tend vers :

$$\omega^2 \sim g\cdot k$$

Le potentiel de vitesse de la houle est ici exprimé pour une seule fréquence et
une seule direction. On peut la généraliser en :

$$\phi(x,y,z,t) = \sum_{i=1}^{nfreq}\sum_{j=1}^{ndir} \sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}\cdot \frac{g}{\omega_i}\frac{\cosh(k\cdot(h-z))}
{\cosh(k_i\cdot h)}\cos(k_i\cdot(x\cdot
\cos(\gamma_j)+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi)$$

### Elévation de la houle

L'élévation de la houle est donnée par la deuxième condition de surface libre :

$$\eta(x,y,t) = \frac{1}{g}\frac{\partial\phi}{\partial t} = -
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}
\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}\sin(k_i\cdot(x\cdot \cos(\gamma_j)
+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$

### Pression dynamique

L'expression de la pression dynamique, utilisée par le modèle de
[Froude-Krylov](#calcul-des-efforts-dexcitation), se déduit de la première
condition de surface libre linéarisée :

$$p_{\textrm{dyn}} = -\rho g z = -\rho \frac{\partial \Phi(x,y,z,t)}{\partial t}$$

soit

$$p_{\textrm{dyn}} = \rho\cdot g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}
\frac{\cosh(k_i\cdot(h-z))}{\cosh(k_i\cdot h)}\cos(k_i\cdot(x\cdot
\sin(\gamma_j)+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$

- $g$ désigne l'accélération de la pesanteur (9.81 $m/s^2$)
- $\rho$ est la densité volumique du fluide (en $kg/m^3$)

Lorsque la profondeur $h$ est très grande devant $z$, on obtient :

$$p_{\textrm{dyn}} = \rho\cdot g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}
e^{-k_i\cdot z}\cos(k_i\cdot(x\cdot\cos(\gamma_j)+ y\cdot
\sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$


### Vitesse orbitale

La vitesse $V(x,y,z,t) = (u,v,w)$ orbitale de la houle est définie par :

$$u = \frac{\partial \phi}{\partial x} = g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\frac{k_i}{\omega_i}
\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}
\frac{\cosh(k\cdot(h-z))}{\cosh(k\cdot h)}\cdot\cos(\gamma_j)
\sin(k\cdot(x\cdot \cos(\gamma_j)+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$
$$v = \frac{\partial \phi}{\partial y} = g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\frac{k_i}{\omega_i}
\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}
\frac{\cosh(k\cdot(h-z))}{\cosh(k\cdot h)}\cdot\sin(\gamma_j)
\sin(k\cdot(x\cdot \cos(\gamma_j)+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$
$$w = \frac{\partial \phi}{\partial z} = g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\frac{k_i}{\omega_i}
\sqrt{A(\omega_i,\gamma_j)\Delta\omega\Delta\gamma}
\frac{\sinh(k\cdot(h-z))}{\cosh(k\cdot h)}
\cos(k\cdot(x\cdot \cos(\gamma_j)+ y\cdot \sin(\gamma_j))-\omega_i\cdot t+\phi_{i,j})$$



### Paramétrisation des modèles de houle

Les spectres directionnels de houle d'Airy sont paramétrés de la façon suivante :

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
     Tp: {value: 15, unit: s}
     gamma: 1.2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `model` : actuellement, ne peut valoir qu'`airy`.
- `depth` : profondeur (distance entre le fond et la surface). 0 pour
l'approximation "profondeur infinie". Utilisé pour le calcul du nombre d'onde.
- `seed of the random data generator` : germe utilisé pour la génération des
phases aléatoires.
- `directional spreading` : étalement directionnel. Cf. infra.
- `spectral density` : densité spectrale de puissance. Cf. infra.



## Densités spectrales de puissance

### Dirac

La plus simple densité spectrale de puissance correspond à une houle
monochromatique, c'est-à-dire à une seule fonction sinusoïdale :

$$\omega_0\in\mathbb{R}^+,\forall \omega\in\mathbb{R}^+, S(\omega) =
\left\{\begin{array}{l}0, \textrm{si }\omega\neq \omega_0\\1, \textrm{si }
\omega=\omega_0\end{array}\right.$$

Le paramétrage de ce spectre est :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
    type: dirac
    Hs: {value: 5, unit: m}
    omega0: {value: 15, unit: rad/s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La hauteur de houle est donnée par `Hs` et sa pulsation par `omega0`.

### JONSWAP

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
    type: jonswap
    Hs: {value: 5, unit: m}
    Tp: {value: 15, unit: s}
    gamma: 1.2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Pierson-Moskowitz

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
    type: pierson-moskowitz
    Hs: {value: 5, unit: m}
    Tp: {value: 15, unit: s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Bretschneider

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
    type: bretschneider
    Hs: {value: 5, unit: m}
    Tp: {value: 15, unit: s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Étalements directionnels

### Dirac

Lorsque cet étalement est choisi, la houle est mono-directionnelle.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
    type: dirac
    waves propagating to: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La direction de propagation est donnée par `waves propagating to`.

### Cos2s

L'étalement est donné par :
$$\gamma\mapsto \cos^{2s}\left({\gamma-\gamma_0}\right)$$

où $\gamma_0$ est la direction de propagation.

Cet étalement est paramétré de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
    type: cos2s
    s: 2
    waves propagating to: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`waves propagating to` donne la direction de propagation $\gamma_0$.



## Discrétisation des spectres et des étalements

Les étalements et les spectres présentés précédemment sont continus. Afin d'en
réaliser l'implémentation informatique, il faut les discrétiser. Si l'on
répartit uniformément les pulsations sur un intervalle, on introduit une
périodicité temporelle de la houle (cela revient en effet à effectuer une
transformée de Fourier inverse, qui donne par construction un résultat
périodique). Afin d'être plus représentatif des états de mers réels, on peut
souhaiter rompre cette périodicité en discrétisant les pulsations de manière
aléatoire. On obtient ainsi un signal apériodique.

La performance de l'implémentation des modèles de houle est cruciale : en
effet, la pression dynamique et la pression statique étant intégrées sur toutes
les facettes du maillage, ces modèles sont évalués de nombreuses fois par pas
de calcul. Comme le nombre de composantes sommées pour calculer les élévations
et pressions dynamiques étant potentiellement important, on ne sélectionne
que les produits $S(\omega_i)D(\gamma_j)$ contribuant de manière significative
à l'énergie totale.
Pour ce faire, on classe ces produits par ordre décroissant et l'on sélectionne
les $n$ premiers de façon à ce que leur somme représente une fraction
prédéterminée de la puissance totale. De cette manière, on réduit
considérablement les temps de calcul, tout en gardant une bonne
représentativité de la physique du problème.

La discrétisation est paramétrée de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
discretization:
    n: 128
    omega min: {value: 0.1, unit: rad/s}
    omega max: {value: 6, unit: rad/s}
    energy fraction: 0.999
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `n` : nombre de points (nombre de fréquences ou nombre de directions)
- `omega min` : pulsation minimale (incluse)
- `omega max` : pulsation maximale (incluse)
- `energy fraction` : les produits de spectre de puissance et d'étalement
directionnel $S_i\cdot D_j$ sont classés par ordre décroissant. On calcule la
somme cumulative et l'on s'arrête lorsque l'énergie accumulée vaut `energy
fraction` de l'énergie totale.

## Sorties

On peut sortir les hauteurs de houle calculées sur un maillage (défini dans un
repère fixe ou mobile). En fait, on peut même choisir de ne faire qu'une
simulation de houle, sans corps, tel que décrit dans le [tutoriel 3](#tutoriel-3-g%C3%A9n%C3%A9ration-de-houle-sur-un-maillage).

On définit un maillage (cartésien) sur lequel sera calculé la houle (dans la
section `environement/model/output`). Par exemple :

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

- `frame of reference` : nom du repère dans lequel sont exprimées les
  coordonnées des points du maillage.
- `xmin`, `xmax`, `nx` : définition de la discrétisation de l'axe x. Les
  valeurs vont de `xmin` (inclus) à `xmax` (inclus) et il y a `nx` valeurs au
  total.
- `ymin`, `ymax`, `ny` : comme pour x.

Dans l'exemple précédent, les coordonnées sont données dans le repère NED. Le
maillage comporte 10 points :
(1,1),(1,2),(2,1),(2,2),(3,1),(3,2),(4,1),(4,2),(5,1),(5,2).

Les sorties sont écrites dans le fichier et le format spécifiés dans la
section [`output`](#sorties).


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

## Références
- *Environmental Conditions and Environmental Loads*, April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
- *Hydrodynamique des Structures Offshore*, 2002, Bernard Molin, Editions TECHNIP, ISBN 2-7108-0815-3, page 70
- *Sea Loads on Ships And Offshore Structures*, 1990, O. M. Faltinsen, Cambridge Ocean Technology Series, ISBN 0-521-37285-2, pages 27
- *Seakeeping: Ship Behaviour in Rough Weather*, 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, ISBN 0-7458-0230-3, page 75
- *Offshore Hydromechanics*, 2001, J.M.J. Journée and W.W. Massie, Delft University of Technology, sections 6-20 and 7-11

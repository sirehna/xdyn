# Modèles d'environnement

La section `environment` définit les modèles d'environnement pour la simulation
à effectuer. Elle permet de prendre en compte des modèles de houle et de vent.
Elle peut être vide (#tutoriel-1-balle-en-chute-libre)).

## Constantes environnementales

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
précédente](#remarques-sur-les-unit%C3%A9s), les
dimensions physiques ne sont pas vérifiées et simplement converties en unités
du système international.

## Modèles de houle

Les modèles de houle interviennent pour le calcul des [efforts
hydrostatiques](#efforts-hydrostatiques-non-lin%C3%A9aires)
(par truchement de l'élévation de la surface libre),
d'une part, et les [efforts de
Froude-Krylov](#calcul-des-efforts-dexcitation) d'autre part (par le biais de la
pression dynamique).

### Conventions

On note $\psi$ la direction de propagation de la houle (les vagues "vont vers"
$\psi$) dans le repère NED (en d'autres termes, $\psi=0$ correspond aux vagues
allant vers le Nord, $\psi = 90^\circ$ à des vagues allant vers l'Est). Cette
convention peut être illustrée par la figure suivante :

![](images/convention_houle.svg)

On appelle $\omega\mapsto S(\omega)$ la densité spectrale de puissance de la
houle, $\psi\mapsto D(\psi)$ l'étalement directionnel de la houle et $A(\omega,
\psi) = S(\omega)\cdot D(\psi)$ le spectre directionnel de la houle.

La densité spectrale de puissance donne la distribution (au sens des
probabilités) de la hauteur de houle
en fonction de la pulsation de celle-ci et l'étalement directionnel est une
caractérisation non-stochastique de la variation de la hauteur de houle dans
l'espace.

### Densités spectrales de puissance implémentées

#### Dirac
La plus simple densité spectrale de puissance correspond à une houle
monochromatique, c'est-à-dire à une seule fonction sinusoïdale :

$$\omega_0\in\mathbb{R}^+,\forall \omega\in\mathbb{R}^+, S(\omega) =
\left\{\begin{array}{l}0, \mbox{si }\omega\neq \omega_0\\1, \mbox{si
} \omega=\omega_0\end{array}\right\}$$

Le paramétrage de ce spectre est :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
   type: dirac
   Hs: {value: 5, unit: m}\n"
   omega0: {value: 15, unit: rad/s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La hauteur de houle est donnée par `Hs` et sa pulsation par `omega0`.

#### JONSWAP

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: jonswap
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
     gamma: 1.2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Pierson-Moskowitz

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: pierson-moskowitz
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Bretschneider

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
spectral density:
     type: bretschneider
     Hs: {value: 5, unit: m}
     Tp: {value: 15, unit: m}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Etalements directionnels

#### Dirac

Lorsque cet étalement est choisi, la houle est mono-directionnelle.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
     type: dirac
     waves propagating to: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La direction de propagation est donnée par `waves propagating to`.

#### Cos2s

L'étalement est donné par :
$$\psi\mapsto \cos^{2s}{\psi-\psi_0}$$

où $\psi_0$ est la direction de propagation.

Cet étalement est paramétré de la façon suivante :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
directional spreading:
   type: cos2s
   s: 2
   waves propagating to: {value: 90, unit: deg}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`waves propagating to` donne la direction de propagation $\psi_0$.


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

#### Houle d'Airy

On peut définir une houle comme étant une somme de plusieurs spectres
directionnels, c'est-à-dire un spectre de puissance et une dispersion spatiale.
Pour dériver l'expression générale d'une houle composée de plusieurs spectres,
on commence par le cas d'une houle monochromatique et monodirectionnelle, pour
laquelle le potentiel de vitesse s'exprime :

$$\phi(x,y,t) = \frac{\cosh(k\cdot(z+h))}{\cosh(k\cdot h)}\cos(k\cdot(x\cdot
\cos(\psi)+ y\cdot \sin(\psi))-\omega_i\cdot t+\phi)$$

$h$ est la profondeur du fluide (hauteur du sol à la surface libre)
$x,y,z$ sont les coordonnées du point considéré, exprimées dans le repère NED
$k$ est le nombre d'onde, traduisant la périodicité spatiale et vérifie la
relation de dispersion :

$$\omega^2 = g\cdot k \cdot \tanh(k\cdot h)$$

On peut généraliser et dériver cette expression pour obtenir l'élévation d'une
houle polychromatique et multi-directionnelle :

$$\eta(x,y,t) = \sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}
\sqrt{A(\omega_i,\psi_j)\Delta\omega\Delta\psi}\cos(k\cdot(x\cdot \cos(\psi_j)
+ y\cdot \sin(\psi_j))-\omega_i\cdot t+\phi_{i,j})$$

ainsi que l'expression de la pression dynamique, utilisée par le modèle de
[Froude-Krylov](#calcul-des-efforts-dexcitation)
:

$$p_{\mbox{dyn}} = \frac{\partial \Phi(x,y,z,t)}{\partial t}$$

soit

$$p_{\mbox{dyn}} = \rho\cdot g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\sqrt{A(\omega_i,\psi_j)\Delta\omega\Delta\psi}\frac{\cosh(k\cdot(z-\eta+h))}{\cosh(k\cdot
h)}\cos(k\cdot(x\cdot
\cos(\psi_j)+ y\cdot \sin(\psi_j))-\omega_i\cdot t+\phi_{i,j})$$

$g$ désigne l'accélération de la pesanteur (9.81 $m/s^2$)
$\rho$ est la densité volumique du fluide (en $kg/m^3$)

Lorsque la profondeur $h$ est très grande devant $z$, on obtient :

$$p_{\mbox{dyn}} = \rho\cdot g
\sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}\sqrt{A(\omega_i,\psi_j)\Delta\omega\Delta\psi}e^{-k\cdot
(z-\eta(x,y,t))}\cos(k\cdot(x\cdot\cos(\psi_j)+ y\cdot
\sin(\psi_j))-\omega_i\cdot t+\phi_{i,j})$$

Les spectres directionnels sont définis de la façon suivante :

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

- `model` : actuellement, ne peut valoir qu'`airy`.
- `depth` : profondeur (distance entre le fond et la surface). 0 pour
l'approximation "profondeur infinie". Utilisé pour le calcul du nombre d'onde.
- `seed of the random data generator` : germe utilisé pour la génération des
phases aléatoires.
- `directional spreading` : étalement directionnel. Cf. infra.
- `spectral density` : densité spectrale de puissance. Cf. infra.


#### Discrétisation des spectres et des étalements

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
de calcul. Comme le nombre de composantes sommées pour calculer les élévations et pressions
dynamiques étant potentiellement important, on ne sélectionne que les produits
$S(\omega_i)D(\psi_j)$ contribuant de manière significative à l'énergie totale.
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

- `n` : nombre de points (nombre de fréquences ou nombr de directions)
- `omega min` : pulsation minimale (incluse)
- `omega max` : pulsation maximale (incluse)
- `energy fraction` : les produits de spectre de puissance et d'étalement
directionnel $S_i\cdot D_j$ sont classés par ordre décroissant. On calcule la
somme cumulative et l'on s'arrête lorsque l'énergie accumulée vaut `energy
fraction` de l'énergie totale.

### Sorties

On peut sortir les hauteurs de houle calculées sur un maillage (défini dans un
repère fixe ou mobile). En fait, on peut même choisir de ne faire qu'une
simulation de houle, sans corps, tel que décrit dans le [tutoriel 3](#tutoriel-3-g%C3%A9n%C3%A9ration-de-houle-sur-un-maillage).

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


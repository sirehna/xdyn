# Modèles d'efforts non-commandés

Les efforts extérieurs (non commandés) sont donnés dans la section `external forces` sous forme
de liste de modèles ayant ou non des paramètres. La seule clef commune à tous
les modèles d'effort est `model` : chaque modèle possède sinon sa propre
paramétrisation (éventuellement aucune paramétrisation). Voici un exemple de
section `external forces` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
  - model: gravity
  - model: non-linear hydrostatic (fast)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Le navire est soumis aux efforts suivants :

- La pesanteur,
- Les efforts hydrostatiques, liés à la pression exercée par le fluide au repos
  sur la coque,
- Les efforts hydrodynamiques, provenant d'une part de l'excitation de la houle
  et d'autre par des phénomènes de radiation/diffraction,
- Les efforts d'amortissement visqueux, dûs aux frottements de l'eau sur la
  coque et aux tourbillons,


## Efforts de gravité

### Description

Le poids est donné dans le repère NED par :

$F_P = m \cdot g \mathbf{u_z}$

où $m$ désigne la masse du navire (en kg), $g$ l'accélération de la pesanteur
(en m/s^2) et $\mathbf{u_z}$ le vecteur unitaire vertical, exprimé dans le
repère dans lequel on veut projeter la force.

### Paramétrage

Pour soumettre un solide à la gravité, on écrit :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: gravity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La valeur de $g$ utilisée est celle définie dans la section [`environmental
constants`](#constantes-environnementales) et la masse est celle figurant dans
la section [`mass`](#d%C3%A9finition-de-la-masse-du-solide).

Un exemple de simulation de solide soumis uniquement à la gravité (chute libre)
est disponible dans les [tutoriels](#tutoriel-1-balle-en-chute-libre).

### Références
- *Physics v. 1*, 2001, D. Halliday, R. Resnick and K. S. Krane, John Wiley and Sons, ISBN 0-471-32057-9
- *Physique théorique*, Lev Landau et Evgueni Lifchits, éd. MIR, Moscou


## Efforts hydrostatiques non-linéaires

### Description

Les efforts hydrostatiques non-linéaires sont dus à la pression statique
(c'est-à-dire indépendante de la vitesse du fluide) s'exerçant sur la carène.
L'hypothèse principale est ici la staticité, c'est-à-dire qu'on considère la
carène au repos et une surface libre plane. Si cette dernière hypothèse n'est
pas vérifiée, il faut ajouter un terme correctif, qui correspond aux efforts
d'excitation de Froude-Krylov. En d'autres termes, pour le modèle
hydrostatique, il faut effectuer le calcul en supposant la surface libre au
repos. La prise en compte de la pression due à la houle est faite par le modèle
de Froude-Krylov.
La force $\textbf{F}_{\mbox{HS}}$ exercée par l'eau sur la carène doit être
calculée comme l'intégrale de la pression hydrostatique $p_{\mbox{HS}}$ sur
la surface immergée totale :

$$\textbf{F}_{\mbox{HS}} = \int_{S}p_{\mbox{HS}}(z)\cdot \textbf{n} dS $$

Le paramétrage des efforts hydrostatiques non-linéaires dans le simulateur est
décrit [ici](#).

Un exemple d'utilisation est présenté dans les
[tutoriels](#tutoriel-2-oscillations-en-immersion).

### Calcul de la résultante

Pour évaluer numériquement cette intégrale, il faut discrétiser la carène au
moyen d'un maillage surfacique. La définition de ce maillage est faite
[ici](#utilisation-dun-maillage).

Les facettes du maillage peuvent alors être réparties en trois catégories :

- les facettes totalement émergées : leur contribution est nulle
- les facettes totalement immergées : leur contribution est $\rho\cdot g\cdot
  dS z_G \textbf{n}$ où $z_G$ désigne la projection du centroïde de la facette
  sur l'axe $z$  (en m), $\textbf{n}$ est le vecteur normal unitaire à la surface
  (orienté vers l'extérieur, c'est-à-dire vers le fluide) et $dS$ est la surface
  de la facette (en m^2)
- les facettes partiellement immergées : il faut les séparer en une partie
  immergée et une partie émergée.

On ne choisit pas, pour $z_G$, la hauteur d'eau au-dessus du centroïde : le faire
reviendrait à inclure une partie des efforts d'excitations de Froude-Krylov,
qui agissent comme une correction si la surface libre n'est pas plane.

Lorsque l'on sépare les facettes partiellement immergées en une sous-facette émergée
et une sous-facette immergée, la situation est la suivante :

![](images/hydrostatic_3.svg)

La surface libre est représentée en bleu. Les vrais points d'intersection sont
P' et Q', mais comme le calcul de la fonction $\eta$ représentant l'élévation
de la surface libre est coûteux, on calcule les points P et Q, barycentres
respectifs des segments [AC] et [AD], affectés des coefficients correspondant à
la hauteur d'eau haut-dessus d'eux. Cela revient à approcher la surface libre
par un plan orthogonal à la facette et passant par P et Q. Cette approximation
est d'autant plus juste que les mailles sont petites par rapport à la longueur
d'onde puisque pour une surface libre modélisée à l'ordre 1 par une houle
monochromatique et monodirectionnelle, on a :

$$\eta = \sin(\omega\cdot t - k\cdot x + \phi) = -k\cdot x + o(x)$$

donc l'erreur que l'on commet peut s'écrire sous la forme :

$$\epsilon(k,L) \sim 1-\cos\left(k\cdot\frac{L}{2}\right) \sim \frac{k^2\cdot L^2}{8}$$

où $k$ désigne le nombre d'onde et $L$ la dimension caractéristique de la
maille.

### Calcul du moment

Pour le calcul du moment, il faut connaître le point d'application de chaque
force élémentaire qui se situe en général en-dessous du centroïde de la facette
(sauf si le gradient de pression est uniforme, auquel cas ces deux points sont
confondus).
On peut soit calculer exactement ce point d'application (on obtient alors le
modèle **`non-linear hydrostatic (exact)`**), soit faire l'approximation que le
point d'application est confondu avec le centre de gravité (donnant ainsi le
modèle **`non-linear hydrostatic (fast)`**).

Pour calculer le point d'application, on définit les notations suivantes :

![](images/hydrostatic_1.svg)

![](images/hydrostatic_2.svg)

On désigne par $t_R$ et $u_R$ les coordonnées du point d'application des
efforts dans le plan ($t$,$u$) et $t_G$ et $u_G$ les coordonnées du centroïde
de la facette dans ce même repère. Le repère ($t$, $u$) est centré au centroïde
de la facette.

Les efforts hydrostatiques s'écrivent :
$$\textbf{F}_{\mbox{HS}} = \int_{P\in S} \rho\cdot g\cdot z(P) \textbf{dS}(P)$$

avec $z(P)=t\cdot\sin\theta$

d'où

$$\textbf{F}_{\mbox{HS}} = \rho\cdot g\cdot \sin\theta \textbf{n} \int_S t dS$$

Or

$\int_S t dS = \int\int t dt du = \int du\int t dt = U\cdot\frac{T^2}{2} =
UT\cdot\frac{T}{2} = S\cdot t_G$

d'où

$$\textbf{F}_{\mbox{HS}} = \rho\cdot g\cdot t_G \cdot S \textbf{n}$$

$t_R$ doit vérifier :

$$\int_S t dF = F\cdot t_R$$

soit

$$t_R = \frac{\int_S t^2 dS}{t_G S}$$

Or $\int_S t^2 dS = I_t(G) + S\cdot t_G^2$ donc

où $I_t(G)$ est le second moment d'inertie de la surface par rapport à l'axe
parallèle à $t$ et passant par $G$.

On a donc :

$$t_R = t_G + \frac{I_t(G)}{t_G\cdot S}$$

De même, on trouve :

$$u_R = u_G + \frac{I_u(G)}{t_G\cdot S}$$

or $u_G=0$ par définition du repère $(t,u)$, donc

$$u_R = \frac{I_u(G)}{t_G\cdot S}$$

En pratique, on constate lors de simulations que les deux modèles sont assez
proches sur l'axe $z$ puisque l'amplitude de la force est identique dans les
deux cas. Les différences se situent plutôt au niveau des moments et sont
d'autant plus notables que les mailles sont grandes par rapport au solide (à la
limite, quand la surface des facettes tend vers zéro, les deux modèles
coïncident). La différence la plus flagrante est obtenue lorsque l'on simule
les oscillations en roulis (c'est-à-dire autour de $x$) d'un cubé maillé par
six triangles rectangles : on obtient pour le modèle `fast` des déplacements
parasites suivant $y$ qui n'apparaissent pas avec le modèle `exact`.

Néanmoins, le modèle `exact` impliquant le calcul des matrices d'inertie de
chaque maille (en particulier des mailles générées dynamiquement en calculant
l'intersection de la carène et de la surface libre), il est très coûteux en
temps de calcul (on peut constater un ordre de grandeur par rapport au modèle
`fast`.

### Paramétrage

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
[tutoriels](#tutoriel-2-oscillations-en-immersion).


### Références
Introduction à la mécanique des fluides - CVG 2516, Statique des Fluides, Ioan
NISTOR

## Efforts de Froude-Krylov

### Description

Les efforts de Froude-Krylov constituent une partie des efforts d'excitation
dus à la houle. Ils correspondent aux efforts générés par le champs de pression
de la houle, en supposant que le navire ne perturbe pas l'écoulement. Ils sont
calculés en intégrant la pression dynamique sur la carène. En pratique, ils
peuvent être négligés dès que le corps est à plus d'une-demi longueur d'onde de
profondeur :

$$\mathbf{F}_{\mbox{FK}}(t)=\int_{P=(x,y,z)\in S}
p_{\mbox{dyn}}(x,y,z,t)dS(P)$$

L'expression de la pression dynamique dépend du modèle de houle utilisé et est
décrite (ici pour la houle d'Airy)
[ici](#modèles-de-houle-1).

La pression totale dans le fluide, en un point donné, est la somme de la
pression hydrostatique et de la pression dynamique. Lorsque l'on utilise
conjointement le modèle hydrostatique et le modèle de Froude-Krylov, on est
dans la situation suivante :

![](images/HS_vs_FK.svg)

### Paramétrage

Pour utiliser ce modèle, on insère la ligne suivante dans la section `external forces` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: non-linear Froude-Krylov
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


### Références

- *Environmental Conditions and Environmental Loads*, April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
- *Hydrodynamique des Structures Offshore*, 2002, Bernard Molin, Editions TECHNIP, ISBN 2-7108-0815-3, page 185
- *Sea Loads on Ships An Offshore Structures*, 1990, O. M. Faltinsen, Cambridge Ocean Technology Series, ISBN 0-521-37285-2, pages 16, 39, 59
- *Seakeeping: Ship Behaviour in Rough Weather*, 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, ISBN 0-7458-0230-3, page 67-68
- *Offshore Hydromechanics*, 2001, J.M.J. Journée and W.W. Massie, Delft University of Technology, sections 6-20 and 7-11

## Efforts de diffraction

### Description

Les efforts de diffraction sont dus à la modification du champs de pression du
fait de la présence du navire. Ils sont interpolés à partir de tables
hydrodynamiques. Comme les efforts de radiation et les efforts de masse
ajoutée, ces tables sont calculées en résolvant un problème de condition aux
limites pour le potentiel de vitesse : on utilise donc des codes basés sur des
méthodes potentielles, tels qu'Aqua+. Les tables sont paramétrées en pulsation,
incidence et vitesse d'avance (RAO d'efforts du premier ordre). La principale
différence entre les efforts de radiation et les efforts de diffraction est
l'écriture de la condition aux limites.

### Calcul numérique

Les RAO d'efforts sont lues à partir d'un fichier HDB. Cette table donne, une
fois interpolée, deux fonctions RAO par axe $k$

$$(\omega,\psi)\mapsto {RAO^{k}}_{\mbox{module}}(\omega,\psi)$$
$$(\omega,\psi)\mapsto {RAO^{k}}_{\mbox{phase}}(\omega,\psi)$$

Pour calculer les efforts et les moments, on somme les RAO comme pour le calcul
de l'élévation de la surface libre :

$$F_k(x_H,y_H,t) = \sum_{i=1}^{nfreq}\sum_{j=1}^{ndir}
{RAO^{k}}_{\mbox{module}}(\omega_i,\psi_j)\sqrt{\Delta\omega\Delta\psi}\cos(k\cdot(x\cdot \cos(\psi_j)
+ y\cdot \sin(\psi_j))-\omega_i\cdot
t+{RAO^{k}}_{\mbox{phase}}(\omega_i,\psi_j)+\phi_{ij})$$

$x_H$ et $y_H$ désignent les coordonnées du [point de calcul des efforts
hydrodynamiques](#rep%C3%A8re-de-calcul-hydrodynamique), $t$ est l'instant courant. Les $\omega_i$ et $\psi_j$
correspondent à la discrétisation du spectre de houle. $\phi_{ij}$ est une
phase aléatoire (voir le [modèle de la houle d'Airy](#houle-dairy)).

Le torseur calculé est ensuite déplacé au centre de gravité (repère body).

### Paramétrage

Pour utiliser ce modèle, on écrit `model: diffraction`. Le seul paramètre de ce
modèle est le chemin vers le fichier HDB contenant les RAO d'effort du premier
ordre.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: diffraction
  hdb: anthineas.hdb
  calculation point in body frame:
      x: {value: 0.696, unit: m}
      y: {value: 0, unit: m}
      z: {value: 1.418, unit: m}
  mirror for 180 to 360: true
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le point de calcul n'est pas nécessairement le centre de gravité. En revanche,
il s'agit nécessairement d'un point fixe dans le repère du solide.

Le paramètre `mirror for 180 to 360` sert à pouvoir ne spécifier que la partie
de la RAO entre 0$^{\circ}$ et 180$^{\circ}$, quitte à la symétriser pour
obtenir les points entre 180$^{\circ}$ et 360$^{\circ}$.


## Résistance à l'avancement

### Hypothèses

On suppose la propulsion rectiligne, uniforme et directe, c'est-à-dire
d'intensité et de direction constantes et située dans le plan ($x$,$y$).

On suppose également qu'il n'y a pas de houle (eau calme), que l'assiette du navire est
constante et que sa gite est nulle.

On suppose enfin que la résistance à l'avancement est colinéaire à la
projection sur le plan horizontal de la force propulsive.

Etant données ces hypothèses, on parle de __résistance de remorquage__ à une
vitesse donnée et on note $R_T$ la force nécessaire pour remorquer le navire à
cette vitesse en eau calme.

### Modélisation

Le paradoxe de d'Alembert est que lorsque l'on remorque un objet partiellement
immergé dans un fluide supposé parfait, sa résistance est nulle.
Expérimentalement, bien sûr, on ne constate pas ce phénomène. Cela implique
que :

- l'eau n'est pas un fluide parfait : elle possède une viscosité qui freine
l'objet
- et/ou la surface libre n'est pas à l'équilibre et s'oppose au mouvement du solide.

On décompose donc la résistance de remorquage en deux composantes :

- la résistance visqueuse, liée au frottement de l'eau sur la carène ;
- la résistance de vagues, due à la création d'un champs de vague par le
navire.

En pratique, on effectue une interpolation par spline cubique de la résistance
à l'avancement en fonction de la vitesse du solide par rapport au repère NED
projetée sur l'axe X du repère body (que l'on note $u$). Si $f:u\mapsto R=f(u)$
désigne la fonction d'interpolation, le torseur des efforts, exprimé au [point
de calcul
hydrodynamique](#rep%C3%A8re-de-calcul-hydrodynamique),
est :

$$\tau_{\mbox{res}} =\left[\begin{array}{c}X\\Y\\Z\\K\\M\\N\end{array}\right] =\left[\begin{array}{c}-f(u)\\0\\0\\0\\0\\0\end{array}\right]$$

### Paramétrage

Ce modèle est accessible par la clef [`resistance
curve`](#r%C3%A9sistance-%C3%A0-lavancement).

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

### Références

- *Dynamique du Navire*, 1986, P. Devauchelle, Bibliothèque de L'Institut
Français d'Aide à la Formation Professionnelle Maritime, ISBN 2-225-80669-1,
pages 58, 81 et 97
- *Hydrodynamique navale : théorie et modèles*, 2009, A. Bovis, Presses de
l'ENSTA, page 205 et 337

## Efforts d'amortissement visqueux

### Description

Les mouvements d'un solide évoluant dans un fluide sont amortis du fait de
l'énergie que ce solide communique au fluide. Ces efforts dissipatifs
proviennent d'une part des vagues générées par les mouvements du fluide (et qui
correspondent aux [amortissements de
radiation](#calcul-des-efforts-dexcitation)),
et d'autre part des amortissements visqueux dus au frottement du fluide sur la
coque (apparition d'un sillage tourbillonaire ou turbulent qui dissipe de
l'énergie de manière purement mécanique, essentiellement sur l'axe roulis). Ce
sont ces derniers qui nous intéressent dans cette section.

Les amortissements non-visqueux (radiation) sont, par nature, linéaires par
rapport à la vitesse. Les amortissements visqueux sont, eux, quadratiques. Le
modèle d'amortissement linéaire ne doit être utilisé que pour prendre en compte
les efforts dissipatifs de radiations, si ceux-ci ne sont pas modélisés par
ailleurs. Suivant les axes, certains termes prédominent par rapport aux autres.
Ainsi, en roulis, l'amortissement quadratique est prépondérant par rapport à
l'amortissement linéaire, tandis qu'en tangage c'est l'inverse.

Outre leur signification physique, les termes amortissements ont également une
incidence sur la simulation dans la mesure où ils ont tendance à stabiliser les
schémas d'intégration numériques explicites (type Runge-Kutta par exemple).

Lorsque l'on utilise conjointement les modèles d'amortissement en cavalement et
de résistance à l'avancement, il convient de prendre des précautions
supplémentaires afin de ne pas modéliser deux fois le même phénomène physique.
On décompose donc la vitesse longitudinale en une composante basse fréquence
(utilisée par le modèle de [résistance à
l'avancement](#r%C3%A9sistance-%C3%A0-lavancement))
et une composante haute fréquence (pour le modèle d'amortissement).

### Modélisation

Pour une description des notations adoptées ici on pourra se référer à [la
description du repère de calcul
hydrodynamique](#rep%C3%A8re-de-calcul-hydrodynamique).

La vitesse du courant (vitesse de l'eau par rapport au repère NED, projetée
dans le repère NED) est notée :

$$V_{\mbox{eau/sol}} =
\left[\begin{array}{c}U_{\mbox{courant}}\\V_{\mbox{courant}}\\0\end{array}\right]$$

On définit :

$$\nu_{\mbox{local}} = {}^{\mbox{local}}T_{\mbox{body}} \nu_b -
{}^{\mbox{local}}T_{\mbox{NED}}V_{\mbox{eau/sol}}$$

$$\omega_{\mbox{local}} = {}^{\mbox{local}}T_{\mbox{body}}\omega_{nb}^b$$

Si les efforts de radiation ne sont par modélisés par ailleurs, les
amortissements linéaires s'écrivent (dans le [repère de calcul
hydrodynamique](#rep%C3%A8re-de-calcul-hydrodynamique)) :

$$F_{\mbox{al}}=-D_l\left[\begin{array}{c}\nu_{\mbox{local}}\\\omega_{\mbox{local}}\end{array}\right]_{\mbox{local}}$$

où $D_l$ est la matrice d'amortissement linéaire lue depuis [le fichier de
paramètres](#amortissement-linéaire).

Pour les amortissements quadratiques :

$$F_{\mbox{aq}}=-D_q(\nu_{\mbox{local}})\left[\begin{array}{c}\nu_{\mbox{local}}\\\omega_{\mbox{local}}\end{array}\right]_{\mbox{local}}$$

où

$$D_q(\nu_{\mbox{local}}) = \left[
\begin{array}{cccccc}
d_{11}\cdot|u_{\mbox{local}}| & d_{12}\cdot |v_{\mbox{local}}| & d_{13}\cdot |w_{\mbox{local}}| & d_{14}\cdot |p_{\mbox{local}}| & d_{15}\cdot |q_{\mbox{local}}| & d_{16}\cdot |r_{\mbox{local}}|\\
d_{21}\cdot|u_{\mbox{local}}| & d_{22}\cdot |v_{\mbox{local}}| & d_{23}\cdot |w_{\mbox{local}}| & d_{24}\cdot |p_{\mbox{local}}| & d_{25}\cdot |q_{\mbox{local}}| & d_{26}\cdot |r_{\mbox{local}}|\\
d_{31}\cdot|u_{\mbox{local}}| & d_{32}\cdot |v_{\mbox{local}}| & d_{33}\cdot |w_{\mbox{local}}| & d_{34}\cdot |p_{\mbox{local}}| & d_{35}\cdot |q_{\mbox{local}}| & d_{36}\cdot |r_{\mbox{local}}|\\
d_{41}\cdot|u_{\mbox{local}}| & d_{42}\cdot |v_{\mbox{local}}| & d_{43}\cdot |w_{\mbox{local}}| & d_{44}\cdot |p_{\mbox{local}}| & d_{45}\cdot |q_{\mbox{local}}| & d_{46}\cdot |r_{\mbox{local}}|\\
d_{51}\cdot|u_{\mbox{local}}| & d_{52}\cdot |v_{\mbox{local}}| & d_{53}\cdot |w_{\mbox{local}}| & d_{54}\cdot |p_{\mbox{local}}| & d_{55}\cdot |q_{\mbox{local}}| & d_{56}\cdot |r_{\mbox{local}}|\\
d_{61}\cdot|u_{\mbox{local}}| & d_{62}\cdot |v_{\mbox{local}}| & d_{63}\cdot |w_{\mbox{local}}| & d_{64}\cdot |p_{\mbox{local}}| & d_{65}\cdot |q_{\mbox{local}}| & d_{66}\cdot |r_{\mbox{local}}|\\
\end{array}\right]$$

les $((d_{ij}))$ étant les coefficients de la matrice d'amortissement
quadratique lue depuis [le fichier de
paramètres](#amortissement-quadratique).

### Paramétrage

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
documentation](#efforts-damortissement-visqueux).

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
documentation](#efforts-damortissement-visqueux).



### Références
- *Hydrodynamique des Structures Offshore*, 2002, Bernard Molin, Editions TECHNIP, ISBN 2-7108-0815-3, page 276
- *Sea Loads on Ships An Offshore Structures*, 1990, O. M. Faltinsen, Cambridge Ocean Technology Series, ISBN 0-521-37285-2, page 223
- *Seakeeping: Ship Behaviour in Rough Weather*, 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, ISBN 0-7458-0230-3, page 223
- *Marine Control Systems: Guidance, Navigation and Control of Ships, Rigs and Underwater Vehicles*, 2002, THor I. Fossen, Marine Cybernetics, ISBN 82-92356-00-2, page 71

## Modèles de manoeuvrabilité

### Description

Le but de ce modèle d'effort est de pouvoir écrire un modèle de manoeuvrabilité
de façon assez générique, sans avoir à recompiler le code source. Des
expressions simples des états et du temps peuvent être calculées, par exemple:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- model: maneuvering
  point of application (in body frame):
      x: {value: 0.696, unit: m}
      y: {value: 0, unit: m}
      z: {value: 1.418, unit: m}
  X: 0.5*rho*Vs^2*L^2*X_
  Y: 0.5*rho*Vs^2*L^2*Y_
  Z: 0
  K: 0
  M: 0
  N: 0.5*rho*Vs^2*L^3*N_
  rho: 1024
  Vs: sqrt(u(t)^2+v(t)^2)
  L: 21.569
  X_: Xu*u_ + Xuu*u_^2 + Xuuu*u_^3 + Xvv*v_^2 + Xrr*r_^2 + Xvr*abs(v_)*abs(r_)
  Y_: Yv*v_ + Yvv*v*abs(v) + Yvvv*v_^3 + Yvrr*v_*r_^2 + Yr*r_ + Yrr*r_*abs(r_) + Yrrr*r_^3 + Yrvv*r_*v_^2
  N_: Nv*v_ + Nvv*v*abs(v) + Nvvv*v_^3 + Nvrr*v_*r_^2 + Nr*r_ + Nrr*r_*abs(r_) + Nrrr*r_^3 + Nrvv*r_*v_^2
  u_: u(t)/Vs
  v_: v(t)/Vs
  r_: r(t)/Vs*L
  Xu: 0
  Xuu: 0
  Xuuu: 0
  Xvv: -0.041
  Xrr: -0.01
  Xvr: -0.015
  Yv: -0.13
  Yvv: -0.18
  Yvvv: 0
  Yvrr: 0
  Yr: 0.015
  Yrr: 0.021
  Yrrr: 0
  Yrvv: 0
  Nv: -0.37
  Nvv: -0.12
  Nvvv: 0
  Nvrr: 0
  Nr: -0.1
  Nrr: 0.005
  Nrrr: 0
  Nrvv: 0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`point of application (in body frame)`: Coordonnées du point d'application du
torseur (dans le repère "body"). Le torseur est automatiquement déplacé au
centre de gravité (point (0,0,0) du repère "body").
`X`, `Y`, `Z`, `K`, `M`, `N` : coordonnées du torseur d'effort (dans le repère
body), exprimé au point d'application défini ci-dessus.

Les autres clefs peuvent être quelconques et dépendent du modèle.

Toutes les valeurs sont supposées en unité du système international. Le modèle
nécessite de spécifier X, Y, Z, K, M et N. Des variables accessoires (telles
que `tau` dans l'exemple ci-dessus) peuvent être utilisées. Le modèle vérifie
automatiquement à l'exécution qu'il possède toutes les clefs nécessaires.

### Grammaire

De façon plus formelle, les modèles doivent obéir à la grammaire suivante
(format Bachus-Naur ou BNF) :


# Efforts commandés

## Description

Les efforts contrôlés correspondent aux efforts de propulsion, de safran et de
foil. Ils sont décrits dans la section `controlled forces`. Les seules clefs
YAML communes à tous les efforts commandés sont `name` (qui est un identifiant
choisi par l'utilisateur) et `model` (qui est une chaîne servant à identifier
le type de modèle utilisé).

La provenance des commandes (où le simulateur lit les commandes à chaque pas
de temps) doit être spécifiée lors de l'appel de l'exécutable en
utilisant le flag `--commands` décrit dans la [documentation de l'interface
utilisateur](#liste-des-arguments). Les commandes ne sont pas
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

Les [commandes
attendues](#syntaxe-du-fichier-de-commande) pour ce
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

## Hélices Wageningen série B

### Description

En 1937, l'ingénieur néerlandais L. Troost, alors employé du Maritime Research
Institue Netherlands (MARIN) basé à Wageningen (Pays-Bas), créa les hélices
Wageningen série B dont la forme est simple et les performances bonnes.  Afin
d'établir une base pour la conception d'hélices, il publia en 1938 puis en 1940
une série de tests systématiques en eau libre de 120 hélices "série B", qui
sont, à ce jour, les séries de test en eau libre les plus connus, bien que
d'autres instituts de recherche en aient réalisés d'autres par la suite.

En 1975, Oosterveld et Ossannen utilisèrent une régression statistique pour
établir le modèle polynomial des hélices Wageningen présenté ici.

Un [tutoriel](#tutoriel-6-propulsion) présente l'utilisation de
ce modèle dans le simulateur.

### Hypothèses du modèle en eau libre

On adopte les notations suivantes :

- $T$ est la poussée de l'hélice en eau libre (en N), c'est-à-dire la norme des
efforts générés par l'hélice suivant son axe (sans tenir compte de la succion de la coque),
- $Q$ est le couple généré par l'hélice en eau libre autour de son axe. Il
  s'exprime en N.m.
- $n$ est le nombre de tours que l'hélice fait par seconde (en tr/s)
- $D$ est le diamètre de l'hélice (en m)
- $rho$ est la densité volumique de l'eau (en kg/m^3)
- $V_a$ est la vitesse d'avance (en m/s), c'est-à-dire la vitesse de l'écoulement non
perturbé (à l'infini). Il s'agit d'une vitesse théorique : on se place dans le
cas où l'hélice est complètement isolée dans un environnement infini (eau
libre) qui bouge à la vitesse $V_a$ par rapport au sol.

Le modèle en eau libre est sujet aux hypothèses suivantes :

- on néglige les effets de la houle (notamment sa vitesse orbitale et le champs
  de pression qu'elle génère) et des courants océaniques,
- on ne tient pas compte des interactions entre l'hélice et la coque
  (perturbation du fluide en amont de l'hélice) et entre l'hélice et la surface
  libre. On suppose ainsi que l'hélice ne crée pas de vagues, donc qu'elle ne
  dissipe pas d'énergie à la
  surface).

L'intérêt de ce modèle est qu'il est paramétrique et permet de représenter les
performances de l'hélice sous forme adimensionnelle. On peut ainsi appliquer le
même modèle (à un coefficient d'échelle près) à des hélices homothétiques.
Une limitation supplémentaire du modèle polynomial en eau libre est que,
contrairement au modèle quatre quadrant, il n'est valable qu'en marche
avant (c'est-à-dire pour $n$ positif ou nul).

### Dérivation du modèle en eau libre

Le modèle en eau libre est un modèle empirique dans la mesure où il ne dérive
pas des équations de Navier-Stokes. Le postulat est, qu'étant données les
hypothèses ci-dessus, on peut s'attendre à ce que la poussée de l'hélice
dépende :

- Du diamètre $D$ (en m) de celle-ci
- De la vitesse $V_a$ d'avance du fluide (en m/s)
- De la vitesse de rotation $n$ de l'hélice (en Hz)
- De la densité $\rho$ du fluide (en kg/m^3)
- De la viscosité $\mu$ du fluide
- De la pression statique du fluide $p_0-e$ au niveau de l'hélice

On aurait donc :

$$T_0 \propto \rho^a\cdot D^b\cdot V_a^c \cdot n^d \cdot \mu^f\cdot(p_0-e)^g$$

En effectuant l'analyse dimensionnelle pour exprimer $a$, $b$ et $d$ en
fonction des autres coefficients, on trouve :

$$T_0 \propto \rho^{1-f-g}\cdot D^{4-c-2f-g}\cdot V_a^c \cdot n^{2-c-f-2g} \cdot \mu^f\cdot(p_0-e)^g$$

Soit, en regroupant les termes de même puissance :

$$T_0 \propto \rho\cdot n^2\cdot D^4\cdot \left(\frac{V_a}{n\cdot D}\right)^c \cdot\left(\frac{\mu}{\rho\cdot n\cdot D^2}\right)^f\cdot\left(\frac{p_0-e}{\rho\cdot n^2\cdot D^2}\right)^g$$

On définit le coefficient de poussée :

$$K_T = \frac{T_0}{\rho\cdot n^2\cdot D^4}$$

Le coefficient d'avance $J$ est défini par :

$$J=\frac{V_a}{n\cdot D}$$

Le nombre de Reynolds $R_n$ s'exprime ici :

$$R_n = \frac{\rho \cdot n\cdot D^2}{\mu}$$

et le nombre de cavitation $\sigma_0$ est :

$$\sigma_0=\frac{p_0-e}{\frac{1}{2}\rho\cdot n^2\cdot D^2}$$

donc il existe une fonction $f$ telle que

$$K_T = f(J,R_n,\sigma_0)$$

De même, pour le couple $Q$, on définit le coefficient de couple $K_Q$ par :

$$K_Q = \frac{Q_0}{\rho\cdot n^2\cdot D^5}$$

Le modèle en eau libre consiste à expliciter les fonctions $K_T$ et $K_Q$, dont
on peut ensuite dériver la poussée et le couple.

### Prise en compte des effets de la coque et du sillage

Lorsque l'écoulement au niveau de l'hélice a été perturbé par la coque, la
vitesse du fluide au niveau de l'hélice $V_a$ n'est pas égale (en valeur absolue) à
la vitesse du navire par rapport à l'eau $V_s$, autrement dit $V_a\neq V_s$.
La vitesse d'avance $V_a$ est, en général, très
difficile à mesurer et l'on suppose qu'elle est proportionnelle à la vitesse du
navire. On définit donc un coefficient $w$ (pour "wake", soit "sillage" en
anglais) tel que :

$$w=1-\frac{V_a}{V_s}$$

$w$ est constant en régime permanent, lorsque l'hélice opère dans les
conditions nominales. Des ordres de grandeurs de ce coefficient sont donnés par
exemple dans Carlton, pages 70,72,73 et 74.

En outre, l'hélice accroît la [résistance à
l'avancement](#r%C3%A9sistance-%C3%A0-lavancement)
: en effet, elle diminue la pression à l'arrière du navire, ce qui augmente la
poussée nécessaire pour la propulsion. L'helice accélérant le fluide, il existe
des frottements supplémentaires sur la coque. Pour prendre en compte ces
phénomènes, on introduit le coefficient de succion $t$ tel que :

$$t = 1 - \frac{R_v}{T_p}$$

où $R_v$ est la résistance à l'avancement (en N) à une vitesse $V_S$, sans
hélice, et $T_p$ est la somme des poussées de tous les actionneurs (également
en N) lorsque le navire va à la vitesse $V_S$ en utilisant l'hélice.

La poussée réelle $T_b$ est alors définie par :

$$T_b = (1-t)\cdot T_0 = (1-t)\cdot \rho\cdot n^2\cdot D^4 \cdot K_T(J, R_n,\sigma_0)$$

et le couple réel est

$$Q_b = \eta_R\cdot Q_0 = \eta_R\cdot \rho\cdot n^2\cdot D^5 \cdot K_Q(J, R_n,\sigma_0)$$

où $\eta_R$ est appelé **rendement d'adaptation**

$$J = \frac{V_a}{n\cdot D} = \frac{(1-w)\cdot V_s}{n\cdot D}$$

### Expression des coefficients $K_T$ et $K_Q$

Afin de rendre les coefficients indépendants de la taille de l'hélice, on
définit la fraction de surface de l'hélice $A_E/A_0$, où $A_E$ désigne l'aire des
pales (en m^2) et $A_0= \frac{\pi\cdot D^2}{4}$ est l'aire du disque
circonscrit à l'hélice. Les séries sont valables pour $0.30\leq A_E/A_0\leq
1.05$.

On définit également le pas $P$ de l'hélice, un paramètre géométrique qui
traduit la distance théorique parcourue par l'hélice en une révolution. Cette
distance varie en fonction de la ligne de référence que l'on choisit. Les
séries B de Wageningen utilisent le pas **de face**, mais il existe d'autres
conventions. Les séries sont paramétrés en $P/D$ et l'on suppose que $0.5\leq
P/D\leq 1.4$.

On note $2\leq Z\leq 7$ le nombre de pales de l'hélice.

Les coefficients des polynômes pour $K_T$ et $K_Q$ sont notés $C_i^T$
et $C_i^Q$ respectivement, où $i$ est un entier tel que $1 \leq i \leq 47$.
$s(i)$, $s'(i)$, $t(i)$, $t'(i)$, $u(i)$, $u'(i)$, $v(i)$ et $v'(i)$ sont des
exposants entre 0 et 6.

$$K_T(J, P/D, A_E/A_0, Z, R_n=2\times 10^6) = \sum_{i=1}^47 C_k^T \cdot J^{s(i)}\cdot
(P/D)^{t(i)}\cdot (A_E/A_0)^{u(i)}\cdot Z^{v(i)}$$
$$K_Q(J, P/D, A_E/A_0, Z, R_n=2\times 10^6) = \sum_{i=1}^47 C_k^Q \cdot J^{s'(i)}\cdot
(P/D)^{t'(i)}\cdot (A_E/A_0)^{u'(i)}\cdot Z^{v'(i)}$$

Les coefficients $C_k^T$ et $C_k^Q$ sont définis pour un nombre de Reynolds
$R_n=2\times 10^6$, mais le modèle a été étendu pour des nombres de Reynolds
entre $2\times 10^6$ et $2\times 10^9$ en introduisant des termes $\Delta K_T$
et $\Delta K_Q$ supplémentaires :

$$K_T(J, P/D, A_E/A_0, Z, R_n) = K_T(J, P/D, A_E/A_0, Z, 2\times 10^6) + \Delta
K_T(J, P/D, A_E/A_0, Z, R_n)$$
$$K_Q(J, P/D, A_E/A_0, Z, R_n) = K_Q(J, P/D, A_E/A_0, Z, 2\times 10^6) + \Delta
K_Q(J, P/D, A_E/A_0, Z, R_n)$$ 

### Expression des efforts

Les efforts générés par l'hélice sont calculés dans un repère spécifique
renseigné dans la section [`position of propeller frame`](#wageningen-b-series) du fichier YAML. La
poussée (c'est-à-dire l'effort généré par l'hélice sur le navire) est faite
dans le sens des $x$ positifs.

![](images/reperes_helices.svg)

Le sens de rotation de l'hélice doit également être spécifié parce qu'il
détermine le signe du couple généré par l'hélice sur le navire. On définit ce
sens de rotation en se plaçant face à l'hélice, en regardant dans la direction
des $x_{\mbox{hélice}}$ positifs (donc vers l'avant du navire). Autrement dit, l'axe de
rotation de l'hélice est non pas $x_{\mbox{hélice}}$ mais $-x_{\mbox{hélice}}$.
Lorsque l'hélice tourne dans le sens **horaire**, elle génère un couple dans le
sens trigonométrique, soit un couple de signe **négatif** lorsqu'il est exprimé
dans le repère de l'hélice :

![](images/rotation_helices.svg)

Le torseur des efforts générés par l'hélice et subit par le navire
(apparaissant donc dans le membre de droite de l'équation fondamentale de la dynamique),
exprimé dans le repère de l'hélice, est donc :

$$\tau_{\mbox{hélice}} = \left[\begin{array}{c}
X\\
Y\\
Z\\
K\\
M\\
N
\end{array}\right]_{\mbox{hélice}}$$

$$= \left[\begin{array}{c}
           T_b\\
           0\\
           0\\
           \kappa Q_b\\
           0\\
           0
\end{array}\right]_{\mbox{hélice}}$$
$$= \left[\begin{array}{c}
           (1-t)\cdot \rho\cdot n^2\cdot D^4 \cdot K_T(J, R_n,\sigma_0)\\
           0\\
           0\\
           \kappa \cdot \eta_R\cdot \rho\cdot n^2\cdot D^5 \cdot K_Q(J,
R_n,\sigma_0)\\
           0\\
           0\end{array}\right]_{\mbox{hélice}}
$$

$\kappa$ vaut -1 si l'hélice tourne dans le sens horaire (en se plaçant
derrière l'hélice et en regardant vers l'avant du navire) et +1 si elle tourne
dans le sens trigronométrique.

Ce torseur est ensuite déplacé (changement de point d'application et changement
de coordonnées) dans le repère body afin d'être sommé avec les autres lors du
bilan des efforts.

### Paramétrage

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
attendues](#syntaxe-du-fichier-de-commande).
- `model` : Nom du modèle. Doit être `wageningen B-series` pour utiliser ce
modèle.
- `position of propeller frame` : Définition du [repère de
l'hélice](#expression-des-efforts).
- `frame` : repère dans lequel sont exprimés `x`,`y`,`z`,`phi`,`theta` et `psi`.
- `x`,`y`,`z` : projection de la position du centre de poussée de l'hélice par rapport au centre du repère attaché au maillage et projeté sur ce dernier.
- `phi`,`theta`,`psi` : Définition de la rotation permettant de passer du
repère attaché au maillage au [repère attaché à
l'hélice](#expression-des-efforts), en suivant la
[convention d'angle choisie](#rotations).
- `wake coefficient` : [coefficient de
sillage](#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
traduisant la perturbation de l'écoulement par la coque du navire. Entre 0 et
1.
- `relative rotative efficiency` : [rendement
d'adaptation](#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
- `thrust deduction factor t` : [coefficient de
succion](#prise-en-compte-des-effets-de-la-coque-et-du-sillage)
- `rotation` définition du sens de rotation pour générer une poussée positive.
Utilisé pour calculer le signe du moment généré par l'hélice sur le navire. Les
valeurs possibles sont `clockwise` et `anti-clockwise`. Si on choisit
`clockwise`, l'hélice tournera dans le sens horaire (en se plaçant à l'arrière
du navire et en regardant vers la proue) et génèrera un moment négatif sur le navire (dans le repère de l'hélice). Voir [la
documentation](#expression-des-efforts).
- `number of blades` : nombre de pales de l'hélice.
- `blade area ratio AE/A0` : [fraction de
surface](#expression-des-coefficients-k_t-et-k_q) de l'hélice.
- `diameter` : diamètre de l'hélice

### Références

- *Marine Propellers and Propulsion*, 2007, John Carlton, Butterworth-Heinermann, ISBN 978-07506-8150-6, page 89, 103
- *Seakeeping: Ship Behaviour in Rough Weather*, 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, ISBN 0-7458-0230-3, page 404
- *KT, KQ and Efficiency Curves for the Wageningen B-series Propellers*, 1981, Bernitsas, Ray, Kinley, University of Michigan
- *Offshore Hydromechanics*, 2001, J.M.J. Journée and W.W. Massie, Delft University of Technology, sections 4-40
- *Thrust Estimation and Control of Marine Propellers in Four-Quadrant Operations*, 2008, Luca Pivano, NTNU, ISBN 978-82-471-6258-3, page 30, 31
- *The Wageningen Propeller Series*, 1992, Gert Kuiper, Marin publication 92-001


## Contrôleur de cap simplifié

### Description

Le but de ce contrôleur est de pouvoir réaliser des simulations sous houle (par
exemple pour calculer des RAO d'effort) en limitant les variations de cap. Ce
contrôleur génère directement un moment au centre de gravité du corps.

### Expression des efforts

Le moment généré est $M_z = K_{\psi}\cdot (\psi_{\mbox{co}}-\psi) - K_r\cdot r =
\sigma_{zz}\ddot{\psi}$ où $\sigma_{zz}$ désigne le moment d'inertie total
(inertie propre et inertie ajoutée) autour de l'axe $z$.

Dans le domaine de Laplace, l'équation du contrôleur s'écrit :

$$\sigma_{zz}p^2 + K_r p + K_{\psi} = 0$$

ou encore, sous forme canonique :

$$p^2 + 2\zeta\omega_0 p + \omega_0^2 = 0$$

d'où

$$K_{\psi} = \sigma_{zz} \omega_0^2$$ et $$K_r= 2\zeta\omega_0\sigma_{zz}$$

On peut exprimer ces gains en fonction de l'amortissement $\zeta$ et du temps de
réponse $T_p$ donné par $T_p=\frac{2\pi}{\omega_0}$.

$$K_{\psi} = \sigma_{zz}\left(\frac{2\pi}{T_p}\right)^2$$
$$K_{r} = 2\zeta\sigma_{zz}\frac{2\pi}{T_p}$$

Le cap $\psi_{\mbox{co}}$ est donné dans le repère NED.
Si l'on suppose que $r=0$, pour $\psi<\psi_{\mbox{co}}$, le moment généré doit
être positif, donc $K_{\psi}\cdot(\psi_{\mbox{co}}-\psi)>0$. Par conséquent,
$K_{\psi}>0$.
De même, en prenant $r<0$ et $\psi=\psi_{\mbox{co}}$, le moment généré doit
être positif pour contrer la vitesse $r$, donc $-K_r\cdot r>0$, d'où $K_r>0$.

### Paramétrage

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
controlled forces:
  - name: controller
    model: simple heading controller
    ksi: 0.9
    Tp: {value: 4, unit: s}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `name` : nom du contrôleur (si l'on en utilise plusieurs)
- `model` : `simple heading controller` pour ce modèle
- `ksi`: coefficient d'amortissement de la loi de commande
- `Tp`: temps de réponse (système du second ordre).

Ce modèle n'a qu'une seule commande, le cap `psi_co` :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
- name: controller
  t: [0,1,3,10]
  psi_co: {unit: deg, values: [25, 30, 40, 0]}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


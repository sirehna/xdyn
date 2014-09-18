# Bilan des efforts

Le navire est soumis aux efforts suivants :

- La pesanteur,
- Les efforts d'amortissement visqueux, dûs aux frottements de l'eau sur la
  coque et aux tourbillons,
- Les efforts hydrostatiques, liés à la pression exercée par le fluide au repos
  sur la coque,
- Les efforts hydrodynamiques, provenant d'une part de l'excitation de la houle
  et d'autre par des phénomènes de radiation/diffraction détaillés ci-après,
- Les efforts commandés. On classe dans cette catégorie les efforts de
  propulsion (hélices, tunnels, azimutaux, Voith...) mais aussi les safrans et
  ailerons.

Les efforts sont renseignés dans la section bodies/external forces du fichier
YAML :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
bodies:
  - name: ship
    external forces:
     - model: gravity
     - model: non-linear hydrostatic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Efforts de gravité

Le poids est donné dans le repère NED par :

$F_P = m \cdot 9.81 \mathbf{u_z}$

La constante de gravité a été volontairement fixée à 9.81 dans le code,
autrement dit elle ne peut pas être modifiée, afin d'alléger le paramétrage.

## Efforts hydrostatiques non-linéaires

La force $\textbf{F}_{\mbox{HS}}$ exercée par l'eau sur la carène doit être
calculée comme l'intégrale de la pression hydrostatique $p_{\mbox{HS}}$ sur
la surface immergée totale :

$$\textbf{F}_{\mbox{HS}} = \int_{S}p_{\mbox{HS}}(z)\cdot \textbf{n} dS $$

Il faut donc déterminer l'intersection de la carène et de la surface libre à
chaque instant, ce qui implique de modifier le maillage.

## Efforts d'amortissement visqueux



## Efforts de propulsion

## Efforts hydrodynamiques

D'après la section "expression des efforts hydrodynamiques", les efforts
hydrodynamiques se décomposent en efforts d'excitation d'une part, et en efforts
de radiation d'autre part.

![](images/efforts_hydros.svg "Catégories de modèles d'efforts hydrodynamiques")

### Efforts d'excitation dus à la houle

Les efforts d'excitation sont à leur tour la somme de deux efforts : les efforts
de Froude-Krylov et les efforts de diffraction.

#### Efforts de Froude-Krylov

$$F_{\mbox{FK}}=\rho \int_{C} \frac{d\Phi_I}{dt} dS $$

$\Phi_I$ est le potentiel des vitesses de la houle incidente

En pratique, comme il s'agit d'un effort surfacique, il est traité de la même
manière que les efforts hydrostatiques : 

#### Efforts de diffraction

Les efforts de diffraction sont interpolés à partir des RAO d'efforts (réponse
impulsionnelle) obtenus sous forme de fichiers HDB par des codes potentiels
tels que Dyodore.

### Efforts de radiation

Les efforts de radiation proviennent des inerties ajoutées et de l'amortissement
de radiation.

#### Masses ajoutées

$$F_{mbox{MA}} = M_A(\infty)\frac{d\nu}{dt}$$ 

La matrice de masse ajoutées à pulsation infinie $M_A(\infty)$ est, là encore,
calculée par un code potentiel type Dyodore ou Aqua+. Elle est supposée
constante au cours du temps et est renseignée dans la section bodies/dynamics:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
bodies:
  - name: ship
    dynamics:
        added mass matrix at the center of buoyancy projected in the body frame:
            frame: ship
            row 1: [3.519e4,0,0,0,0,0]
            row 2: [0,3.023e5,0,0,0,0]
            row 3: [0,0,1.980e5,0,0,0]
            row 4: [0,0,0,3.189e5,0,0]
            row 5: [0,0,0,0,8.866e6,0]
            row 6: [0,0,0,0,0,6.676e6]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Elle est exprimée au centre de flottaison projeté dans le repère body et chaque
coefficient est en kg.

#### Amortissement non-visqueux de radiation

$$F_R = - \int_{-\infty}^tK(t-\tau)\frac{d\nu}{dt}(\tau)d\tau$$

La matrice $K$ des fonctions de retard est fournie. On utilise la base de
données Diodore. En pratique, on se limite à 25 secondes pour le produit de
convolution.

## Efforts de Coriolis

Afin de ne pas réaliser de changement de repère intempestif sur des quantités
complexes telles que les masses ajoutées, le bilan des efforts est écrit dans le
repère navire, qui n'est pas galliléen. Par conséquent, il faut tenir compte des
termes d'inertie d'entraînement (accélération de Coriolis) dus aux mouvements du
référentiel navire par rapport au repère NED, considéré comme galliléen.

### Expression

$$F_{\mbox{coriolis}} = C(\nu)\nu$$

$C(\nu)$ est appelé "matrice de Coriolis".

$$
C(\nu) = C(\left[\nu_1,\nu_2\right]) = \left[
\begin{array}{cc}
0_{3\times 3}            & -S(M_{11}\nu_1 + M_{12}\nu_2)\\
-S(M_{11}\nu_1 +M_{12}\nu_2) & -S(M_{21}\nu_1 +M_{22}\nu_2)
\end{array}
\right]$$

Où

$\nu_1^T = \left[u,v,w\right]$
$\nu_2^T = \left[p,q,r\right]$

$S(u)=\left[
\begin{array}{ccc}
0         &-u_z&u_y\\
u_z  &0&-u_x\\
-u_y &u_x&0
\end{array}
\right]$ est la matrice du produit vectoriel avec le vecteur $u$.

Les $M_{ij}$ sont tels que :
$M=\left[
\begin{array}{cc}
M_{11} & M_{12}\\
M_{21} & M_{22}\\
\end{array}
\right]$
$M$ étant la matrice d'inertie propre du solide et les matrices $M_{ij}, 1\leq
i,j\leq 3$ étant toutes de dimensions $3\times 3$.

En réalité, il faudrait également faire apparaître la matrice des masses
ajoutées.


### Suppression des termes de Coriolis pour les masses ajoutées

Lorsque l'on écrit le bilan des efforts appliqués au solide, on a :

$M \dot{\nu} = - B_v \nu + F_{\mbox{prop}} + F_{\mbox{hs}} + F_{\mbox{hd}}$

$M$ est la matrice des inerties propres du solide, $B_v$ est la matrice
d'amortissement visqueux dus aux frottements fluides, aux tourbillons..., $F_{\mbox{prop}}$
désigne les efforts de propulsion (hélices, tunnels, azimutaux...)
$F_{\mbox{hs}}$ sont les efforts hydrostatiques et $F_{\mbox{hd}}$ les efforts
hydrodynamiques.

On a donc :

$M \dot{\nu} = - B_v \nu + F_{\mbox{prop}} + F_{\mbox{hs}} + F_E
-M_A(\infty)\dot{\nu} -  \int_{0}^{+\infty} K(t)\nu(t-\tau)d\tau + F_{\mbox{FK}} + F_D$

soit

$(M+M_A(\infty)) \dot{\nu} = - B_v \nu + F_{\mbox{prop}} + F_{\mbox{hs}} + F_E
+ \int_{0}^{+\infty} K(t)\dot{X}(t-\tau)d\tau + F_{\mbox{FK}} + F_D$

La matrice des masses ajoutées est calculée suivant des hypothèses restrictives
de petits mouvements qui ne sont souvent pas totalement respectées en pratique. On
constate notamment lors des essais que les résultats des simulations sont plus
proches de la réalité lorsque l'on ne considère pas les termes de transport
(Coriolis) dus au fait que l'on intègre les équations du mouvement dans un
repère non-galiléen. En pratique, on doit donc fausser la cinématique du
problème pour obtenir des résultats proches du réel.

D'autre part, les efforts dus aux inerties ajoutées dépendant de la dérivée des
états, on ne peut pas les traiter dans le membre de droite de l'équation pour
des raisons de stabilité numérique.

En d'autres termes, les efforts dus aux inerties ajoutées doivent recevoir un
traitement bien distinct des autres efforts. Le calcul des
accélérations est ainsi fait de la façon suivante :

$$\dot{\nu}= (M+M_A(\infty))^{-1}(F_{\mbox{coriolis}} + F_E +
\int_{0}^{+\infty} K(t)\nu(t-\tau)d\tau + F_{\mbox{FK}} +F_D)$$

où $F_{\mbox{coriolis}}$ est défini comme au paragraphe suivant, c'est-à-dire en
ne considérant que les inerties propres et pas les masses ajoutées. Cette
formule, bien que cinématiquement fausse (il faudrait en principe utiliser
$M+M_A(\infty)$ et non juste $M$), donne en pratique des résultats plus fidèles
que la formule exacte, surtout en giration.



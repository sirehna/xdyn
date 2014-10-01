# Expression des efforts hydrodynamiques

## Potentiel d'interaction entre la houle et l'obstacle

On suppose l'eau non visqueuse, incompressible, homogène et isotrope et l'on
considère un écoulement irrotationnel.
Supposer l'écoulement irrotationnel implique (d'après le lemme
de Poincaré) que la vitesse dérive d'un potentiel que l'on appelle
$\Phi_T:(x,y,z,t)\mapsto\Phi_T(x,y,z,t)$. Par définition, la vitesse en tout
point de l'écoulement est donc donnée par :

$$V(x,y,z,t) = \mbox{grad}{\Phi_T(x,y,z,t)}$$

Le potentiel de la houle incidente est connu si l'on se place dans le cadre de
la théorie linéaire de Stokes. On désignera par $\Phi_I$ ce potentiel. On a en
effet :

$$\Phi_I(y,z,t) = \frac{g H}{2 \omega}
\frac{\cosh(k(z-h))}{\cosh(kh)}\cos(ky-\omega t)$$

avec : $$\omega^2 = g\cdot k\cdot \tanh(kh)$$

où $k$ désigne le nombre d'onde, $\omega$ la pulsation de la houle, $h$ la
profondeur d'eau, $H$ la double amplitude (ou creux) et $g$ l'accélération de
la pesanteur.

On pose :

$$\Phi_P := \Phi_T-\Phi_I$$

que l'on nomme "potentiel d'interaction entre la houle et l'obstacle". C'est ce
potentiel qui nous intéresse ici puisque c'est la seule indéterminée.

On contraint $\Phi_P$ à être une fonction harmonique du temps de pulsation
$\omega$ :

$$\Phi_P(x,y,z,t) = \Re{\Psi_P e^{-i\omega t}}$$


## Conditions satisfaites par le potentiel d'interaction entre la houle et l'obstacle

Le potentiel inconnu $\Phi_P$ doit satisfaire les conditions suivantes :

1. Le fluide étant incompressible et l'écoulement irrotationnel, $\Phi_P$
   vérifie l'équation de Laplace $\Delta \Phi_P = 0$.
2. Si l'on suppose l'obstacle immobile, la condition de surface libre s'écrit :
   $$\left[g\cdot\frac{\partial \Psi_P}{\partial z} + \omega^2
   \Psi_P\right](x,y,0,t) = 0$$
3. Le potentiel étant indéterminé si l'on n'impose pas une condition sur le sens
   de propagation de l'énergie, on imposera la condition de rayonnement de
   Sommerfeld (phénomène divergent ; l'énergie s'éloigne de l'obstacle) :
   $\sqrt{r}\lvert{\partial \Psi_P}{\partial r} - i \lvert k\rvert\Psi_P\rvert
   \rightarrow 0$ quand $r=\sqrt{x^2 + y^2}\rightarrow \infty$ (condition de
   rayonnement à l'infini). Cette condition exprime que, au loin, les ondes de
   diffraction-radiation ont pour nombre d'onde $k$, se propagent dans la
   direction radiale et leur amplitude décroît comme $1/\sqrt{r}$.
4. Le fond étant imperméable, la composante suivant $z$ de la vitesse y est
   nulle : $V_z(x,y,h,t) = \frac{\partial Phi_P}{\partial z}(x,y,h,t) = 0$ et
   par là même : $\frac{\partial Psi_P}{\partial z}(x,y,h) = 0$.
5. L'obstacle étant imperméable, $(\mbox{grad} \Psi_P) \cdot n = \frac{\partial
\Psi_P}{\partial n} = -(\mbox{grad}
   \Psi_I) \cdot n + V_o \cdot n$ où $V_o$ est la vitesse normale de l'obstacle en
   un point $P$ et $n$ est la normale extérieure à l'obstacle.



## Résolution

Si l'on suppose l'obstacle fixe, la condition (5) s'écrit :
$$ \frac{\partial \Psi_P}{\partial n} = - \frac{\partial \Psi_I}{\partial n}$$
Cette condition traduit la réflexion (ou diffraction) de la houle incidente sur
l'obstacle fixe. Un potentiel $\Psi_{\mbox{PD}}$ vérifiant les conditions (1) à
(4) et la condition de diffraction est appelé potentiel de diffraction.

Si l'on ne considère qu'on mouvement oscillatoire élémentaire de l'obstacle
suivant son $j$-ème degré de liberté, la condition (5) s'écrit : $$
\frac{\partial \Psi_P}{\partial n} = n_j$$. Un potentiel $\Psi_{PR_j}$
vérifiant les conditions (1) à (4) et cette condition-ci est appelé $j$-ème
potentiel élémentaire de rayonnement et correspond au mouvement engendré par ce
mouvement oscillatoire élémentaire de l'obstacle.

En définitive, la solution complète $\Phi_D$ du problème de
diffraction-rayonnement obtenue par superposition de la solution "obstacle
fixe" et des solutions ocillatoires élémentaires peut s'écrire :

$$ \Phi_D = \Re\left[\Psi_{\mbox{PD}}e^{-i\omega t} + \sum_{j=1}^6
f_j\Psi_{\mbox{PR}_j} e^{-i\omega t}\right]$$

en adoptant les notations suivantes :

- $V=\left[f_1,f_2,f_3\right]$ est la vitesse de translation du corps
- $\Omega=\left[f_4,f_5,f_6\right]$ est sa vitesse de rotation



## Expression des efforts hydrodynamiques

On pose $$\Phi_{\mbox{PR}} =  \Re\left[\sum_{j=1}^6 f_j \Psi_{PR_j} e^{-i\omega
t}\right]$$

Les efforts hydrodynamiques s'écrivent :

$$F_{\mbox{hd}} = \rho \frac{d}{dt} \int_{C}\Phi_T dS$$
$$= \rho \frac{d}{dt} \int_{C}\Phi_I+\Phi_{\mbox{PD}}+\Phi_{\mbox{PR}} dS$$

On appelle $$F_{\mbox{FK}}=\rho \frac{d}{dt} \int_{C}\Phi_I dS $$ "efforts de
Froude-Krylov" et $$F_D = \rho \frac{d}{dt} \int_{C}\Phi_{\mbox{PD}} dS $$ "efforts de
diffraction". Ensemble ils constituent les efforts d'excitation de la houle
$F_E$. Les efforts $\rho \frac{d}{dt} \int_{C}\Phi_{\mbox{PR}} dS$ sont nommés
"efforts de radiation" et sont désignés par $F_R$. On a donc :

$$F_{\mbox{hd}} = F_E + F_R$$



## Calcul des efforts d'excitation

Les efforts de Froude-Krylov sont calculés en intégrant la pression dynamique
sur la carène et en supposant que le corps ne perturbe pas la houle. En
pratique, ils peuvent être négligés dès que le corps est à plus d'une-demi
longueur d'onde de profondeur.

Les efforts de diffraction sont interpolés à partir de tables
calculées par des logiciels de calculs hydrodynamiques basés sur des méthodes
potentielles, tels qu'Aqua+. Ils sont paramétrés en pulsation, incidence et
vitesse d'avance (RAO d'efforts).



## Calcul des efforts de radiation

On a, pour l'axe $k$ :

$$ F_{\mbox{R}_k} = \rho \frac{\partial}{\partial t} \int_C \Phi_{\mbox{PR}}
n_k dS$$
$$= \rho \frac{\partial}{\partial t} \Re \int_C \sum_j \Psi_{\mbox{PR}_j} f_j
e^{-i\omega t} n_k dS$$
$$= \rho \Re \int_C \sum_j -i\omega  \Psi_{\mbox{PR}_j} f_j e^{-i\omega t} n_k dS$$

On décompose $\Psi_{\mbox{PR}_j}$ en sa partie réelle et sa partie imaginaire :
$\Psi_{\mbox{PR}_j} = \Psi_j^R + i \Psi_j^I$

On a alors :

$$ F_{\mbox{R}_k} = \rho \Re \int_C \sum_j \Re(-i\omega \Psi_j^R + \omega \Psi_j^I)
f_j e^{-i\omega t} n_k dS$$
$$ = \rho \Re \int_C \sum_j -i\omega f_j e^{-i\omega t} \Psi_j^R + \sum_j \omega
f_j e^{-i\omega t} \Psi_j^I n_k dS$$

On remarque que

$-i\omega f_j e^{-i\omega t} \Psi_j^R = \frac{\partial}{\partial t}(f_j
e^{-i\omega t})$, donc

$$ F_{\mbox{R}_k} = \rho \Re \int_C \sum_j \omega f_j e^{-i\omega t} \Psi_j^I + \sum_j \frac{\partial}{\partial t} f_j e^{-i\omega t}\Psi_j^R n_k dS$$

$$ = \rho\omega \sum_j\Re (f_j e^{-i\omega t})\int_C \Psi_j^I
n_k dS + \rho \sum_j \frac{d}{dt}\Re(f_j
e^{-i\omega t})\int_C\Psi_j^R n_k dS$$

On pose $U_j=\Re(f_j e^{-i\omega t})$

$$ F_{\mbox{R}_k} = \rho\omega \sum_j U_j\int_C\Psi_j^I n_k dS + \rho \sum_j
\frac{dU_j}{dt}\int_C \Psi_j^R n_k dS$$

Or d'après la condition (5) écrite pour les potentiels élémentaires de
rayonnement,

$$n_k = \frac{\partial \Psi_k^R}{\partial n}$$

On a donc :

$$ F_{\mbox{R}_k} = \rho\omega \sum_j U_j\int_C\Psi_j^I  \frac{\partial
\Psi_k^R}{\partial n} dS + \rho \sum_j
\frac{dU_j}{dt}\int_C \Psi_j^R  \frac{\partial \Psi_k^R}{\partial n} dS$$

On pose :

$${M_A}_{jk}(\omega) = -\rho \int_C \Psi_j^R \frac{\partial \Psi_k^R}{\partial n} dS$$ (en
kg)
$${B_r}_{jk}(\omega) = -\rho \omega \int_C \Psi_j^I \frac{\partial \Psi_k^R}{\partial n} dS$$ (en
kg/s)

On appelle $M_A$ matrice des masses ajoutées (qui vient de ce que le solide
déplace le fluide) et $B_r$ matrice des
amortissements dus à la radiation (termes d'amortissement non-visqueux dû à la
dispersion d'énergie par les vagues générées par le solide).

On a alors :

$$F_{\mbox{R}_k} = -\left(\sum_j U_j {B_r}_{jk}(\omega) + \sum_j \frac{dU_j}{dt}
{M_A}_{jk}(\omega)\right)$$

En prenant $U=d\eta/dt$, on obtient une formulation vectorielle des efforts dans
le domaine fréquentiel :

$$F_{\mbox{R}}(t) = -\left(M_A(\omega)\frac{d^2\eta(t)}{dt^2} + B_r(\omega)
\frac{d\eta(t)}{dt}\right)$$



## Propriétés

On peut montrer en utilisant la deuxième identité de Green, que les matrices
$M_A$ et $B_r$ obtenues précédemment sont symétriques et que la matrices $M_A$
est définie positive : on peut donc considérer $M_A$ comme une matrice d'inertie
que l'on appelle "inertie ajoutée". On peut aussi retrouver l'expression de $M_A$
de l'évaluation de l'énergie cinétique du fluide :

$$2Ec = \rho\int_{\omega} (\nabla \Phi)^2 dV = \rho \int_{\partial \Omega} \Phi
\frac{\partial \Phi}{\partial n} dS = \rho f_k f_l \int_{\partial \Omega}\Phi_k
\frac{\partial \Phi_l}{\partial n} dS = \rho {M_A}_{kl} f_k f_l$$


## Ecriture en temporel

Lorsque l'on écrit le bilan des efforts appliqués au solide, on a :

$$(M+M_A(\omega)) \ddot{X} + B_r(\omega) \dot{X} = F_{\mbox{autres}}$$

Bien que cette équation ressemble à une équation différentielle, il n'en est
rien car elle ne décrit que les mouvements en régime établi sinusoïdal : cette
équation n'est qu'une représentation de la réponse fréquentielle du navire.

Cette constatation a été faite en 1962 par W. E. Cummins, alors employé par le
David Taylor Model Basin de l'armée américaine ("The Impulse Response & Ship
Motions", Report 1661, October 1962).

Dans ce document, Cummins entreprend d'expliciter les efforts hydrodynamiques
dans le domaine temporel. Pour ce faire, il fait l'hypothèse que les mouvements
du navire sont un système linéaire à temps invariant et que, par conséquent, on
peut déduire la réponse du navire à n'importe quelle excitation de sa réponse
impulsionnelle. Il considère le potentiel de vitesse $\Theta_j(t)$ de
l'écoulement lors d'une réponse impulsionnelle suivant l'axe $j$ et le
décompose en deux composantes :

- d'une part, $\psi_j(t)$ le potentiel de vitesse (normalisé par la
  vitesse $v_j$) de l'écoulement durant une excitation impulsionnelle
  (Dirac) d'amplitude $v_j$ sur l'axe $j$,
- d'autre part, $\phi_{j}(t)$ le potentiel de vitesse (normalisé par le
  déplacement ${\Delta_x}_j$) de l'écoulement à la suite du Dirac sur l'axe $j$.

On a donc, pour une excitation impulsionnelle de l'axe $j$ :

$$\Theta = v_j\Psi_j + \phi_j(t)\Delta x_j$$

Le potentiel de vitesse dû à un mouvement arbitraire suivant l'axe $j$ s'écrit
alors :

$$\Theta = \dot{x}\Psi_j + \int_{-\infty}^t \phi_j(t-\tau)\dot{x_j}(\tau)d\tau$$

Les efforts agissant sur la carène suivant l'axe $k$ du fait d'une excitation
de l'axe $j$ peuvent s'exprimer en fonction de la pression dynamique de
l'écoulement :

$$F_{jk} = -\int_S p_j n_k dS$$

Or par définition, $p = \frac{\partial \Theta}{\partial t}$

En dérivant sous le signe intégral on obtient :

$$\frac{\partial \Theta}{\partial t} = \ddot{x_j}\Psi_j + \int_{-\infty}^t
\frac{\partial \phi_j(t-\tau)}{\partial t}\dot{x_j}(\tau)d\tau$$

Il en découle :

$-F_{jk} = \rho\int_C\ddot{x_j}\psi_j n_k dS +
\rho\int_{-\infty}^t\int_C\frac{\partial \phi_j(t-\tau)}{\partial
t}n_k dS \dot{x_j}(\tau)d\tau$

On pose :

$$A_{jk} = \rho\int_C\psi_j n_k dS$$ (masses ajoutées)
$$K_{jk}(t) = \rho\int_C\frac{\partial\phi_j(t)}{\partial t}n_k dS$$ (fonctions
de retard)

On a alors :

$$-F_{jk} = \ddot{x_j}A_{jk} + \int_{-\infty}^t K(t-\tau)\dot{x_j}(\tau)
d\tau$$

## Relation entre les formulations temporelle et fréquentielle

La formulation fréquentielle s'écrit :

$$ -F_R = M_A(\omega)\frac{d^2X}{dt^2} + B_r(\omega)\frac{dX}{dt}$$

La formulation temporelle est :

$$-F_R = A\frac{d^2X}{dt^2} + \int_{-\infty}^t
K(t-\tau)\frac{dX}{dt}(\tau)d\tau$$

Les codes potentiels tels qu'Aqua+ ou Diodore fournissent les matrices
$M_A(\omega)$ et $B_r(\omega)$ à n'importe quelle fréquence, mais qu'en est-il
des matrices $A$ et $K$ utilisées par la formulation temporelle ?

Deux ans après Cummins, en 1964, Ogilvie propose une méthode pour déterminer
les matrices $A$ et $K$ en fonction des matrices $M_A$ et $B_r$.
Pour ce faire, il considère que le mouvement du solide est oscillant de période
$\omega$ :

$X(t) = \cos(\omega t)$

En substituant dans la formulation fréquentielle, on obtient :

$$F_R = \omega^2 M_A(\omega) \cos(\omega t) + \omega B_r(\omega) \sin(\omega
t)$$

En ce qui concerne la formulation temporelle, on a :

$F_R = \omega^2 A \cos(\omega t) + \omega \int_{-\infty}^t K(t-\tau)
\sin(\omega\tau)d\tau$

En effectuant le changement de variable $\tau\leftarrow t-\tau$ on a :

$F_R = \omega^2 A \cos(\omega t) + \omega \int_0^{+\infty} K(\tau)
\sin(\omega(t-\tau))d\tau$

En développant $\sin(\omega(t-\tau)$ on obtient :

$$F_R = \omega^2\left[A - \frac{1}{\omega}\int_0^{\infty}
K(\tau)\sin(\omega\tau)d\tau\right]\cos(\omega t) +
\omega\int_0^{+\infty}K(\tau)\cos(\omega\tau)d\tau \sin(\omega t)$$

Les relations suivantes doivent donc être valables pour tout $\omega$ :

$$M_A(\omega) = A-\frac{1}{\omega}\int_0^{+\infty}K(\tau)\sin(\omega\tau)
d\tau$$
$$B_r(\omega)=\int_0^{+\infty}K(\tau)\cos(\omega\tau)d\tau$$

En faisant tendre $\omega$ vers l'infini, on a :

$$A = M_A(\infty) = \lim_{\omega\rightarrow\infty}M_A(\omega)$$

$K$ est obtenu en prenant la transformée de Fourier inverse de $B_r$ :

$$K(t) = \frac{2}{\pi}\int_0^{+\infty} B_r(\omega)\cos(\omega\tau)d\tau$$

En pratique, il est plus robuste de considérer $K$ comme une entrée du
simulateur car $B_r$ présente souvent des anomalies et donc le passage de $B_r$
à $K$ est souvent assorti d'opérations de filtrage qui dépendent de l'allure de
$B_r$.



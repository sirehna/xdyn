# Métacentre

## Définitions

* On appelle "paramètres de flottaison" et l'on note $X_f$ le triple d'états
$X_f=(z,\phi,\theta)$. $z$ est la coordonnée sur l'axe $Z$ du repère NED du
centre d'inertie $G$ et $\phi$ et $\theta$ sont les deux premiers angles des
rotations du repère NED vers le repère body. Par définition, le volume immergé
ne dépend que des paramètres de flottaison.

* On appelle "flottaison" la surface délimitée par l'intersection de la coque
et de la surface libre (au repos, pour que cette définition ait un sens).

* On appelle "axe d'inclinaison" l'intersection des surfaces libres dans le
repère "body" pour deux attitudes différentes du corps.

* Un angle d'inclinaison est l'angle d'une rotation autour d'un axe
d'inclinaison.

* Un plan d'inclinaison est un plan orthogonal à un axe d'inclinaison. Il
existe une infinité de plans d'inclinaison pour n'importe quel axe
d'inclinaison.

* On appelle "transformation isocarène" $f$ toute isométrie du navire
$f:\left\{\begin{array}{c}\mathbf{R}^3\rightarrow\mathbf{R}^3\\X_f\mapsto
f(X_f)\end{array}$ telle que le volume immergé (la carène) soit constant. Deux
paramètres de flottaison liées par une transformation isocarène sont dites
isocarènes.

* On note $V$ la fonction calculant le volume immergé (le volume de la carène)
: $$V:\left\{\begin{array}{c}\mathbf{R}^3\rightarrow\mathbf{R}^+\\X_f\mapsto
V(X_f)\end{array}$$ On suppose $V$ continue.

* On note $B$ la fonction calculant la position du centre de carène (dans le
repère NED) :
$$V:\left\{\begin{array}{c}\mathbf{R}^3\rightarrow\mathbf{R}^3\\X_f\mapsto
B(X_f)\end{array}$$

* Soit $V_{\mbox{tot}}$ le volume total du navire. On note $I$ l'intervalle de
$\mathbf{R}$ I=[0,V_{\mbox{tot}}]$

## Surface de flottaison et surface de poussée

Soit $V_0\in I$.
On appelle surface de poussée $$\mathcal{B}=V^{-1}(V_0)$$
On appelle surface de flottaison (à ne pas confondre avec la
flottaison) $$B(V^{-1}(V_0))$$. La surface de flottaison est donc le lieu des
centres de flottaison correspondant à des positions isocarènes du corps.

## Caractérisation de la surface de flottaison : théorème d'Euler

Le théorème d'Euler stipule que le centre de flottaison correspondant à une
position du flotteur est aussi le centre de flottaison d'une position isocarène
infiniment voisine. Un corollaire de ce théorème est que, les flottaisons
variant peu et possédant un point fixe (le centre de flottaison), d'après le
théorème d'Euler sur les rotations, il existe une rotation permettant de passer
de l'une à l'autre.

Le théorème d'Euler fournit aussi une autre caractérisation de la surface de
flottaison : les flottaisons isocarènes admettent une surface
enveloppe qui est touchée par chacune d'elles au centre de flottaison
correspondant. Cette surface enveloppe est la surface de flottaison.

## Caractérisation de la surface de poussée : théorème de Dupin

Le plan tangent à la surface de poussée au centre de poussée est parallèle à la
flottaison correspondante. La poussée hydrostatique est donc normale à la
surface de poussée.

Ce théorème fournit une caractérisation de la surface de poussée : les droites
d'action des poussées hydrostatiques correspondant à des paramètres de poussée
isocarènes sont les normales à une même surface qui est la surface de poussée.

## Courbe de poussée

L'intersection de la surface de poussée et d'un plan d'inclinaison s'appelle
courbe de poussée.

Pour toute position de navire, il existe une position isocarène. Soit en effet
$X_0=(z_0,\phi_0,\theta_0)$ et $X_1=(z_1,\phi_1,\theta_1)$ deux paramètres de
flottaison tels que $\phi_0\neq\phi_1$ et $\theta_0\neq\theta_1$. Soit
$V_0=V(X_0)\in I$. On note $V_z$ l'application partielle $V_z:z\mapsto
V(z,\phi_0,\theta_0)$. $V$ étant continue par hypothèse, $V_z$ l'est aussi.
Comme $V_z$ prend ses valeurs dans $I$, d'après le théorème des valeurs
intermédiaires $\exists z_2:V_z(z_2)=V_0$.

## Métacentre

Si l'on considère deux positions du navire $X_0,X_1$ et un plan d'inclinaison
quelconque (non-horizontal), on peut calculer la projection de la droite
d'action de la résultante des efforts hydrostatiques pour $X_0$ et $X_1$ sur ce
plan d'inclinaison. Les droites se coupent en un point appelé point
métacentrique. Sans hypothèse supplémentaire, ce point n'a pas d'autre
propriété particulière.

Si l'on se place dans les conditions d'applicabilité du théorème d'Euler
(positions isocarènes infiniment voisines) et que l'on suppose que l'une des
positions est une position d'équilibre, le plan contenant les centres de carène
correspondant à ces positions (notés $B_0$ et $B_1$) et $G$ est vertical. En
outre, il existe une rotation permettant de passer d'une position à l'autre. Le
plan de cette rotation n'est pas forcément le plan $(G,B_0,B_1)$. Si l'on
ajoute une hypothèse de symétrie transversale et longitudinale, alors ces deux
plans sont nécessairement confondus. Dans ce cas, le point métacentrique est à
l'intersection des droites d'action des efforts hydrostatiques (et non plus
simplement de leur projection). On peut alors écrire :

$$GZ = GM\sin(\alpha)$$, où $\alpha$ désigne l'angle de la rotation isocarène.

Lorsque l'angle d'inclinaison $\alpha$ tend vers 0, le point métacentrique $H$
tend vers un point appelé métacentre.

D'une façon plus générale, si l'on suppose la surface de poussée convexe (ce
qui n'est pas forcément le cas, comme le montre Thearle dans Theoretical Naval
Architecture), on peut l'approcher localement par une sphère osculatrice dont
le rayon est le rayon de courbure de la surface. Le centre de cette sphère est
le métacentre.


Le couple de rappel $Mr$ est par définition :

$$Mr = F_{\mbox{HS}} \cdot GZ = F_{\mbox{HS}} \cdot GM\cdot \sin(\theta)$$

Pour de petites variations isocarènes de $\theta$, le volume ne variant pas,
$F_{\mbox{HS}}$ est constante donc 

$$dMr = F_{\mbox{HS}}\cdot(\sin(\theta)dGM + GM\cdot\cos(\theta)d\theta)$$

or $$dMr = F_{\mbox{HS}}\cdot dGZ$$
d'où

$$dGZ = \sin(\theta)dGM + GM\cdot\cos(\theta)d\theta$$

Comme $GM$ est constant,

$$dGZ \sim GM d\theta$$

donc

$$GM\sim \frac{dGZ}{d\theta}$$

On a également le théorème de Bouguer :

$$GM\sim \frac{I_{Oxx}}{V}$$

## Extensions

Pour utiliser le concept de métacentre sous houle pour un navire réel, 
il faut s'affranchir des hypothèses précédentes (planéité de la surface libre,
petites rotations isocarènes, symétries). On pose alors comme définition :

$$GM=\frac{dGZ}{d\theta}$$

Le $GM$ donne donc une indication sur la stabilité du souple de rappel
hydrostatique : plus le $GM$ est important pour un volume de carène donné, plus
le moment de rappel aura tendance à varier rapidement pour de faibles
changement d'inclinaison $\theta$.

## Calcul du GM dans le simulateur

Le calcul du GM est réalisé par le simulateur à l'aide d'un modèle d'effort
particulier appelé "GM". Ce modèle ce contente de calculer GZ pour deux
positions voisines pour en déduire GM par dérivation numérique. Il utilise pour
le calcul de GZ un modèle hydrostatique (`non-linear hydrostatic (fast)` ou
`non-linear hydrostatic (exact)`) spécifié en paramètre. Il doit donc être
utilisé à l'exclusion de tout autre modèle hydrostatique.
Le modèle évalue GZ pour l'état courant et modifie ensuite l'angle $\theta$
d'une valeur d$\theta$ spécifiée en paramètre puis approche GM par

$$GM = \frac{GZ(\theta+d\theta)-GZ(\theta)}{d\theta}$$

Voici un exemple de paramétrisation :

~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
      - model: GM
        name of hydrostatic force model: non-linear hydrostatic (fast)
        roll step: {value: 1, unit: degree}
~~~~~~~~~~~~~~~~~~~~


## Références
- *Étude sur la flottabilité et la stabilité du navire suivie d’une méthode simple pour le calcul des volumes de carènes et des éléments définissant le navire pour construire les tables hydrostatiques nécessaires aux calculs de stabilité et d’assiette quand on dispose des plans des formes du navire*, Dominique Lavoille, http://vieillemarine.pagesperso-orange.fr/architecture/pages_finales/theorie_navire.pdf
- *A Treatise on the Stability of Ships*, Edward James Reed, page 89



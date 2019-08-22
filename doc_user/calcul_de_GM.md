# Métacentre

## Définitions

(définitions valables dans ce chapitre)

Le métacentre $`M`$ est un point défini comme l'intersection des axes
d'application de la force d'Archimède pour de petites variations d'inclinaison.
La distance algébrique entre le centre de gravité $`G`$ d'un navire et son
métacentre $`M`$, notée $`\overline{GM}`$, permet de caractériser la stabilité
d'un navire :

- Une valeur faible ou négative de $`\overline{GM}`$ correspond à un navire "mou",
  c'est-à-dire plus facile à incliner et qui n'aura pas tendance à revenir
  rapidement à sa position droite. Un navire mou aura plus de risque de
  chavirer lors de conditions environnementales adverses.
- Une valeur forte de $`\overline{GM}`$ correspond à un navire "raide",
  c'est-à-dire un navire assez difficile à faire gîter et qui sera soumis à un
  roulis rapide, potentiellement inconfortable pour les passagers et l'équipage.

## Utilisation dans xdyn

Le calcul du GM est réalisé par le simulateur à l'aide d'un modèle d'effort
particulier appelé "GM". Ce modèle se contente de calculer GZ pour deux
positions voisines pour en déduire GM par dérivation numérique. Il utilise pour
le calcul de GZ un modèle hydrostatique (`non-linear hydrostatic (fast)` ou
`non-linear hydrostatic (exact)`) spécifié en paramètre. Pour éviter de calculer
trois fois les efforts hydrostatiques (une fois pour la simulation elle-même et
deux fois pour le calcul du GM), il a été choisi de faire du modèle GM un modèle
hydrostatique particulier. Ainsi, il doit être utilisé à l'exclusion de tout
autre modèle hydrostatique.

Le modèle évalue GZ pour l'état courant et modifie ensuite l'angle $`\theta`$
d'une valeur d$`\theta`$ spécifiée en paramètre puis approche GM par

```math
GM = \frac{GZ(\theta+d\theta)-GZ(\theta)}{d\theta}
```

Voici un exemple de paramétrisation :

~~~~~~~~~~~~~~~~~~~~ {.yaml}
external forces:
      - model: GM
        name of hydrostatic force model: non-linear hydrostatic (fast)
        roll step: {value: 1, unit: degree}
~~~~~~~~~~~~~~~~~~~~

## Fondements théoriques

Le métacentre est défini comme l'intersection des axes d'application de la force
d'Archimède pour de petites variations d'inclinaison, ou, autrement dit, le
point où la résultante de la pression que l'eau exerce sur le navire (gîté,
c'est-à-dire incliné) rencontre le plan médian de celui-ci. Pour que cette
définition ait un sens, il convient de préciser dans quelles conditions ces axes
se coupent.

### Preuve d'existence du métacentre

Si l'on considère deux positions du navire $`X_0,X_1`$ et un [plan d'inclinaison]()
quelconque (non-horizontal), on peut calculer la projection de la droite
d'action de la résultante des efforts hydrostatiques pour $`X_0`$ et $`X_1`$ sur ce
[plan d'inclinaison](). Les droites se coupent en un point appelé point
métacentrique. Sans hypothèse supplémentaire, ce point n'a pas d'autre propriété
particulière.

Si l'on se place dans les conditions d'applicabilité du [théorème d'Euler]()
(positions [isocarènes]() infiniment voisines) et que l'on suppose que l'une des
positions est une position d'équilibre, le plan contenant les [centres de carène]()
correspondant à ces positions (notés respectivement $`B_0`$ et $`B_1`$) et le centre de gravité $`G`$ est
vertical puisque $`B_0`$ est à la verticale de $`G`$. En
outre, il existe une rotation permettant de passer d'une position à l'autre (d'après le [théorème d'Euler]()). Le
plan de cette rotation n'est pas forcément le plan $`(G,B_0,B_1)`$. Si l'on
ajoute une hypothèse de symétrie transversale et longitudinale (coque
amphidrome), alors ces deux plans sont nécessairement confondus. Dans ce cas,
le point métacentrique est à l'intersection des droites d'action des efforts
hydrostatiques (et non plus simplement de leur projection). On peut alors
écrire :


```math
GZ = GM\sin(\alpha)
```
 où $`\alpha`$ désigne l'angle de la rotation isocarène.

Lorsque l'angle d'inclinaison $`\alpha`$ tend vers 0, le point métacentrique $`H`$
tend vers un point appelé métacentre.


### Définitions

#### Plan de flottaison

Plan de la surface libre du liquide au repos

#### Flottaison

Intersection du flotteur et du plan de flottaison

#### Ligne de flottaison

Périmètre de la flottaison

#### Centre de flottaison

Centre de gravité de la flottaison

#### Carène

Partie immergée du flotteur, située au-dessous du plan de flottaison.
Son centre de gravité $`C`$ est appelé "centre de carène".
C'est le point d'application des efforts hydrostatiques.

#### Flottaisons isocarènes

Flottaisons limitant des carènes de même volume. On peut démontrer
qu'il [existe une flottaison isocarène]().

#### Axe d'inclinaison

Intersection de deux flottaisons isocarènes infiniment voisines.

#### Enveloppe des flottaisons isocarènes

Enveloppe de la famille des plans constitués de toutes les flottaisons isocarènes.

#### Surface des poussées

Lieu des centres de carène correspondant à toutes les flottaisons isocarènes.
Si l'on suppose la surface des poussées convexe (ce
qui n'est pas forcément le cas, comme le montre Thearle dans Theoretical Naval
Architecture), on peut l'approcher localement par une sphère osculatrice dont
le rayon est le rayon de courbure de la surface. Le centre de cette sphère est
le métacentre.

### Caractérisation

#### Caractérisation de la surface de flottaison : théorème d'Euler

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

#### Caractérisation de la surface de poussée : théorème de Dupin

Le plan tangent à la surface de poussée au centre de poussée est parallèle à la
flottaison correspondante. La poussée hydrostatique est donc normale à la
surface de poussée.

Ce théorème fournit une caractérisation de la surface de poussée : les droites
d'action des poussées hydrostatiques correspondant à des positions
isocarènes sont les normales à une même surface qui est la surface de poussée.


#### Existence d'une position isocarène

Pour toute position de navire, il existe une position isocarène.

Soit en effet
$`X_0=(z_0,\phi_0,\theta_0)`$ et $`X_1=(z_1,\phi_1,\theta_1)`$ deux paramètres
de flottaison tels que $`\phi_0\neq\phi_1`$ et $`\theta_0\neq\theta_1`$.
Soit $`V_{\textrm{max}}`$ le volume de la carène lorsque le navire est
totalement immergé. On note $`I`$ l'intervalle $`[0,V_{\textrm{max}}]`$.

On note $`V_z`$ l'application partielle $`V_z:z\mapsto
V(z,\phi_1,\theta_1)`$. $`V`$ étant continue par hypothèse, $`V_z`$ l'est aussi.
Par ailleurs, on sait que $`V_z(-\infty)=0`$ et $`V_z(+\infty)=V_{\textrm{max}}`$
donc d'après le théorème des valeurs intermédiaires, $`\forall V_0\in I,
\exists z_2:V_z(z_2)=V_0`$ ce qui signifie que $`X_0=(z_0,\phi_0,\theta_0)`$ et
$`X_1=(z_2,\phi_1,\theta_1)`$ sont isocarènes.


#### Couple de rappel et métacentre : théorème de Bouguer

Le couple de rappel $`M_r`$ est par définition :


```math
M_r = F_{\textrm{HS}} \cdot GZ = F_{\textrm{HS}} \cdot GM\cdot \sin(\theta)
```

Pour de petites variations isocarènes de $`\theta`$, le volume ne variant pas,
$`F_{\textrm{HS}}`$ est constante donc

```math
dMr = F_{\textrm{HS}}\cdot(\sin(\theta)dGM + GM\cdot\cos(\theta)d\theta)
```

or

```math
dMr = F_{\textrm{HS}}\cdot dGZ
```

d'où

```math
dGZ = \sin(\theta)dGM + GM\cdot\cos(\theta)d\theta
```

Comme $`GM`$ est constant,

```math
dGZ \sim GM d\theta
```

donc

```math
GM\sim \frac{dGZ}{d\theta}
```

On a également le théorème de Bouguer :

```math
GM\sim \frac{I_{Oxx}}{V}
```

### Extension : utilisation du GM en dynamique

Pour utiliser le concept de métacentre pour un navire réel sur houle,
il faut s'affranchir des hypothèses précédentes (planéité de la surface libre,
petites rotations isocarènes, symétries). On pose alors comme définition
(et non plus un approximation au premier ordre) :


```math
GM=\frac{dGZ}{d\theta}
```

Le $`GM`$ donne donc une indication sur le taux de variation du couple de rappel
hydrostatique : plus le $`GM`$ est important pour un volume de carène donné,
plus le moment de rappel aura tendance à varier rapidement pour de faibles
changements d'inclinaison $`\theta`$.

## Bibliographie

- *Theoretical Naval Architecture*, Samuel James Pope Thearle, ISBN 978-1236765673
- *A Treatise on the Stability of Ships*, Edward James Reed, page 89
- [*Mécanique des fluides: équations générales et statique des fluides*](https://cel.archives-ouvertes.fr/cel-00851021/document), Pierre Guével, École nationale supérieure de mécanique - Université de Nantes, 1969, page 113
- *Hydrodynamique navale : théorie et modèles*, 2009, Alain Bovis, Presses de l'ENSTA, page 80

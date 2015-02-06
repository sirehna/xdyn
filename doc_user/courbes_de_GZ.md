# Courbes de stabilité

## Stabilité en roulis

Parmi les performances navales, les plus importantes tant du point
pratique que théorique sont les performances en roulis et en tangage. On s'intéresse ici
plus particulièrement à la stabilité du navire en roulis. Du point de vue
réglementaire, la stabilité des navires est soumise à des conditions strictes,
définies par exemple dans les arrêtés du 28 février 1975 (bateaux non motorisés) et du 2
septembre 1970 (bateaux motorisés).

On considère un navire partiellement immergé en eau calme (la surface libre
étant donc plane et horizontale) dont le volume immergé $\Omega$ est délimité par une
surface appelée "carène" (ou surface mouillée) et notée $\mathcal{C}$.
On appelle "surface de flottaison" et l'on note $S$
l'intersection du volume du navire avec le plan de la surface libre.
On a donc $\partial\Omega = \mathcal{C}\cup S$.
On note $G$ le centre de gravité du navire et $C$ le centre de $\Omega$ (appelé
"centre de carène").

Les axes $Y$ et $Z$ du [repère "body"](#reperes-et-conventions) sont notés
respectivement $y$ et $z$. La situation peut êtr représentée par la figure
suivante :

![](images/gz.svg)

La distance algébrique $GZ$ est le bras de levier du couple de rappel
hydrostatique. Celui-ci doit être suffisant pour redresser le navire. Une
condition nécessaire et suffisante pour que le le couple de rappel
hydrostatique soit un couple de redressement est que le point métacentrique $E$
soit situé au-dessus du centre de gravité $G$. Le point $E$ est à
l'intersection de la droite $Cz$ et de la droite $C_0 z$.

## Outil de calcul de $GZ$

Parallèlement au simulateur, un autre exécutable appelé `gz` permet de calculer
$GZ$ en fonction de la gîte $\phi$.

### Algorithme




## Références
- *Hydrodynamique des Structures Offshore*, 2002, Bernard Molin, Editions TECHNIP, ISBN 2-7108-0815-3, page 398
- *Seakeeping: Ship Behaviour in Rough Weather*, 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, ISBN 0-7458-0230-3, page 191
- *Dynamique du Navire*, 1986, P. Devauchelle, Bibliothèque de L'Institut Français d'Aide à la Formation Professionnelle Maritime, ISBN 2-225-80669-1, p. 168
- *Hydrodynamique navale : théorie et modèles*, 2009, A. Bovis, Presses de l'ENSTA, p. 79
- *Stabilité des bateaux - examen d'un dossier*, CETMEF, décembre 2012, p.11

## Tutoriel 2 : oscillations en immersion

Ce tutoriel vise à illustrer l'utilisation des modèles hydrostatiques et à
comparer succinctement les modèles `non-linear hydrostatic (exact)` et
`non-linear hydrostatic (fast)`.

```python echo=False, results='raw'
yaml_data_exact_hs = load_yaml('tutorial_02_exact_hydrostatic.yml')
yaml_data_fast_hs = load_yaml('tutorial_02_fast_hydrostatic.yml')
```

### Description du problème
Dans cet exemple, nous considérons un navire,
[l'Anthineas](http://www.marinetraffic.com/en/ais/details/ships/228367000/vessel:ANTHINEAS),
soumis uniquement à la pesanteur et aux efforts hydrostatiques, sans
amortissement. Le navire est lâché sans vitesse initiale au-dessus de la
surface libre (supposée plane) et va donc réaliser des oscillations
non-amorties en immersion.

### Écriture du fichier de configuration du simulateur

Nous documentons ici uniquement les changements par rapport au
[tutoriel 1](#tutoriel-1-chute-libre).

L'environnement est défini de la façon suivante :

```python echo=False, results='raw'
print_yaml(yaml_data_exact_hs, 'environment models')
```

Comme décrit dans la [documentation du fichier
d'entrée](#absence-de-houle), ceci signifie que la
surface libre est parfaitement plane et horizontale, à la hauteur $z=0$ dans le
repère NED.

Par rapport au [tutoriel 1](#tutoriel-1-chute-libre), la
position du repère "body" par rapport au maillage est ici importante puisque
l'on [fournit un fichier STL](#d%C3%A9finition-du-fichier-de-maillage) :

```python echo=False, results='raw'
print_yaml(yaml_data_exact_hs, 'bodies/0/position of body frame relative to mesh')
```

On décrit dans les conditions initiales le fait que le bateau est lâché à 5 m
au-dessus du niveau de l'eau (l'axe z du repère NED étant orienté vers le bas,
des valeurs négatives correspondent à des points au-dessus de la surface libre)
:

```python echo=False, results='raw'
print_yaml(yaml_data_exact_hs, 'bodies/0/initial position of body frame relative to NED')
print_yaml(yaml_data_exact_hs, 'bodies/0/initial velocity of body frame relative to NED')
```

Les données dynamiques comprennent la masse, la matrice d'inertie, les inerties
ajoutées et la position du centre d'inertie :

```python echo=False, results='raw'
print_yaml(yaml_data_exact_hs, 'bodies/0/dynamics')
```

On utilise dans un premier temps le [modèle hydrostatique
approché](#calcul-du-moment) dont la
documentation est décrite
[ici](#hydrostatique-non-lin%C3%A9aire) :


```python echo=False, results='raw'
print_yaml(yaml_data_fast_hs, 'bodies/0/external forces')
```

En définitive, on obtient le fichier suivant :

```python echo=False, results='raw'
print_yaml(yaml_data_fast_hs)
```

### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :


{{exec('xdyn tutorial_02_fast_hydrostatic.yml --dt 0.1 --tend 10 -o fast.csv')}}
{{exec('xdyn tutorial_02_exact_hydrostatic.yml --dt 0.1 --tend 10 -o exact.csv')}}

### Résultats

Voici les résultats :

{% set fast_data = csv('fast.csv') %}
{% set exact_data = csv('exact.csv') %}
{% set fast_plot = prepare_plot_data(fast_data, x = 't', y = 'z(Anthineas)', name='Modèle hydrostatique rapide') %}
{% set exact_plot = prepare_plot_data(exact_data, x = 't', y = 'z(Anthineas)', name='Modèle hydrostatique exact') %}
{% set g = cartesian_graph([fast_plot, exact_plot], x='t (s)', y='Elévation (m)') %}
{{layout(size=(1,1),graphs=[(g,(0,0))], title='Elévation au cours du temps')}}

On peut également représenter les déplacements suivant l'axe $y$ en exécutant :

{% set fast_plot = prepare_plot_data(fast_data, x = 't', y = 'y(Anthineas)', name='Modèle hydrostatique rapide') %}
{% set exact_plot = prepare_plot_data(exact_data, x = 't', y = 'y(Anthineas)', name='Modèle hydrostatique exact') %}
{% set g = cartesian_graph([fast_plot, exact_plot], x='t (s)', y='y (m)') %}
{{layout(size=(1,1),graphs=[(g,(0,0))], title='Embardée au cours du temps')}}


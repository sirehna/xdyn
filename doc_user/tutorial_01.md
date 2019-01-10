## Tutoriel 1: chute libre

```python echo=False, results='raw', name='tutorial_01_load_yaml'
yaml_data = load_yaml('tutorial_01_falling_ball.yml')
```

On commence par définir les conventions de rotation :

```python echo=False, results='raw', name='tutorial_01_print_rotation_conventions'
print_yaml(yaml_data, 'rotations convention')
```

Puis l'on donne des [constantes
environnementales](#constantes-environnementales) :

```python echo=False, results='raw', name='tutorial_01_print_environmental_constants'
print_yaml(yaml_data, 'environmental constants')
```

Aucun modèle d'environnement (houle, vent...) n'est nécessaire pour cette
simulation :

```python echo=False, results='raw', name='tutorial_01_print_environment_models'
print_yaml(yaml_data, 'environment models')
```

On définit la position du repère "body" par rapport au maillage :

```python echo=False, results='raw', name='tutorial_01_print_position_of_body_frame'
print_yaml(yaml_data, 'bodies/0/position of body frame relative to mesh')
```

Les conditions initiales sont décrites comme suit :


```python echo=False, results='raw', name='tutorial_01_print_initial_conditions'
print_yaml(yaml_data, 'bodies/0/initial position of body frame relative to NED')
print_yaml(yaml_data, 'bodies/0/initial velocity of body frame relative to NED')
```

Les données dynamiques comprennent la masse, la matrice d'inertie, les inerties ajoutées
et la position du centre d'inertie :

```python echo=False, results='raw', name='tutorial_01_print_dynamics_section'
print_yaml(yaml_data, 'bodies/0/dynamics')
```

Seule la gravité agit sur le solide :

```python echo=False, results='raw', name='tutorial_01_print_external_forces_section'
print_yaml(yaml_data, 'bodies/0/external forces')
```

En définitive, on obtient le fichier suivant :

```python echo=False, results='raw', name='tutorial_01_print_full_yaml'
print_yaml_file('tutorial_01_falling_ball.yml')
```

### Lancement de la simulation

La simulation peut s'exécuter comme suit :

```python echo=False, results='raw', name='tutorial_01_launch_simulation_csv_output'
execCommand('xdyn tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -o out.csv')
```

Pour avoir des sorties sur la console, on peut faire :

```python echo=False, results='raw', name='tutorial_01_launch_simulation_console_output'
execCommand('xdyn tutorial_01_falling_ball.yml --dt 1 --tend 5 -o tsv')
```

`tsv` signifie ici "tab-separated values".

On peut également changer l'instant initial (étant entendu que les conditions
initiales définies dans le fichier YAML s'appliquent à cet instant initial,
quel qu'il soit, et non pas à t = 0) :

```python echo=False, results='raw', name='tutorial_01_sun_with_modified_initial_conditions'
execCommand('xdyn tutorial_01_falling_ball.yml --dt 0.01 --tstart 2 --tend 3 -o out.csv')
```

On peut choisir le solveur :

```python echo=False, results='raw', name='tutorial_01_change_solver'
execCommand('xdyn tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -s rk4 -o out.csv')
```

La liste de toutes les options est disponible en exécutant :

```python echo=False, results='raw', name='tutorial_01_xdyn_help'
execCommand('xdyn -h', echo_output=True)
```

### Résultats

Voici un tracé de l'élévation au cours du temps :

```python echo=False, results='raw', name='tutorial_01_plot_results'
data = csv('out.csv')
plot = prepare_plot_data(data, x='t', y='z(ball)', name='Résultat')
g = cartesian_graph([plot], x='t (s)', y='Élévation (m)')
create_layout(g, title='Élévation au cours du temps')
```

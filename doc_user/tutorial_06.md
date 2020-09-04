## Tutoriel 6 : propulsion

Jusqu'ici nous n'avons simulé que des efforts environnementaux. Dans ce
tutoriel, nous simulons un propulseur.

### Description du problème

Le navire évolue dans un environnement sans houle. Il est soumis aux cinq
efforts suivants :

- La pesanteur
- Les efforts hydrostatiques (rapides et non exacts)
- L'amortissement visqueux
- Un effort de propulsion dû à une hélice
- La résistance à l'avancement

On peut aussi se contenter des 2 seuls efforts de résistance et de propulsion.

### Écriture du fichier de configuration du simulateur

Les changements par rapport au tutoriel 2 sont les ajouts des efforts d'amortissement
et de résistance, d'une section 
[`controlled forces`](#efforts-commandés) et d'une section
[`commands`](#syntaxe-des-commandes).

On commence par définir les [caractéristiques du propulseur](#efforts-commandés) :

```python echo=False, results='raw', name='tutorial_06_load_yaml'
yaml_data = load_yaml('tutorial_06_1D_propulsion.yml')
```

```python echo=False, results='raw', name='tutorial_06_print_controlled_forces_section'
print_yaml(yaml_data, 'bodies/0/controlled forces')
```

Les commandes sont définies à la racine du YAML:

```python echo=False, results='raw', name='tutorial_06_print_commands_section'
print_yaml(yaml_data, 'commands')
```

En définitive, le fichier d'entrée est :

```python echo=False, results='raw', name='tutorial_06_print_full_yaml'
print_yaml(yaml_data)
```

### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

```python echo=False, results='raw', name='tutorial_06_launch_simulation'
execCommand('xdyn tutorial_06_propulsion.yml --dt 0.1 --tend 20 -o out.csv')
```

### Résultats

Voici l'évolution temporelle de la vitesse d'avance :

```python echo=False, results='raw', name='tutorial_06_plot_results'
data = csv('out.csv')
plot = prepare_plot_data(data, x = 't', y = 'u(TestShip)', name="Vitesse d'avance")
g = cartesian_graph([plot], x='t (s)', y='U (m/s)')
create_layout(graph=g, title="Vitesse d'avance longitudinale")
```

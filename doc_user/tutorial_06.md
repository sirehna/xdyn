## Tutoriel 6 : propulsion

Jusqu'ici nous n'avons simulé que des efforts environnementaux. Dans ce
tutoriel, nous simulons un propulseur.

### Description du problème

Le navire évolue dans un environnement sans houle. Il est soumis aux quatre
efforts suivants :

- La pesanteur
- Les efforts hydrostatiques (rapides et non exacts)
- L'amortissement visqueux
- Un effort de propulsion

### Écriture du fichier de configuration du simulateur

Les deux seuls changements par rapport au tutoriel 2 sont l'ajout d'une section
[`controlled forces`](#efforts-commandés) et un
[fichier supplémentaire de commandes](#syntaxe-du-fichier-de-commande)
que l'on nomme `controlled_forces_commands.yml`.

On commence par définir les [caractéristiques du propulseur](#efforts-commandés) :

```python echo=False, results='raw'
yaml_data = load_yaml('tutorial_06_1D_propulsion.yml')
```

```python echo=False, results='raw'
print_yaml(yaml_data, 'bodies/0/controlled forces')
```

Les commandes sont définies à la racine du YAML:

```python echo=False, results='raw'
print_yaml(yaml_data, 'commands')
```

En définitive, le fichier d'entrée est :

```python echo=False, results='raw'
print_yaml(yaml_data)
```

### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

```python echo=False, results='raw'
execCommand('xdyn tutorial_06_1D_propulsion.yml --dt 0.1 --tend 20 -o out.csv')
```

### Résultats

```python echo=False, results='raw'
data = csv('out.csv')
plot = prepare_plot_data(data, x = 't', y = 'u(Anthineas)', name="Vitesse d'avance")
g = cartesian_graph([plot], x='t (s)', y='U (m/s)')
create_layout(graph=g, title="Vitesse d'avance longitudinale")
```

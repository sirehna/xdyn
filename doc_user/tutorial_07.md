## Tutoriel 7 : utilisation d'un modèle de houle distant

Ce tutoriel explique comment utiliser un modèle de houle externe
dans xdyn.

Nous utiliserons Docker compose pour lancer le client (xdyn) et le
serveur de houle.


### Lancement de la simulation

La simulation peut maintenant être lancée comme suit :

```python echo=False, results='raw', name='tutorial_07_launch_simulation'
execCommand('docker-compose up --exit-code-from xdyn --abort-on-container-exit')
```

- `--exit-code-from xdyn` signifie que le résultat de cette commande (succès ou
  erreur) est le résultat du lancement de xdyn : si la simulation se passe
  correctement, cette commande réussira. Elle échouera dans le cas contraire
- `--abort-on-container-exit` : demande à `docker-compose` de rendre la main
  quand la simulation se termine

Pour plus de détails, la [documentation de
`docker-compose`](https://docs.docker.com/compose/overview/) est assez
complète.

### Résultats

Voici l'évolution temporelle de la vitesse pour la cas 1D :

```python echo=False, results='raw', name='tutorial_07_plot_results'
data = csv('out.csv')
plot = prepare_plot_data(data, x = 't', y = 'u(TestShip)', name="Vitesse d'avance")
g = cartesian_graph([plot], x='t (s)', y='U (m/s)')
create_layout(graph=g, title="Vitesse d'avance longitudinale")
```

# Interface MatLab

`X-Dyn` peut être appelé depuis le logiciel `MatLab`.
Cela présente l'avantage de disposer d'un environnement graphique pour afficher
les résultats de simulation.

Voici les fonctions de base pour travailler avec `X-Dyn`.

- `xdyn_run` éxécute XDyn depuis `MatLab`.
- `xdyn_loadResultsFromHdf5File.m` permet de charger les résultats d'une
  simulation dans `MatLab`.
- `xdyn_postProcess.m` lance l'ensemble des post-traitements disponibles.
- `xdyn_plotStates.m` permet de tracer les états, à partir de résultats de
  simulations. Cela comprend les positions et les vitesses de chaque corps.
- `xdyn_plotPositions.m` permet de tracer les positions et les orientations
   de chaque corps.
- `xdyn_plotVelocities.m` permet de tracer les vitesses de translation et de
   rotation de chaque corps dans le repère de chaque repère.
- `xdyn_animate3d.m` permet de lancer une animation 3d d'une simulation avec
  les objets simulés et le champ de vagues lorsque celui-ci est exporté.

Enfin, le fichier `xdyn_demos.m` permet de lancer les différents tutoriels.

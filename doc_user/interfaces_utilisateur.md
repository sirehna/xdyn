
# Ligne de commande

## Liste des arguments

```python echo=False, results='verbatim', name='xdyn-command-line-arguments'
from subprocess import check_output
import re
r=re.compile(r"This is a.*USAGE:", re.DOTALL)
print(re.sub(r,"", check_output(['xdyn']).decode('utf-8')))
```

## Exemples

Les exemples suivants peuvent être lancés à partir du répertoire `demos`:

### Simulation avec un solveur Runge-Kutta d'ordre 4 en commençant à t=0

~~~~~~~~~~~~~~~~~~~~ {.bash}
./xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1
~~~~~~~~~~~~~~~~~~~~

ou si l'on utilise une invite de commande MS-DOS :

~~~~~~~~~~~~~~~~~~~~ {.bash}
xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1
~~~~~~~~~~~~~~~~~~~~

En ajoutant l'option -o csv, les sorties se font sur la sortie standard
(le terminal). Ceci permet de chaîner les traitements (pipe UNIX), par exemple :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o csv| python plot.py test 0 3
~~~~~~~~~~~~~~~~~~~~

La commande précédente lance la simulation et génère un tracé (format SVG) à
l'aide du
[script python de post-traitement](https://gitlab.sirehna.com/root/xdyn/blob/master/postprocessing/Python/plot.py)
livré avec le simulateur. Des scripts de post-traitement sont livrés avec
l'installation du simulateur, pour
[Matlab](https://gitlab.sirehna.com/root/xdyn/tree/master/postprocessing/MatLab),
[Python](https://gitlab.sirehna.com/root/xdyn/tree/master/postprocessing/Python).

### Simulation avec un solveur Euler en commençant à t=1

~~~~~~~~~~~~~~~~~~~~ {.bash}
./xdyn tutorial_01_falling_ball.yml -s euler --dt 0.1 --tstart 1 --tend 1.2
~~~~~~~~~~~~~~~~~~~~

# Documentations des données d'entrées du simulateur

Les données d'entrées du simulateur se basent sur un format
de fichiers [YAML](http://www.yaml.org/) qui fonctionne par clef-valeur.

Ce format présente l'avantage d'être facilement lisible et éditable. Des
parsers YAML existent pour de nombreux langages de programmation.

## Vue d'ensemble

### Liste des sections

Le fichier YAML comprend quatre sections de haut niveau :

- la section `rotations convention` définit la convention d'angles utilisée,
- `environmental constants` donne les valeurs de la gravité et la densité de
  l'eau,
- les modèles environnementaux figurent dans `environment models`,
- `bodies` décrit les corps simulés.

### Remarques sur les unités

On retrouve fréquemment dans le fichier YAML des lignes du type :

~~~~~~~~~~~~~~ {.yaml}
clef: {value: 65456, unit: km}
~~~~~~~~~~~~~~

Les unités ne sont pas vérifiées par le système : le parser se contente de
convertir toutes les entrées en unité du système international avant simulation,
sans préjuger de l'homogénéité. Ce décodage est fait en convertissant l'unité
en un facteur multiplicatif (en utilisant la liste des unités de l'utilitaire UNIX
[units](http://heirloom.cvs.sourceforge.net/viewvc/heirloom/heirloom/units/)) et
en multipliant la valeur originale par ce facteur multiplicatif. Par exemple:

~~~~~~~~~~~~~~ {.yaml}
masse: {value: 10, unit: lb}
~~~~~~~~~~~~~~

sera vu par xdyn comme:

````{cpp}
masse = 4.5359237;
````
L'exemple précédent :

~~~~~~~~~~~~~~ {.yaml}
clef: {value: 65456, unit: km}
~~~~~~~~~~~~~~

aurait tout aussi bien pu être écrit :

~~~~~~~~~~~~~~ {.yaml}
clef: {value: 65456, unit: kW}
~~~~~~~~~~~~~~

et on aurait obtenu exactement la même valeur numérique, bien que la grandeur
physique ne soit pas la même : on suppose donc que l'utilisateur renseigne des
données de façon homogène. En interne, tous les calculs sont faits en unité du
système international.

## Sorties

La spécification des sorties se fait au moyen de la section `output`, à la
racine du fichier YAML, dont voici un exemple :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
output:
   - format: csv
     filename: test.csv
     data: [t, x(ball), 'Fx(gravity,ball)']
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `format` : `csv` pour un fichier texte dont les colonnes sont séparées par
  une virgule ou `hdf5` pour le format des fichiers .mat de MatLab (HDF5)
- `filename` : nom du fichier de sortie
- `data` : liste des colonnes à écrire. Le temps est noté `t`, et les états
  sont `x(body)`, `y(body)` `z(body)`, `u(body)`, `v(body)`, `w(body)`,
  `p(body)`, `q(body)`, `r(body)`, `qr(body)`, `qi(body)`, `qj(body)`,
  `qk(body)`. `body` doit être remplacé par le nom du corps (`ball` dans
  l'exemple ci-dessus). Les sorties d'effort sont `Fx(modèle,corps,repère)`,
  `Fy(modèle,corps,repère)`, `Fz(modèle,corps,repère)`,
  `Mx(modèle,corps,repère)`, `My(modèle,corps,repère)`,
  `Mz(modèle,corps,repère)` où `modèle` est le nom du modèle d'effort
  (renseigné dans la clef `modèle` de chaque modèle d'effort),
  `corps` est le nom du corps sur
  lequel agit l'effort et `repère` est le repère d'expression (qui ne peut être
  que `NED` ou le nom du corps). Les sorties de houle sont notées
  `waves` et leur contenu est décrit dans la section [Modèle de
  houle/Sorties](#sorties-1). La somme des efforts appliqués à un corps est
  accessible par `Fx(sum of forces,corps,repère)` (resp. Fy, Fz, Mx, My, Mz).

# Interface MatLab

`xdyn` peut être appelé depuis le logiciel `MatLab`.
Cela présente l'avantage de disposer dans la foulée d'un environnement graphique
pour afficher les résultats de simulation.

Voici les fonctions de base pour travailler avec `xdyn`.

- `xdyn_run` exécute `xdyn` depuis `MatLab`.
- `xdyn_loadResultsFromHdf5File` permet de charger les résultats d'une
  simulation dans `MatLab`.
- `xdyn_postProcess` lance l'ensemble des post-traitements disponibles.
- `xdyn_plotStates` permet de tracer les états, à partir de résultats de
  simulations. Cela comprend les positions et les vitesses de chaque corps.
- `xdyn_plotPositions` permet de tracer les positions et les orientations
   de chaque corps.
- `xdyn_plotVelocities` permet de tracer les vitesses de translation et de
   rotation de chaque corps dans le repère de chaque repère.
- `xdyn_animate3d` permet de lancer une animation 3d d'une simulation avec
  les objets simulés et le champ de vagues lorsque celui-ci est exporté.
- `xdyn_extractMatLabScript` permet d'extraire les scripts `MatLab` de
  dépouillement intégrés dans le fichier de sortie hdf5.

Enfin, le fichier `xdyn_demos.m` permet de lancer les différents tutoriels.

Il est également possible d'utiliser l'environnement `MatLab` pour analyser des
simulations déjà réalisées.
Par exemple, le script `xdyn_demos.m` va lire le fichier
`tutorial_01_falling_ball.h5` qui aura préalablement été généré à partir
de la commande :

~~~~~~~~~~~~~~~~~~~~ {.bash}
./xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o tutorial_01_falling_ball.h5
~~~~~~~~~~~~~~~~~~~~

# Interface Docker et génération automatique de rapports

`xdyn` peut être utilisé sous la forme d'un conteneur
[Docker](https://www.docker.com/)
ce qui permet de l'exécuter sur n'importe quelle plateforme.

En outre, c'est actuellement le seul moyen d'utiliser le module de génération
automatique de rapport décrit ci-après.

Pour utiliser l'image `xdyn`, il faut d'abord installer Docker puis récupérer
l'image `sirehna/xdyn` en exécutant :

~~~~{.bash}
docker pull sirehna/xdyn
~~~~

Pour utiliser l'image `xdyn` avec son environnement de post-traitement,
on récupérera l'image `sirehna/xweave` en exécutant :

~~~~{.bash}
docker pull sirehna/xweave
~~~~

## Lancement d'xdyn via l'image docker

Une fois l'image chargée par la commande précédente, on peut lancer :

~~~~{.bash}
docker run -it --rm -v $(pwd):/work -w /work sirehna/xdyn
~~~~

- le paramètre `-v $(pwd):/work` permet de faire correspondre le répertoire courant avec le répertoire `/work` du conteneur,
- le paramètre `-w /work` précise que le répertoire de travail du conteneur (celui depuis lequel sera exécuté xdyn) est `/work`,
- `sirehna/xdyn` correspond au nom de l'image Docker,

Si l'on souhaite lancer une simulation, on ajoute les arguments d'xdyn à la suite :

~~~~{.bash}
docker run -it --rm -v $(pwd):/work -w /work sirehna/xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o tsv
~~~~

## Génération automatique de rapports (X-Weave)

Cette fonctionnalité n'est accessible qu'en utilisant le conteneur Docker
`sirehna/xweave`.


### Principe de fonctionnement

On utilise [Pweave](http://mpastell.com/pweave/) pour inclure des balises Python
dans du texte au format [Markdown](https://pandoc.org/MANUAL.html#pandocs-markdown)).
Par exemple, la présente documentation est générée en utilisant
ce système, y compris les tutoriels et leurs courbes de résultats.

Grâce à ce générateur (appelé "x-weave"), on peut obtenir des rapports dans le
format que l'on souhaite (DOCX, PDF, HTML...). Ces rapports peuvent par exemple
lancer des simulations xdyn et inclure des courbes à partir de ces résultats.

L'intérêt est que l'on peut gérer le code de ses rapports en configuration (puisque
c'est du texte), regarder facilement les différences entre deux versions sans
utiliser d'outil propriétaire et regénérer à la demande une nouvelle version
du rapport lorsque l'on change les entrées.

X-Weave n'est ni plus ni moins que Pweave dans un containeur Docker avec xdyn
pré-installé, ce qui permet de lancer xdyn depuis Pweave.

### Ligne de commande

La ligne de commande à utiliser est :

~~~~{.bash}
docker run --rm -it -v $(pwd):/work -w /work -u $(id -u):$(id -g) --entrypoint /usr/bin/xdyn-weave sirehna/xweave -f markdown concatenated_doc.pmd -o concatenated_doc.md
~~~~

- La première partie de la ligne de commande (`docker run --rm -it -v
    $(pwd):/work -w /work -u $(id -u):$(id -g) --entrypoint /usr/bin/xdyn-weave
    sirehna/xweave`) contient les arguments passés à Docker pour partager le
    dossier courant avec le containeur et créer les fichiers de sortie avec les
    permissions de l'utilisateur courant.
- La partie après `sirehna/xweave` contient les arguments passés à pweave. Une
    liste complète de ces arguments peut être obtenue en exécutant `docker run
    --rm -it -v $(pwd):/work -w /work -u $(id -u):$(id -g) --entrypoint
    /usr/bin/xdyn-weave sirehna/xweave -h` ou en consultant [la documentation
    de Pweave](http://mpastell.com/pweave/docs.html).

On obtient alors un fichier Markdown et (le cas échéant) des images. On peut
ensuite utiliser [Pandoc](https://pandoc.org/) pour convertir le Markdown au
format de son choix en utilisant le Pandoc installé dans le containeur X-Weave
:

~~~~{.bash}
docker run --rm -it -v $(shell pwd):/work -w /work -u $(shell id -u):$(shell id -g) --entrypoint /usr/bin/pandoc xweave concatenated_doc_pandoc.md -o doc.html
~~~~

### Commandes Python supplémentaires

Par rapport à Pweave "classique", on a inclus quelques fonctions Python pour
simplifier le travail avec xdyn.

#### Chargement du fichier YAML : `load_yaml`

L'insertion de la section suivante dans un document X-weave ne génèrera pas de
sortie. Par contre, dans les sections `python` ultérieures, on pourra utiliser
les données ainsi chargées.

```python echo=False, results='markdown', name='example-load-yaml'
print('~' * 4 + '{.markdown}')
print('`' * 3 + 'python')
print("yaml_data = load_yaml('tutorial_06_1D_propulsion.yml')")
print('`' * 3)
print('~' * 4)
```

#### Affichage de l'intégrabilité du YAML : `print_yaml`

Suite au chargement du YAML, on peut vouloir l'afficher dans le document.
Pour cela, on utilise une section `python` de la forme :


```python echo=False, results='markdown', name='example-print-yaml'
print('~' * 4 + '{.markdown}')
print('`' * 3 + 'python')
print("yaml_data = load_yaml('tutorial_06_1D_propulsion.yml')")
print("")
print("# Pour afficher l'intégralité du YAML avec de la coloration syntaxique")
print("print_yaml(yaml_data)")
print("")
print("# Pour n'afficher qu'une sous-section du YAML")
print("print_yaml(yaml_data, 'bodies/0/controlled forces')")
print('`' * 3)
print('~' * 4)
```

#### Exécution d'une simulation

La commande suivante ne produira pas de contenu visible dans le document. En
revanche, il sera possible de récupérer et afficher les données ainsi générées.

```python echo=False, results='markdown', name='example-run-simulation'
print('~' * 4 + '{.markdown}')
print('`' * 3 + 'python')
print("execCommand('xdyn tutorial_01_falling_ball.yml --dt 0.01 --tend 1 -o out.csv')")
print('`' * 3)
print('~' * 4)
```

#### Tracé des résultats

```python echo=False, results='markdown', name='example-plot-results'
print('~' * 4 + '{.markdown}')
print('`' * 3 + 'python')
print('# Récupération des données générées')
print("data = csv('out.csv')")
print('')
print('# Définition de la donnée à tracer')
print("plot = prepare_plot_data(data, x = 't', y = 'z(ball)', name='Résultat')")
print('')
print('# Définition du type de graph')
print("g = cartesian_graph([plot], x='t (s)', y='Élévation (m)')")
print('')
print('# Tracé de la planche')
print("create_layout(g, title='Élévation au cours du temps')")
print('`' * 3)
print('~' * 4)
```

## Utilisation de xdyn en serveur websocket

### Description

Il est possible de lancer xdyn en tant que serveur, afin de l'intégrer à un
autre environnement de simulation. L'utilisation d'un serveur plutôt qu'une
[MEX-fonction](https://www.mathworks.com/help/matlab/matlab_external/introducing-mex-files.html) (*MATLAB executable* - fonction) ou un [FMU](https://fmi-standard.org/) (*Functional Mock-up Unit*) permet d'exécuter xdyn sur une autre machine et de
l'interroger par plusieurs clients.

Il s'agit d'une utilisation en "model exchange" (au sens de la [spécification
"Functional Mockup
Interface"](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=2ahUKEwimvuWL6tDeAhUC1hoKHWiwALAQFjABegQIBBAC&url=https%3A%2F%2Fsvn.modelica.org%2Ffmi%2Fbranches%2Fpublic%2Fspecifications%2Fv2.0%2FFMI_for_ModelExchange_and_CoSimulation_v2.0.pdf&usg=AOvVaw2ePLxrLtnb42qW1aLIVoov)),
par opposition à "xdyn for Co-simulation". La différence se situe dans
l'utilisation du solveur : dans le cas de la co-simulation, on utilise le
solveur interne de xdyn (le serveur renvoie les états intégrés au pas suivant
$`X(t+dt)`$). Dans le cas "model exchange", le serveur renvoie la dérivée
des états $`\frac{dX}{dt}`$.

### Justification technique

L'utilisation des websockets permet des temps de réponse plus courts (puisque
c'est un protocole assez léger, comparé au HTTP par exemple). Dans
l'implémentation actuelle, les messages envoyés sont en JSON, pour offrir un
bon compromis entre la verbosité (moins que du XML mais plus qu'un format
binaire) et une utilisation plus aisée qu'un format type [Protobuf](https://developers.google.com/protocol-buffers/) ou [Thrift](https://thrift.apache.org/),
quitte à sacrifier un peu de performance (taille des messages, temps d'encodage/décodage).

### Lancement du serveur "Model Exchange"

Le serveur est lancé grâce à l'exécutable `xdyn-for-me` (xdyn for Model Exchange)
avec un ou plusieurs fichiers YAML en paramètre : une fois lancé, ce
serveur ne simulera donc qu'une seule configuration de l'environnement et du
navire. Concrètement, on lance le serveur comme suit :

~~~~{.bash}
./xdyn-for-me --port 9002 tutorial_01_falling_ball.yml
~~~~

où `--port` sert à définir le port sur lequel écoute le serveur websocket.

La liste complète des options avec leur description est obtenue en lançant
l'exécutable avec le flag `-h`.

Ensuite, on peut se connecter à l'adresse du serveur pour l'interroger.

### Lancement du serveur "Co-Simulation"

Le serveur est lancé grâce à l'exécutable `xdyn-for-cs` (xdyn for
Co-Simulation) avec un ou plusieurs fichiers YAML en paramètre, un pas de temps
et un solveur : une fois lancé, ce serveur ne simulera donc qu'une seule
configuration de l'environnement et du navire, en utilisant un solveur et
un pas de temps. Concrètement, on lance le serveur comme suit :

~~~~{.bash}
./xdyn-for-cs --port 9002 tutorial_01_falling_ball.yml --dt 0.1
~~~~

où `--port` sert à définir le port sur lequel écoute le serveur websocket.

La liste complète des options avec leur description est obtenue en lançant
l'exécutable avec le flag `-h`.

Ensuite, on peut se connecter à l'adresse du serveur pour l'interroger.


### Utilisation avec Chrome

Le navigateur Chrome dispose d'une extension websocket [Simple Websocket
Client](https://chrome.google.com/webstore/detail/simple-websocket-client/pfdhoblngboilpfeibdedpjgfnlcodoo?hl=en)
qui permet de faire quelques tests de bon fonctionnement.

### Utilisation avec Matlab

On initie une connexion websocket via MATLAB en utilisant par exemple
[MatlabWebSocket](https://github.com/jebej/MatlabWebSocket).
Il faut également pouvoir encoder et décoder du JSON en MATLAB, par exemple en
utilisant les fonctions MATLAB [jsondecode et
jsonencode](https://fr.mathworks.com/help/matlab/json-format.html).

### Description des entrées/sorties pour une utilisation en "Model Exchange" (x -> dx/dt)

Dans ce mode, xdyn calcule uniquement la dérivée des 13 états navire mais
n'effectue pas l'intégration numérique, ce qui permet d'utiliser un solveur
externe, par exemple Matlab ou Simulink.

| Entrées    | Type                                                             | Détail                                                                                                                                                                                                                                                                  |
| ---------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `states`   | Liste d’éléments de type « État »                                | Historique des états jusqu’au temps courant t. Si les modèles utilisés ne nécessitent pas d'historique, cette liste peut n'avoir qu'un seul élément.                                                                                                                    |
| `commands` | Liste de clefs-valeurs (dictionnaire)                            | État des actionneurs au temps t                                                                                                                                                                                                                                         |

Chaque élément de type « État » est composé des éléments suivants:

| État  | Type      | Détail                                                                                                                                                                                                                                                                  |
| ----- | --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `t`   | Flottant  | Temps courant de la simulation                                                                                                                                                                                                                                          |
| `x`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du [repère body](#rep%C3%A8re-navire-mobile-ou-body-ou-rep%C3%A8re-de-r%C3%A9solution) |
| `y`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY                                                                         |
| `z`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY                                                                         |
| `u`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                                                                                             |
| `v`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                                                                                             |
| `w`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                                                                                             |
| `p`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                                                                                                 |
| `q`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                                                                                                 |
| `r`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                                                                                                 |
| `qr`  | Flottant  | Partie réelle du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                                                                                             |
| `qi`  | Flottant  | Première partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                                                                                |
| `qj`  | Flottant  | Seconde partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                                                                                 |
| `qk`  | Flottant  | Troisième partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                                                                               |

Exemple d'entrée:

~~~~{.json}
{
  "states": [
    {
      "t": 0,
      "x": 0,
      "y": 8,
      "z": 12,
      "u": 1,
      "v": 0,
      "w": 0,
      "p": 0,
      "q": 1,
      "r": 0,
      "qr": 1,
      "qi": 0,
      "qj": 0,
      "qk": 0
    },
    {
      "t": 0.1,
      "x": 0.1,
      "y": -0.156,
      "z": 10,
      "u": 0,
      "v": 0,
      "w": 0,
      "p": 0,
      "q": 0,
      "r": 0,
      "qr": 0,
      "qi": 0,
      "qj": 0,
      "qk": 0,
    }
  ],
  "commands": {
    "beta": 0.1
  }
}
~~~~

La sortie du "Model Exchange" correspond à la dérivée des états
à savoir `dx/dt`, `dy/dt`, `dz/dt`, `du/dt`, `dv/dt`, `dw/dt`,
`dp/dt`, `dq/dt`, `dr/dt`, `dqr/dt`, `dqi/dt`, `dqj/dt`, `dqk/dt`.

| Sorties   | Type      | Détail                                                                                                                                                                                                                             |
| --------- | --------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `dx/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `dy/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `dz/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `du/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `dv/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `dw/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `dp/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `dq/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `dr/dt`   | Flottant  | Dérivée par rapport au temps de la projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `dqr/dt`  | Flottant  | Dérivée par rapport au temps de la partie réelle du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                     |
| `dqi/dt`  | Flottant  | Dérivée par rapport au temps de la première partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                        |
| `dqj/dt`  | Flottant  | Dérivée par rapport au temps de la seconde partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                         |
| `dqk/dt`  | Flottant  | Dérivée par rapport au temps de la troisième partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                       |

Exemple de sortie:

~~~~{.json}
{
  "dx_dt": 0.1,
  "dy_dt": -0.156,
  "dz_dt": 10,
  "du_dt": 0,
  "dv_dt": 0,
  "dw_dt": 0,
  "dp_dt": 0,
  "dq_dt": 0,
  "dr_dt": 0,
  "dqr_dt": 0,
  "dqi_dt": 0,
  "dqj_dt": 0,
  "dqk_dt": 0
}
~~~~

La représentation textuelle de ces nombres flottants est faite façon unique (et non
exacte) : cela signifie que si les flottants sont différents (binairement),
leur représentation textuelle sera différente. Cela signifie également que si xdyn lit
la représentation tectuelle et la traduit en binaire, on retrouvera bien la valeur binaire
initiale. En d'autres termes, la fonction flottant -> texte est injective. Cela
n'implique pas qu'elle soit bijective, puisque si l'on part d'une
représentation textuelle, que l'on convertit en binaire pour reconvertir ensuite en
texte on ne retrouvera pas nécessairement le texte initial.

### Description des entrées/sorties pour une utilisation en "Co-Simulation" (x(t) -> [x(t), ...,x(t+Dt)])

| Entrées    | Type                                   | Détail                                                                                                                                                  |
| ---------- | -------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Dt`       | Flottant strictement positif           | Horizon de simulation (en secondes). La simulation s'effectue de t0 à t0 + Dt, où t0 est la date du dernier élément de la                               |
|            |                                        | liste `states`, par pas de `dt`, où `dt` est spécifié sur la ligne de commande. L'état t0 est donc présent à                                            |
|            |                                        | la fois dans les entrées et dans les sorties, avec la même valeur.                                                                                      |
| `states`   | Liste d'éléments de type « État »      | Historique des états jusqu'au temps courant `t`. Si les modèles utilisés ne nécessitent pas d'historique, cette liste peut n'avoir qu'un seul élément.  |
| `commands` | Liste de clefs-valeurs (dictionnaire)  | État des actionneurs au temps `t`. Commande au sens de xdyn (modèle d'effort commandé) au temps t0 (début de la                                        |
|            |                                        | simulation, i.e. date du dernier élément de la liste `states`). Le plus souvent, correspond à l'état interne                                            |
|            |                                        | d'un modèle d'actionneur (safran ou hélice par exemple) dans xdyn et dont on souhaite simuler la dynamique                                             |
|            |                                        | en dehors d'xdyn.

Chaque élément de type « État » est composé des éléments suivants:

| État  | Type      | Détail                                                                                                                                                                                          |
| ----- | --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `t`   | Flottant  | Date de l'état                                                                                                                                                                                  |
| `x`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `y`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `z`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `u`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `v`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `w`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `p`   | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `q`   | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `r`   | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `qr`  | Flottant  | Partie réelle du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                     |
| `qi`  | Flottant  | Première partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                        |
| `qj`  | Flottant  | Seconde partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                         |
| `qk`  | Flottant  | Troisième partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                       |

Exemple d'entrée:

~~~~{.json}
{
  "Dt": 10,
  "states": [
    {
      "t": 0,
      "x": 0,
      "y": 8,
      "z": 12,
      "u": 1,
      "v": 0,
      "w": 0,
      "p": 0,
      "q": 1,
      "r": 0,
      "qr": 1,
      "qi": 0,
      "qj": 0,
      "qk": 0
    },
    {
      "t": 0,
      "x": 0.1,
      "y": -0.156,
      "z": 10,
      "u": 0,
      "v": 0,
      "w": 0,
      "p": 0,
      "q": 0,
      "r": 0,
      "qr": 0,
      "qi": 0,
      "qj": 0,
      "qk": 0,
    }
  ],
  "commands": {
    "beta": 0.1
  }
}
~~~~

La sortie est une liste d'éléments de type « État augmenté » contenant
l'historique des états de `t0` à `t0 + Dt` par pas de `dt`.

| État     | Type      | Détail                                                                                                                                                                                          |
| -------- | --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `t`      | Flottant  | Date de l'état augmenté                                                                                                                                                                         |
| `x`      | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `y`      | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `z`      | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur entre l'origine du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) et l'origine du repère BODY |
| `u`      | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `v`      | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `w`      | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse du navire par rapport au sol (BODY/NED).                                                     |
| `p`      | Flottant  | Projection sur l'axe X du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `q`      | Flottant  | Projection sur l'axe Y du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `r`      | Flottant  | Projection sur l'axe Z du [repère NED](#rep%C3%A8re-de-r%C3%A9f%C3%A9rence-ned) du vecteur vitesse de rotation du navire par rapport au sol (BODY/NED).                                         |
| `qr`     | Flottant  | Partie réelle du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                                     |
| `qi`     | Flottant  | Première partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                        |
| `qj`     | Flottant  | Seconde partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                         |
| `qk`     | Flottant  | Troisième partie imaginaire du quaternion définissant la rotation du navire par rapport au sol (BODY/NED)                                                                                       |
| `phi`    | Flottant  | Angle d'Euler phi. Lire note ci-après                                                                                                                                                           |
| `theta`  | Flottant  | Angle d'Euler phi. Lire note ci-après                                                                                                                                                           |
| `psi`    | Flottant  | Angle d'Euler phi. Lire note ci-après                                                                                                                                                           |

La signification exacte des angles d'Euler dépend de la convention d'angle
choisie dans le fichier YAML d'entrée de xdyn (voir la section correspondante
dans la documentation). Cette sortie est fournie pour faciliter
le travail du client du serveur, mais n'est pas utilisée en interne par xdyn.

Exemple de sortie:

~~~~{.json}
[
  {
    "t": 10,
    "x": 0,
    "y": 8,
    "z": 12,
    "u": 1,
    "v": 0,
    "w": 0,
    "p": 0,
    "q": 1,
    "r": 0,
    "qr": 1,
    "qi": 0,
    "qj": 0,
    "qk": 0,
    "phi": 0,
    "theta": 0,
    "psi": 0
  },
  {
    "t": 10.1,
    "x": 0.0999968,
    "y": 8,
    "z": 12.0491,
    "u": 0.897068,
    "v": 0,
    "w": 1.07593,
    "p": 0,
    "q": 1,
    "r": 0,
    "qr": 0.99875,
    "qi": 0,
    "qj": 0.0499792,
    "qk": 0,
    "phi": 0,
    "theta": 0.1,
    "psi": 0
  }
]
~~~~

Comme pour le "model exchange", la représentation textuelle de ces nombres
flottants est faite façon unique (et non exacte) : cela signifie que si les
flottants sont différents (binairement), leur représentation textuelle sera
différente. Cela signifie également que si xdyn lit la représentation tectuelle
et la traduit en binaire, on retrouvera bien la valeur binaire initiale. En
d'autres termes, la fonction flottant -> texte est injective. Cela n'implique
pas qu'elle soit bijective, puisque si l'on part d'une représentation
textuelle, que l'on convertit en binaire pour reconvertir ensuite en texte on
ne retrouvera pas nécessairement le texte initial.


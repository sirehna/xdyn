% Documentation développeur du simulateur X-Dyn
% Charles-Edouard CADY, SIREHNA (DCNS Research)


Le but de ce document est de donner une vue globale de l'architecture
logicielle du simulateur X-DYN, en insistant sur les justifications
techniques du design.

Il s'agit moins d'une description systématique et exhaustive de tous les
fichiers du code que d'une présentation fonctionnelle structurée des concepts
et principes qui ont régi les développements.

# Cartographie des modules

Le code a été réparti en répertoires que l'on appelera ici
`module`. L'inclusion de code dans un module dépend de deux critères :

- Ce que fait le code en question (chaque module doit être fonctionnellement
  cohérent)
- Les dépendances dudit code (il faut minimiser les dépendances entre les
  modules)

La cohérence fonctionnelle facilite l'appréhension du code et contribue à
réduire les risques de conflit en cas de développements concomittants. La
minimisation des dépendances est nécessaire pour ordonnancer la compilation
(éviter les cycles, savoir quel fichier compiler en premier) et permet de
limiter les impacts des modifications éventuelles apportées à un module.

Voici la description des modules de X-DYN :

| Module                     | Description                                                                     |
|----------------------------|---------------------------------------------------------------------------------|
| `core`                     | Cœur de calcul décrivant le comportement de l'outil au cours de la simulation   |
| `environment_models`       | Modèles de houle                                                                |
| `exceptions`               | Gestion des erreurs                                                             |
| `executables`              | Programmes principaux (sim, gz)                                                 |
| `external_data_structures` | Structures de données image des fichiers YAML. Toutes les structures de         |
|                            | données de ce module ont un nom commençant par Yaml pour signifier qu'elles     |
|                            | sont l'image d'une partie du fichier YAML d'entrée.                             |
| `external_file_formats`    | Lecture des fichiers externes (hdb, stl)                                        |
| `force_models`             | Modèles d'effort                                                                |
| `gz_curves`                | Calcul des GZ et GM                                                             |
| `hdb_interpolators`        | Calcul des efforts de radiation (convolution)                                   |
| `interface_hdf5`           | Ecriture des fichiers HDF5                                                      |
| `mesh`                     | Calculs sur les maillages (intersection navire/surface libre, itération sur les |
|                            | facettes                                                                        |
| `observers_and_api`        | Définition des sorties en cours de simulation (CSV, HDF5, websocket...)         |
| `parser_extensions`        | Lecture des fichiers de commande et des spectres de houle                       |
| `slamming`                 | Calcul des efforts de slamming                                                  |
| `test_data_generator`      | Génération des données de test (notamment utilisées pour générer tutoriels)     |
| `yaml_parser`              | Interprétation des parties génériques du YAML (non spécifiques à un modèle)     |
|                            | par exemple la définition des corps et des sorties.                             |

Chaque module contient nécessairement :

- Un fichier `CMakeLists.txt` qui indique comment le module doit être compilé,
  quelles sont ses dépendances, etc.
- Un répertoire `inc` qui contient les fichiers d'en-tête (interface, API)

Les sous-répertoires suivants sont généralement présent, mais pas
systématiquement :
- `src` contient les fichiers source à compilé. Le module `exceptions` n'en
  contient pas, par exemple.
- `unit_tests` (qui contient à son tour un fichier `CMakeLists.txt`, un
  répertoire `inc` et un répertoire `src`) stocke les tests unitaires (une
  classe de test par classe, en principe). Le module `test_data_generator` ne
  contient pas de tests unitaires.

# Déroulement d'une simulation

Le schéma ci-dessous représente la liste des tâches que le simulateur va
réaliser entre le moment où l'utilisateur entre la ligne de commande
et la fin des calculs :

* **Récupération des arguments** : le programme lit les arguments fournis par
  l'utilisateur sur la ligne de commande.
* **Ouverture des fichiers** : le contenu des fichiers de commande et du (ou des)
  fichiers YAML est lu (sans être interprêté, à ce stade).
* **Création du système à simuler** : le contenu des fichiers est interprêté et
  l'on crée les structures de données internes utilisées pour la simulation.
* **Création des observateurs** : les observateurs permettent de réaliser des
  actions en cours de simulation (sauvegarde des états, tracés...)
* **Simulation** : la simulation proprement dite utilise le solveur et les
  structures de données construites précédemment

## Récupération des arguments

Le module concerné est `executables`. Les fichiers conernés sont :

- `simulator.cpp` : le fichier principal contenant `main`
- `utilities_for_InputData.cpp` : lit la ligne de commande en utilisant
  boost::program_options
- `simulator_run.cpp` : fonction de haut niveau qui appelle la lecture de la
  ligne de commande, l'ouverture des fichiers, la création du système et des
  observateurs et la simulation.
  commande

## Création du système à simuler

La création du système à simuler est faite en deux étapes par la fonction
`get_system` dans le fichier `simulator_api.cpp` du module `observers_and_api`
:

- `get_system` appelle le parseur YAML principal (classe `SimulatorYamlParser`)
- création du système (classe `SimulatorBuilder`) par truchement de la fonction
  `get_builder`.


### Parseur YAML principal
La classe `SimulatorYamlParser` a pour responsabilité de construire une
structure arborescente de type `YamlSimulatorInput` qui est l'image de
l'ensemble du fichier YAML d'entrée.

Le parseur principal ne se préoccupe pas du YAML spécifique des modèles
d'effort et des modèles de houle : celui-ci est conservé en tant que chaîne de
caractère et parsé ultérieurement par chaque module. L'idée sous-jacente est
que les modèles d'effort sont une partie du code amenée à évoluer
(potentiellement de manière indépendante pour répondre aux besoins internes
spécifiques de chacun des partenaires) tandis que la description des corps
(inerties, position du centre de masse, conditions initiales...) doivent peut
évoluer.

En conséquence, le YAML est parsé en deux temps :

1. parties communes
2. modèles d'effort et d'environnement

Cette architecture permet de regrouper l'ensemble des traitements afférents à
un modèle particulier dans une seule et même classe.  Ainsi, lorsque l'on
intervient par exemple sur le modèle de Wageningen, on utilise la classe
`WageningenControlledForceModel` du module `force_models` qui contient, outre
le modèle d'effort spécifique des hélices Wageningen, une méthode statique
`parse`. Cela évite à devoir intervenir sur deux modules distincts lorsque l'on
souhaite faire évoluer le modèle.

### Construction du système

La classe `SimulatorBuilder` s'occupe de la construction du système à simuler.
Cette classe ne connaît aucun des modèles d'effort et d'environnement et ne
sait pas les construire à partir du YAML : cette responsabilité échoit aux
parseurs spécifiques à chaque modèle.
Pour que la classe `SimulatorBuilder` ait connaissance de ces parseurs
spécifiques, elle doit être configurée et c'est le rôle de la fonction
`get_builder`, définie dans le fichier `simulator_api.cpp` du module
`observers_and_api`. On y trouve des instructions du type :


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
SimulatorBuilder builder(yaml, t0, command_listener);
builder.can_parse<DefaultSurfaceElevation>()
       .can_parse<BretschneiderSpectrum>()
       .can_parse<JonswapSpectrum>();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`can_parse` est un template de méthode, défini dans le fichier
`SimulatorBuilder.cpp` du module `core`. Sa responsabilité est d'ajouter un
parseur à la liste des parseurs de `SimulatorBuilder`.

Une fois que le `SimulatorBuilder` contient les parseurs, on appelle la méthode
`SimulatorBuilder::build`. Cette méthode construit successivement
l'environnement (notamment le modèle de houle), les efforts non-contrôlés, les
efforts contrôlés et les corps à simuler.

#### Construction de l'environnement

La méthode `SimulatorBuilder::get_environment` construit l'environnement. Elle
commence par stocker des constantes lues directement depuis le YAML, puis elle
construit le modèle de houle et l'objet `Kinematics` qui va servir à faire les
changements de repère.

La méthode de construction du modèle de houle est la suivante
(`SimulatorBuilder::get_wave`) :

- On a récupéré du fichier YAML une liste de modèle (c'est-à-dire un nom et une
  chaîne de caractères contenant le YAML de configuration de ce modèle)
- Pour chaque modèle, on appelle la méthode `try_to_parse` de chaque parseur de
  vague (inséré par `can_parse` décrit précédemment). Cette méthode retourne un
  `boost::optional<SurfaceElevationPtr>` qui contient soit un modèle de houle
  si le parseur a réussi à interpréter la chaîne de caractères, soit rien dans
  le cas contraire.
- Si la boucle se termine sans qu'aucun parseur n'ait réussi à interprêter la
  chaîne YAML, une erreur est renvoyée.

Ce schéma de construction est utilisé pour tous les autres modèles (efforts
non-commandés et commandés).

#### Construction des efforts

Les efforts contrôlés et non-contrôlés sont construits en utilisant le même
mécanisme donc nous ne parlerons ici que du cas des efforts non-contrôlés,
construits par la méthode `SimulatorBuilder::get_forces` :

- Pour chaque corps défini dans le fichier YAML, on appelle la méthode
  `SimulatorBuilder::forces_from` en lui fournissant le modèle d'environnement
  construit précédemment (parce que le constructeur de chaque modèle d'effort a
  besoin de l'environnement puisqu'il est possédé par chaque modèle)
- La méthode `SimulatorBuilder::forces_from` boucle sur tous les modèles
  d'effort (c'est-à-dire, à ce stade, un nom et une chaîne de caractère
  contenant le YAML de configuration) et appelle la méthode
  `SimulatorBuilder::add`
- Cette dernière boucle sur tous les parseurs d'effort non-commandés, à la
  manière de `SimulatorBuilder::get_wave`, en appelant `try_to_parse` sur
  chacun. Le résultat est un `boost::optional<ForcePtr>` qui contient soit un
  modèle d'effort, soit rien, suivant que le parseur a réussi à interprêter la
  chaîne de caractères.

#### Construction des corps à simuler

Les corps à simuler sont construits dans la méthode `SimulatorBuilder::build`,
simplement en invoquant le constructeur de la classe `Sim`, en lui fournissant
les objets construits précédemment (efforts commandés et non-commandés,
environnement et commandes).

## Création des observateurs

Le rôle des observateurs est, étant donné l'état du système, d'effectuer des
actions telles que la sérialisation en différents formats. Le design de ces
observateurs a été assujetti aux contraintes suivantes :

- Pouvoir utiliser plusieurs sérialisations en parallèle, éventuellement en
  sérialisant des choses différentes dans chacune et les traiter de façon
  homogène
- N'avoir à intervenir qu'à un seul endroit du code pour rendre une variable
  interne "sérialisable" (par exemple, lorsque l'on a rendu "GM" disponible à
  partir du modèle `GMForceModel`)
- N'avoir à intervenir qu'à un seul endroit lorsque l'on rajoute un type de
  sérialisation

L'API des observateurs est décrite dans une classe abstraite (`Observer`) définie dans
le module `core`. Toutes les méthodes virtuelles pures de cette classe sont
protégées (`protected`) afin de garantir le fonctionnement de l'API : les
classes dérivées ne peuvent pas changer l'ordre dans lequel sont appelées ces
méthodes.

Deux éléments de design sont essentiels dans la classe `Observer` :

1. L'initialisation est séparée de la sérialisation. L'initialisation a lieu
   avant la première sérialisation et sert, par exemple, à écrire la première
   ligne (les titres de colonne) d'un fichier CSV ou à initialiser la structure
   d'un fichier HDF5. La sérialisation proprement dite a lieu à chaque pas de
   temps.
2. Les fonctions virtuelles de la classe `Observer` n'effectuent pas
   directement ni la sérialisation ni l'initialisation : elles renvoient des
   fonctions qui effectueront ces tâches pour une valeur donnée. Ainsi, lorsque
   l'on appelle la méthode `Observer::write`, on ne fait qu'ajouter une
   fonction d'initialisation et une fonction de sérialisation à un dictionnaire
   : aucune valeur n'est écrite immédiatement.

En pratique, le fonctionnement est le suivant :

- L'utilisateur défini les sérialisations qu'il souhaite voir réaliser dans la
  section `output` du fichier YAML
- Juste après la création du système à simuler, dans la fonction
  `run_simulation` du fichier `simulator_run.cpp` du module `executables`, on
  crée les observateurs au moyen de la fonction `get_observers` du même module.
- Cette fonction parse la section `output` du fichier YAML et retourne une
  liste de structures de données externes `YamlOutput`.
- Une fois le parsing achevé, on ajoute à cette liste les éventuels
  observateurs demandés sur la ligne de commande (qui sont spécifiés avec le flag `-o`)
- La dernière étape de la fonction `get_observers` est la construction de la
  liste des observateurs au moyen du constructeur de la classe
  `ListOfObservers`. Ce constructeur appelle les constructeurs de chaque type
  d'observateur avec en paramètre la liste des variables à sérialiser.

La classe `ListOfObservers` est un observateur (au sens de
`ssc::solver::quicksolve`) mais ne dérive pas de `Observer`. Cela signifie
qu'elle doit simplement disposer d'une méthode `observer`, prenant un système à
simuler et le temps courant. Cette méthode se contente d'appeler la méthode
`observe` de chaque observateur contenu dans `ListOfObservers`.

La méthode `Observer::observe` commence par rendre le temps disponible en
appelant la méthode `write`. Ensuite, l'observateur demande au système de lui
donner toutes les valeurs sérialisables grâce à la méthode `Sim::output`. Cette
dernière appelle la méthode `feed` de chaque modèle d'effort et de chaque
corps. Le rôle des méthodes `feed` consiste à fournir à l'observateur toutes
les valeurs qui peuvent être sérialisées. Dans le cas des modèles d'effort, ce
sont bien sûr Fx, Fy, Fz, K, M, N, mais éventuellement aussi d'autres
observations spécifiques renseignées dans `extra_observations`. La méthode
`Observer::write` prend une adresse (utilisée surtout pour la sérialisation en
HDF5) et une valeur.

Il est important de noter qu'à ce stade aucune valeur n'a été effectivement
écrite par l'observateur : il contient simplement des valeurs que l'on peut
sérialiser. La sérialisation effective est commandée par les deux dernières
lignes d'`Observer::observer` : `initialize_everything_if_necessary` et
`serialize_everything`. La première n'est appelée qu'une fois avant le premier
pas de temps (par exemple, pour écrire la ligne de titre d'un fichier CSV) et
la seconde est appelée systématiquement. Elle boucle sur la liste des choses à
sérialiser (qui ont été définies au moment de la construction de l'observateur)
et cherche pour chacune une fonction réalisant cette sérialisation dans le
dictionnaire de fonction rempli par les appels à la fonction `write`de chaque
observateur effectué par chacun des modèles. Si elle trouve une telle fonction,
elle l'appelle (et la sérialisation s'effectue), sinon elle lance une exception
(et la simulation s'arrête).

Les observateurs implémentés sont les suivants :

- CSV (Comma-Separated Values)
- HDF5
- TSV (tab-separated values)
- std::map (utilisé uniquement pour les tests unitaires internes)
- JSON
- Websocket (pour l'interface HTML)

## Simulation

## Appel du solveur

La simulation proprement dite utilise `ssc::solver`, une fine couche
d'interface autour de `boost::odeint`. La commande exécutant la simulation
s'appelle `quicksolve` et figure dans le fichier `simulator_run.cpp` du module
`executables` :

~~~~~~~~~~~~~~ {.cpp}
ssc::solver::quicksolve<Stepper>(system, tstart, tend, dt, observer);
~~~~~~~~~~~~~~

Voici une brève description des paramètres (le fonctionnement détaillé du
solveur figure dans la documentation utilisateur).

+-----------+---------------------------------------------------------------------------------------+
| Paramètre | Description                                                                           |
+===========+=======================================================================================+
| `Stepper` | Type du solveur à utiliser. Il s'agit de `typedef`s vers les types de solveur de      |
|           | `odeint`. Par exemple, `ssc::solver::EulerStepper` est un alias pour                  |
|           | `::boost::numeric::odeint::euler<std::vector<double> >`.                              |
+-----------+---------------------------------------------------------------------------------------+
| `system`  | N'importe quel objet possédant une méthode                                            |
|           | `void operator()(const std::vector<double>& x, std::vector<double>& dxdt, double t)`  |
|           | Dans le cas du simulateur "Bassin Numérique", un objet de la classe `Sim`             |
+-----------+---------------------------------------------------------------------------------------+
| `tstart`  | `double` représentant la date du premier pas de temps                                 |
+-----------+---------------------------------------------------------------------------------------+
| `tend`    | `double` représentant la date du dernier pas de temps calculé                         |
+-----------+---------------------------------------------------------------------------------------+
| `dt`      | `double` représentant l'incrément en temps. Dans le cas d'un solveur à pas adaptatif, |
|           | longueur initiale du pas de temps.                                                    |
+-----------+---------------------------------------------------------------------------------------+

La responsabilité du solveur est double :

- Appeler l'observateur à la fin de chaque pas de temps en lui fournissant le
  système et l'instant courant
- Calculer la valeur des états du système à chaque pas de temps en passant d'un
  instant au suivant en utilisant le `Stepper`

Le fonctionnement des observateurs a déjà été décrit ci-dessus. Pour calculer
les états du système, le `Stepper` a uniquement besoin des dérivées des états.
C'est la responsabilité de la classe `Sim` de les lui fournir.

## Fonctionnement de la classe `Sim`

La classe `Sim` a deux responsabilités : d'une part, elle calcule la dérivée des
états du système simulé pour le stepper, et d'autre part elle offre des vues sur
ce système aux observateurs (états, vagues, forces).

### Calcul de la dérivée des états du système

Ce calcul est piloté par la méthode `Sim::operator()` qui est appelée par le
Stepper. Cette méthode réalise quatre actions :

1. Mise à jour des corps (dans un sens décrit ci-après)
2. Calcul de la somme des efforts appliqués à chaque corps
3. Calcul des dérivées des états
4. Normalisation des quaternions

#### Mise à jour des corps

`Sim::operator()` appelle la méthode `update` de chaque corps. Cette méthode :

- Met à jour les transformations de NED vers BODY
- Enregistre la nouvelle valeur des états fournie par `Sim`
- Met à jour l'intersection du maillage du solide avec la surface libre (le cas
  échéant)
- Calcule la projection de l'axe z du repère BODY dans le repère maillage (pour
  les calculs de houle)

La classe `Body` est une classe abstraite
(`update_intersection_with_free_surface` est virtuelle pure) dont dérivent deux
sous-classes : `BodyWithSurfaceForces` et `BodyWithoutSurfacesForces`. La mise à
jour de l'intersection du maillage et de la surface libre n'est faite que dans
la classe `BodyWithSurfaceForces` et est déléguée à l'objet `MeshIntersector`.

Le rôle de la classe `MeshIntersector`, définie dans le module `mesh`, est de
pouvoir itérer sur les facettes immergées ou sur les facettes émergées du
maillage. Cette fonctionalité est utilisée par les efforts de surface
(`ExactHydrostaticForceModel`, `FastHydrostaticForceModel`, `GMForceModel` et
`HydrostaticForceModel`) pour intégrer les efforts sur la coque (réalisé par la
méthode `SurfaceForceModel::operator()` définie dans le module `core`.

`MeshIntersector` est une classe dont les performances sont critiques pour la
rapidité globale de la simulation. Pour atteindre des temps de calculs acceptables,
un tribut a été payé à l'optimisation par un code d'assez bas niveau. Il s'agit
donc du code le plus difficile à appréhender du simulateur, en particulier la
méthode `MeshIntersector::split_partially_immersed_facet_and_classify`.

#### Calcul de la somme des efforts appliqués à chaque corps

Le bilan des forces est réalisé par la classe `Sim` (méthode
`Sim::sum_of_forces`) et comprend les étapes suivantes :

- Calcul des efforts centripète et de Coriolis (réalisé dans le SSC,
  c'est-à-dire la bibliothèque de connaissances antérieures de SIREHNA)
- Mise à jour des modèles d'effort avec les nouveaux états (appel de la méthode
  `update` de chaque modèle d'effort qui effectue le calcul du modèle d'effort)
- Le cas échéant, changement de repère pour exprimer tous les efforts dans le
  repère de résolution du principe fondamental de la dynamique
- Incrémentation de la somme des efforts
- Calcul de la projection de la somme des efforts dans le repère NED

Ce calcul n'a pas été intégré à la classe `Body` pour limiter sa responsabilité
à la gestion des états et du maillage du corps. _Stricto sensu_, il sort même du
périmètre de la classe `Sim` et il serait justifié de créer une nouvelle classe
pour le réaliser.


#### Calcul de la dérivée des états

Ce calcul est dévolu à la classe `Body` (méthode
`Body::calculate_state_derivatives`). La seule particularité de ce calcul est la
dernière étape : le forçage des états. Comme l'algorithme d'intégration temporel
utilisé dans le simulateur ne fait pas de résolution sous contrainte, on ne peut
pas forcer tous les états et l'on ne fait que forcer les dérivées. Ce forçage
est de la responsabilité de la classe `BlockedDOF` définie dans le module
`core`.

Le forçage est réalisé en deux temps :

1. Forçage des états (réalisé par `BlockedDOF::force_states` appelée par la
   méthode `Body::update_body_states`)
2. Forçage des dérivées (réalisé par `BlockedDOF::force_states` appelée par la
   méthode `Body::calculate_state_derivatives`)

### Préparation des observations

Hormis le calcul de la dérivée des états, la classe `Sim` doit aussi préparer
les données pour les observateurs. Cette tâche est réalisée par la méthode
`Sim::output`. Cette méthode a pour but de fournir à chaque observateur les
valeurs de toutes les variables exportées par le système. Elle fait ainsi passer
l'observateur considéré par tous les modèles d'effort (commandés et
non-commandé) et tous les corps simulés (pour récupérer les états). En outre,
elle se charge de calculer la différence entre la somme des efforts calculés et
les efforts qu'il faut réellement appliquer pour maintenir les forçages.

# Architecture de l'interface graphique

L'interface graphique est réalisée en HTML5 (Javascript + HTML + CSS). Voici les
raisons de ce choix :

- Utiliser un langage différent de celui du simulateur force à découpler
  clairement l'interface graphique du cœur de calcul
- Le simulateur n'a pas besoin d'être installé sur la machine d'un utilisateur
  pour pouvoir y être utilisé : il n'a besoin que d'un navigateur internet
- Le développement d'interfaces graphiques en HTML5 ne nécessite que peu
  d'outils, pas même un compilateur puisque celui-ci est déjà dans le
  navigateur, ce qui rend le développement très rapide
- Si l'on souhaite sous-traiter l'interface graphique, il existe plus de
  développeurs Javascript que dans tout autre langage de programmation

Afin de faire communiquer le cœur de calcul et l'interface graphique
(Javascript), on utilise des Wesockets. Ce sont des sockets TCP classiques mais
ils sont compris par le navigateur. L'intérêt principal de cette technologie est
qu'elle permet d'envoyer des données de façon asynchrone du serveur au client et
non pas simplement de répondre à des requêtes du client.

Le fonctionnement de l'interface graphique s'articule autour de trois éléments :

- Le simulateur, par le truchement du `WebSocketObserver`
- Le code HTML5
- Un serveur Python (`tornado`) qui s'occupe d'appeler le simulateur avec les
  paramètres fournis à l'interface graphique et de créer le websocket

## L'observateur websocket

La classe `WebSocketObserver` est définie dans le module `observers_and_api`.
Elle se connecte à un websocket existant (elle ne crée pas de websocket) et
envoie les données sous format YAML. Voici un exemple de trame émise :

~~~~~~~~~~~~~ {.yaml}
{
'x(Anthineas)': 23.4,
'y(Anthineas)': 121.4,
'z(Anthineas)': 0.4,
'u(Anthineas)': 0.4,
'v(Anthineas)': 0.7,
'w(Anthineas)': 0.1,
'p(Anthineas)': 0.07,
'q(Anthineas)': 0.4,
'r(Anthineas)': 0.121,
'qr(Anthineas)': 1,
'qi(Anthineas)': 0,
'qj(Anthineas)': 0,
'qk(Anthineas)': 1
}
~~~~~~~~~~~~~

Pour des raisons de place, les données de houle sont transmises sous forme
binaire au format 'base 91' qui permet de n'utiliser que des caractères ascii
valides.

## Structure du code HTML5

Le code HTML5 comprend :

- Un fichier HTML (`websocket_test.html`) qui contient les conteneurs pour les
  différents éléments de la page
- Des fichiers CSS (le framework Bootstrap de Twitter est utilisé pour le rendu
  des widgets)
- Des fichiers Javascript. Le fichier `websocket.js` contient tous les aspects de communication avec le
  simulateur et le fichier `realtime_plot.js` les fonctions de tracé 

## Fonctionnement du serveur Python

Le serveur a été écrit en Python en utilisant la bibliothèque `tornado` car
c'était le langage qui minimisait la quantité de code à écrire et les opérations
de mise en place.

# Compilation du simulateur

La machine virtuelle Linux livrée est déjà préconfigurée pour la compilation du
simulateur. Voici cependant les instructions pour compiler depuis une machine
vierge.

La première étape est de décompresser le fichier `tgz` contenant le code source.
On obtient ainsi un dossier que nous nommerons `XDYN_ROOT` par la suite.

## Récupération des dépendances



## En ligne de commande

- A partir du dossier `XDYN_ROOT/code`, créer un répertoire `build`
- Se placer dans `XDYN_ROOT/code/build`
- Sous Windows (MinGW), exécuter `cmake -G "MSYS Makefiles" ..`
- Sous Linux, `cmake ..` On peut aussi choisir d'utiliser Ninja (plus rapide que
  Make) en spécifiant `-G Ninja`
- Lancer `make`

On peut ensuite lancer les tests unitaires :

`./run_all_tests`

La documentation est construite par :

`make doc`

On peut créer un programme d'installation en faisant :

`make package`


## A partir d'Eclipse

1. Créer un nouveau projet
    ![Right-click - New Project](create_eclipse_project.png)
2. Selectionner 'C++ Project'
    ![Select C++ Project](create_eclipse_project_2.png)
3. Ne pas utiliser l'emplacement par défaut
    ![Don't use default location](create_eclipse_project_3_1.png)
4. Choisir "Empty Project"
    ![Choose Empty project](create_eclipse_project_3_2.png)
5. Sous Windows, choisir 'MinGW GCC toolchain'
    ![MinGW GCC Toolchain](create_eclipse_project_3_3.png)
6. Choisir le répertoire de build
    ![Change project properties](create_eclipse_project_4.png)
    ![Check we're using external builder](create_eclipse_project_4_1.png)
    ![Change build location to ${workspace}/build](create_eclipse_project_4_2.png)
7. Créer les cibles
    ![New target](create_eclipse_project_5.png)
    ![Target parameters](create_eclipse_project_6.png)

# Perspectives de développement

Pour une poursuite du travail sur ce simulateur, les fonctionalités suivantes
pourraient être considérées :

## Modèles supplémentaires

- Hollenbach résistance à l'avancement
- Modèle de houle régulière non-linéaire
- Vent (spectre de Harris)
- Variation des conditions environnementales suivant un scénario
- Faciliter la maintenance des modèles de houle en calquant le fonctionnement
  de leurs parseurs sur celui des modèles d'effort

## Aspects multi-corps

- Liaisons cinématiques (permet de simuler le déplacement d'objets amarés au
  navire)
- Détection des collisions
- Intégration d'un système algébro-différentiel (permet de forcer proprement les
  états)

## Amélioration des performances

- Des gains de performance peuvent sans doute être obtenus sur les calculs
  d'intersection de maillage. Par exemple, on pourrait utiliser la librarie LGPL
  "GNU Triangulated Surface Library" ou CGAL. L'idée serait alors de créer un
  maillage de la surface libre et de calculer son intersection avec le maillage
  de la coque, plutôt que de calculer les hauteurs de houle en chaque point du
  maillage.
- L'évaluation des modèles de houle peut être accélérée en la parallélisant (par
  exemple, sur un GPU)
- La vitesse d'exécution des modèles de manoeuvrabilité peut être améliorée :
  actuellement, ils sont interprêtés, mais il est possible de les compiler
  dynamiquement pour atteindre les performances d'un code natif.

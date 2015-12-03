% Documentation développeur du simulateur X-Dyn
% Charles-Edouard CADY, SIREHNA (DCNS Research)


Le but de ce document est de donner une vue globale de l'architecture
logicielle du simulateur Bassin Numérique, en insistant sur les justifications
techniques du design.

# Cartographie des modules

Les modules sont matérialisés par des répertoires.

| Module                     | Description                                                                     |
|----------------------------|---------------------------------------------------------------------------------|
| `core`                     | Coeur de calcul décrivant le comportement de l'outil au cours de la simulation  | 
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

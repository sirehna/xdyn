L'objectif de la présente note est de donner les étapes nécessaires à la
création d'un dépôt de code open source.

Ceci implique d'utiliser un dépôt de code public et
de préparer le dépôt pour les contributions extérieures.

# Pré-requis

On suppose que le code que l'on souhaite est géré en configuration en utilisant
[Git](https://git-scm.com/) car c'est l'outil de versionnement le plus utilisé
dans l'industrie logicielle, qu'il est souple (utilisation hors connexion,
gestion des branches...) et s'intègre avec de nombreux outils.


# Dépôt de code

## Choix d'un dépôt public

Pour rendre le code accessible, il faut le stocker sur un dépôt public. Le choix
s'est porté sur [GitHub](https://github.com/), du fait de sa très grande
popularité et de l'intégration
qu'il propose avec de nombreux outils (analyse statique de code, couverture de
tests, intégration et déploiement continus, etc.).

## Création d'un compte GitHub SIREHNA

En utilisant [ce
guide](https://help.github.com/articles/creating-a-new-organization-from-scratch/),
on crée un compte SIREHNA afin que les développeurs appartenant à la société
soient identifiés comme tels. Ce compte sera administré par l'administrateur
système de SIREHNA.

## Création ou association des comptes utilisateurs

Les contributeurs internes à SIREHNA peuvent soit [créer un compte sur GitHub]()
et demander son association au compte SIREHNA, soit (le cas échéant) demander
l'association de leur compte existant. Nous préconisons plutôt de
systématiquement créer un nouveau compte afin de bien distinguer les
contributions faites par le collaborateur pendant son temps de travail des
éventuelles contributions faites sur son temps libre.

# Préparation du dépôt

Avant de rendre le dépôt de code public, plusieurs étapes
sont nécessaires. Il faut notamment créer cinq fichiers à
la racine du dépôt :

- Fichier de licence : `LICENSE`
- Règles pour les rétro-contributions : `CONTRIBUTING.md`
- Page d'accueil du projet : `README.md`
- Description sommaire des derniers changements : `CHANGELOG`
- Un code de conduite pour la communauté : `CODE_OF_CONDUCT.md`

## Fichier de licence

La licence choisie est [Apache
2.0](https://choosealicense.com/licenses/apache-2.0/) parce qu'elle est à la
fois très souple (ce qui facilite les contributions) et préserve les brevets et
marques éventuels.

Il faut reproduire le code de la licence dans un fichier `LICENSE` que l'on met à
la racine du dépôt.

## Règles pour les rétro-contributions

Afin de clarifier les règles pour les contributions externes, il convient de
créer un fichier `CONTRIBUTING.md` à la racine du projet. Le contenu et le
format de ce fichier sont assez libres, mais de bons exemples existent :

- [Règles de contribution du projet Atom](https://github.com/atom/atom/blob/master/CONTRIBUTING.md)
- [Règles de contribution du projet Ruby on Rails](https://github.com/rails/rails/blob/master/CONTRIBUTING.md)
- [Règles de contribution du projet opengovernment](https://github.com/opengovernment/opengovernment/blob/master/CONTRIBUTING.md)

## Page d'accueil du projet

La page d'accueil (ou `README`) donne une description sommaire du projet, à quoi
il sert, et comment l'utiliser. Un guide d'écriture d'une telle page est
disponible [ici](https://open-source-guide.18f.gov/making-readmes-readable/).

## Description sommaire des derniers changements

Ce fichier vient en complément de l'outil de gestion de configuration et donne
une vue synthétique des derniers changements, contrairement à l'outil de gestion
de configuration qui en donne le détail. Des règles pour créer un tel fichier
sont disponibles [ici](http://keepachangelog.com/fr/1.0.0/).

## Code de conduite

Afin de normaliser les règles d'échanges dans la communauté libre que l'on crée,
il est préférable d'utiliser un code de conduite. Ce code de conduite définit
les comportements acceptables et non-acceptables et les règles de bienséances.
Un exemple d'un tel code de conduite est le
[contributor covenant](https://www.contributor-covenant.org/).

## Upload sur GitHub

C'est l'étape la plus simple. Elle consiste simplement à lancer :

~~~~~~~~~~~ {.bash}
git remote add github <URL>
~~~~~~~~~~~

où `<URL>` désigne l'adresse du dépôt.

Pour mettre à jour le dépôt GitHub avec les modifications faites à SIREHNA il
suffit dès lors de faire :

~~~~~~~~~~~ {.bash}
git push github
~~~~~~~~~~~

De même, pour récupérer les modifications distantes :

~~~~~~~~~~~ {.bash}
git fetch github
~~~~~~~~~~~

# Configuration du projet sur GitHub

Pour tirer pleinement parti de la puissance des outils disponibles via GitHub,
on peut ajouter des badges au `README` du projet. Ces badges apparaissent sur
la page principale du projet et informent sur la santé du projet en un clin
d'oeil. On peut voir une liste (non-exhaustive) de badges
[ici](https://github.com/boennemann/badges).

Nous proposons de commencer par les badges suivants :

| Type de badge  | Fournisseur | Exemple de badge    |
|----------------|-------------|-------------|
| Couverture de code | [codecov](https://codecov.io) | [![codecov.io](https://camo.githubusercontent.com/e4fdf971b0e9618ca15a013befc12aaefd770cfd/68747470733a2f2f636f6465636f762e696f2f67682f5265666572656e63652d4c415041434b2f6c617061636b2f6272616e63682f6d61737465722f67726170682f62616467652e737667)](https://codecov.io/github/boennemann/badges?branch=master) |
| Intégration continue (Linux) | [travis-ci](https://travis-ci.org) | [![Build Status](https://travis-ci.org/boennemann/badges.svg?branch=master)](https://travis-ci.org/boennemann/badges) |
| Intégration continue (Windows) | [appveyor](https://ci.appveyor.com) | [![Build Status](https://travis-ci.org/boennemann/badges.svg?branch=master)](https://travis-ci.org/boennemann/badges) |
| Analyse des failles de sécurité, des violation de licence et des dépendances obsolètes | [VersionEye](https://www.versioneye.com/) | [![VersionEye](https://img.shields.io/versioneye/d/ruby/rails.svg?style=plastic)]()
| Couverture de tests et analyse statique de code | [Code Climate](https://codeclimate.com/product) | [![Code Climate](https://img.shields.io/codeclimate/issues/github/me-and/mdf.svg?style=plastic)]()
| Lien avec Docker hub | [Docker Hub](https://hub.docker.com/) | [![Docker Build Status](https://img.shields.io/docker/build/jrottenberg/ffmpeg.svg?style=plastic)]()

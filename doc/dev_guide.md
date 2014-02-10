# Developer guide

This page is guide for the development of the lightweight temporal ship simulator.

This simulator is composed of a *kernel* that contains the core of the simulator and
a set of modules that represent the physical models implemented in the simulator.

The kernel relies on previous developments and is thus the property of DCNS Research.
The modules are developped withtin the context of the project and are the property
of IRT JV.

This simulator is written in C++ and relies on:

* a decentralized version control system (DCVS)
* a continuous integration server that ensures the integrity of the development.

The following explains the philosophy of the development.

## Guidelines

Source files are versionned using [GIT](http://www.git-scm.com),
free and open source distributed version control system.

Among the many pros of GIT, it will allow easy branching for
development of new features.

The tasks of continuous integration are to:

* retrieve all the source for a compilation
* compile the source
* create the documentation of the program
* test the sources with automatic unit testing
* run various metric on the code
* perform code coverage and memory leak detection
* create installation program
* deploy a revision to the client

[Jenkins](http://www.jenkins-ci.org) is used to provide
continuous integration service.

It is used on a Windows OS and a Linux-like OS.

## File repositories
Because of the nature of the project, two GIT file
repositories hosted on [Sir6](http://130.66.124.6/git/) will be used :

* One that will contain all developments performed during the project.
  It contains all the developments made for the integration of
  physical models, that are included in the simulator. It will also
  contain all the dependencies to create the kernel of the simulator
  with for example the ``datasource`` that has been developed
  previously with other projects.
* Another one, that will match the GIT Lab of the IRT JV repository.
  It contains all physical models developed during this project
  with the binary version the kernel library.

As a consequence, two Jenkins jobs will be used to compile, test, the source
code stored on these two file repositories.

## Jenkins jobs

This section presents the tasks performed by the two Jenkins jobs
associated to the two files repositories

### Sirehna/DCNS Research file repository

| Jobs                  | Description                                                                                             |
| :---------------------| :------------------------------------------------------------------------------------------------------ |
| ``build``             | Compile all the source files required to generate the libraries and program associated to the simulator |
| ``document``          | Generate the program documentation with [Doxygen](http://www.doxygen.org)                               |
| ``get_test_data``     | Generate all the test data required to execute unit tests                                               |
| ``test``              | Perform the unit testing of the code and produce a XML JUnit report                                     |
| ``valgrind``          | Perform a memory check on the test program for Unix platform                                            |
| ``report``            | Parse all report files and generate synthetic diagram to follow the evolution of the project            |

### IRT JV file repository

| Jobs                  | Description                                                                                                         |
| :---------------------| :------------------------------------------------------------------------------------------------------------------ |
| ``build``             | Compile all the source physical models required to generate simulator and link against the compiled kernel library  |
| ``manual``            | Retrieve all the information needed for the user manual and publish it                                              |
| ``get_test_data``     | Generate all the test data required to execute unit tests                                                           |
| ``get_test_data``     | Generate all the test data required to execute unit tests                                                           |
| ``test``              | Perform the integration tests                                                                                       |
| ``package``           | Create an installation program that will be used bu final user                                                      |

## Developpement environment

This section presents the tools and libraries used to perform the development of the simulator

### Generic tools used
| Tools                                 | Description                                                   |
| :--------------------------------     | :------------------------------------------------------------ |
| [GIT](http://www.git-scm.com)         | Cross-platform open-source build system                       |
| [CMake](http://www.cmake.org)         | Cross-platform open-source build system                       |
| [Eclipse](http://www.eclipse.org)     | IDE with module CDT                                           |
| [Jenkins](http://www.jenkins-ci.org)  | Continuous integration tool                                   |
| [Doxygen](http://www.doxygen.org)     | Tool for generating documentation from annotated C++ sources  |
| [Graphviz](http://www.graphviz.org)   | Optional tool for graph generation with documentation         |

### Specific Windows tools
| Tools                                             | Description                               |
| :------------------------------------------------ | :---------------------------------------- |
| [MINGW-MSYS](http://www.mingw.org)                | Tool chain compilation                    |
| [MikTeX](http://www.miktex.org)                   | LaTeX compiler                            |
| [NSIS](http://www.nsis.sourceforge.net/Main_Page) | Installation program generation           |
| [Notepad++](http://www.notepad-plus-plus.org)     | Open source code editor                   |

### Thirdparty libraries
| Library                                           | Description                               |
| :-------------------------                        | :---------------------------------------- |
| [GTest](http://www.code.google.com/p/googletest)  | MIT Open-source unit testing library      |
| [Yaml-CPP](http://www.code.google.com/p/yaml-cpp) | MIT Open-source input/output file library |


## Development policy and coding rules

### Development rules

* Each new feature should be developed in a new branch.
* While developing these new features, a rebase on the trunk should be run every time possible
  to deal merge conflicts as soon as possible.
* ...

### Coding rules

* EOL should be Unix-styles with LF characters.
* File should be UTF-8 encoded.
* Committed files should not contain any trailing spaces.
* ...
* For more informations, check the wiki [XWiki](http://sir6:8080/xwiki/bin/view/Espace+de+travail+commun/Checklist+de+Revue+de+code)

## Compilation instructions

Below are the typical commands to run to compile the simulator:

* ``cd simulator``
* ``mkdir build``
* ``cd build``
* ``cmake ..``
* ``make``
* ``make doc``
* ``make install``
* ``make test``
* ``make package``
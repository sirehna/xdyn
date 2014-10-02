% IRT JV Simulator - Developer Guide
% Guillaume Jacquenot, Charles-Edouard Cady

This page is guide for the development of the lightweight temporal ship simulator.
To generate this document, run:
``pandoc dev_guide.md -s -c ../planning/stylesheet.css --toc -o dev_guide.html``

This simulator is composed of a *kernel* that contains the core of the simulator and
a set of modules that represent the physical models implemented in the simulator.

The kernel relies on previous developments and is thus the property of DCNS Research.
The modules are developed within the context of the project and are the property
of IRT Jules Verne.

This simulator is written in C++ and relies on:

* a decentralized version control system (DCVS)
* a continuous integration server that ensures the integrity of the development.

The following explains the philosophy of the development.

# Guidelines

Source files are versionned using [GIT](http://www.git-scm.com) version 1.8.5.2 (windows) 1.7.9.5 (linux),
a free and open-source distributed version control system.

Among the many pros of GIT, it will allow easy branching for
the development of new features.

The tasks of the continuous integration server are:

* Retrieve the source code from the DCVS
* Compile the source code
* Create the documentation of the program
* Test the sources with automatic unit testing
* Calculate various metrics on the code
* Perform code coverage and memory leak detection
* Create an installation program
* Deploy a release to the client

The continuous integration server used in this project
is [Jenkins](http://www.jenkins-ci.org).

It is used on a Windows OS (Windows XP) and a POSIX OS (Ubuntu).

# File repositories
Because of the nature of the project, two GIT
repositories hosted on [Sir6](ssh://git_simulateur@130.66.124.6/home/git_projets/simulator.git) are  used :

* One that contains all developments performed during the project.
  It contains all the source code of
  the physical models included in the simulator. It also
  contain all the source code of the kernel of the simulator
  with, e.g., the ``DataSource`` that has been developed
  for another project (EONAV).
* One that matches the repository hosted on the IRT JV's Git Lab server.
  It contains all physical models developed during this project
  with the binary version the kernel library.

As a consequence, two Jenkins jobs are used to compile, test and deploy
the source code stored on these two file repositories.

# Version control
The DVCS used is Git. The repository was initially created on a shared folder
on ``pc-maroff``. It was then cloned on ``pc-gj`` while waiting for a git server
on sir6. However, the installation of a Git server on sir6 proved unnecessary
because the repository could be treated as a normal folder by Samba. The downside
is a possible performance lag (compared to using SSH or the ``git`` protocol).
The reference repository is now located at:

[http://130.66.124.6/public/DOP/EtudesEnCours/ETI140043/simulator.git](http://130.66.124.6/public/DOP/EtudesEnCours/ETI140043/simulator.git)

# Jenkins jobs

This section presents the two Jenkins jobs
corresponding to the two file repositories (internal to SIREHNA & external for
IRT JV).

## Sirehna/DCNS Research file repository

| Jobs                  | Description                                                                                                                    |
| :---------------------| :------------------------------------------------------------------------------------------------------------------------------|
| ``build``             | Compile all the source files required to generate the libraries and program associated to the simulator                        |
| ``document``          | Generate the program documentation with [Doxygen](http://www.doxygen.org)                                                      |
| ``get_test_data``     | Generate all the test data required to execute unit tests                                                                      |
| ``test``              | Perform the unit testing of the code and produce a XML JUnit report                                                            |
| ``valgrind``          | Perform a memory check on the test program for Unix platform                                                                   |
| ``report``            | Parse all report files and generate synthetic diagram                                                                          |
|                       | to follow the evolution of the project                                                                                         |


## IRT JV file repository

| Jobs                  | Description                                                                                                         |
| :---------------------| :------------------------------------------------------------------------------------------------------------------ |
| ``build``             | Compile all the source physical models required to generate simulator and link against the compiled kernel library  |
| ``manual``            | Retrieve all the information needed for the user manual and publish it                                              |
| ``get_test_data``     | Generate all the test data required to execute unit tests                                                           |
| ``test``              | Perform the integration tests                                                                                       |
| ``package``           | Create an installation program that will be used by the final user                                                  |

# Developpement environment

This section presents the tools and libraries used to perform the development of the simulator.

The compiler chosen for this project is [GNU GCC 4.5.2](http://gcc.gnu.org/gcc-4.5).
The rationale behind this choice is that this compiler is widely used at Sirehna.
GCC 4.6 is used on the Linux platform (Ubuntu) because:

- More errors are detected by GCC 4.6
- Code coverage appears to be broken with CMake & GCC 4.5.2.

## Generic tools used
| Tools                                         | Description                                                   |
| :-------------------------------------------  | :------------------------------------------------------------ |
| [GIT](http://www.git-scm.com)                 | Cross-platform open-source build system                       |
| [CMake](http://www.cmake.org)                 | Cross-platform open-source build system                       |
| [Eclipse](http://www.eclipse.org)             | IDE with module CDT                                           |
| [Jenkins](http://www.jenkins-ci.org)          | Continuous integration tool                                   |
| [Pandoc](http://johnmacfarlane.net/pandoc/)   | Tool to used to generate documentation                        |
| [Doxygen](http://www.doxygen.org)             | Tool for generating documentation from annotated C++ sources  |
| [Graphviz](http://www.graphviz.org)           | Optional tool for graph generation with documentation         |

## Specific Windows tools
| Tools                                             | Description                               |
| :------------------------------------------------ | :---------------------------------------- |
| [MINGW-MSYS](http://www.mingw.org)                | Tool chain compilation                    |
| [MikTeX](http://www.miktex.org)                   | LaTeX compiler                            |
| [NSIS](http://www.nsis.sourceforge.net/Main_Page) | Installation program generation           |
| [Notepad++](http://www.notepad-plus-plus.org)     | Open source code editor                   |

## Thirdparty libraries
| Library                                           | Description                                                    |
| :------------------------------------------------ | :------------------------------------------------------------- |
| [Boost](http://www.boost.org/)                    | MIT|BSD Open-source peer-reviewed portable C++ source libraries|
| [GTest](http://www.code.google.com/p/googletest)  | MIT Open-source unit testing library                           |
| [Yaml-CPP](http://www.code.google.com/p/yaml-cpp) | MIT Open-source input/output file library                      |
| [Lapack](http://www.netlib.org/lapack)            | BSD Open-source C/Fortran algebra library                      |
| [Eigen](http://www.eigen.tuxfamily.org/index.php) | MPL2 Open-source C++ algebra library                           |


# Development policy and coding rules

## Development rules

Development will follow the Git branching model described [here](http://nvie.com/posts/a-successful-git-branching-model/):

* Each new feature should be developed in a new branch.
* While developing these new features, a rebase on the trunk should be run regularly
  to fix possible merge conflicts as soon as possible, from the feature branch.

## Coding rules

* EOL should be Unix-styles with LF characters.
* File should be UTF-8 encoded.
* Committed files should not contain any trailing spaces.
* Class names are CamelCase & start with an uppercase letter
* Namespaces & variables are lower case, separated by underscores
* No m_ in front of member variables (need for such a convention is a sign
  classes have too many members)
* Use RAII objects (such as std::tr1::shared_ptr) for memory management
* In general, there should not be more than half a dozen members & half a dozen
  methods per class
* Use forward-declarations (if practical) to split compilation units but don't
  forward-declare the STL for heaven's sake!
* Heavy use of STL & external, proven libraries (such as boost, Lapack, Blas, Eigen...)
  (do not reinvent the wheel)
* For more informations, check the [wiki](http://sir6:8080/xwiki/bin/view/Espace+de+travail+commun/Checklist+de+Revue+de+code).

## How to add a force model

When adding a force model, we must:

- Write the code, of course, and perhaps a wrapper if we want the code to be
  reusable outside this project (or if we're using external code)
- Tell the program how to parse the corresponding YAML

### Coding the force model

The only class the simulator understands in terms of force models is the
ForceModel class. We can either subclass it directly (as in GravityForceModel
for example) or we first create the model independently from the simulator &
then wrap it with a ForceModel (as in HydrostaticForceModel). The choice
depends essentially on whether the code is likely to be used outside the
simulator & the volume of model code with respect to the amount of boiler
plate code to wrap it. Whatever the situation is, a class needs to be created
in the model_wrappers module. The model's parameters are embedded in a Input
structure (as in GravityForceModel::Input). Apart from its parameters, a force
model has access to the environment (wind & waves) the current instant $t$ and
the body's parameters (inertia, mass...) & states (rotation matrix, position
relative to mesh). The force model also has access to a Kinematics object to
change reference frames.

### Coding the parser

First we must decide on the YAML that will configure the force model. For
instance, for a 'foo' model:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}
bodies:
  - name: ship
    external forces:
     - model: foo
       bar: [1,2,3]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Then, we write the corresponding external data structure in the
simulator_external_data_structures module:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
struct YamlFoo
{
    YamlFoo();
    std::vector<int> bar;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once we've done that, we write the parser in simulator_yaml_parser:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
YamlDiracDirection parse_foo(const std::string& yaml)
{
    YamlFoo ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    try
    {
        node["bar"] >> ret.bar;
    }
    catch(std::exception& e)
    {
        std::stringstream ss;
        ss << "Error parsing section foo/bar: " << e.what();
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
    return ret;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The final step is letting the simulator know that there is a parser for this
model. To do that, we add a parser extension which will build the internal data
structure from the external YAML we just defined. In the module
parser_extensions, we add a builder in builder.hpp like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
template <>
class ForceBuilder<FooForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const
std::string& yaml, const EnvironmentAndFrames& env) const;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

which we implement in builders.cpp:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
boost::optional<ForcePtr> ForceBuilder<FooForceModel>::try_to_parse(const std::string& model, const std::string& , const EnvironmentAndFrames& env) const
{
    boost::optional<ForcePtr> ret;
    if (model == "foo")
    {
        const YamlFoo yaml_data = parse_foo(yaml);
        FooForceModel::Input input(yaml_data);
        ret.reset(ForcePtr(new FooForceModel(input)));
    }
    return ret;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Then letting the simulator know about it is just a matter of modifying
get_builder from the simulator_api.cpp file in the simulator module:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.cpp}
SimulatorBuilder get_builder(const YamlSimulatorInput& yaml)
{
    SimulatorBuilder builder(yaml);
    builder.can_parse<GravityForceModel>()
           .can_parse<DefaultSurfaceElevation>()
           .can_parse<HydrostaticForceModel>()
           .can_parse<BretschneiderSpectrum>()
           .can_parse<JonswapSpectrum>()
           .can_parse<PiersonMoskowitzSpectrum>()
           .can_parse<DiracSpectralDensity>()
           .can_parse<DiracDirectionalSpreading>()
           .can_parse<Cos2sDirectionalSpreading>()
           .can_parse<SurfaceElevationFromWaves>()
           .can_parse<Airy>()
           .can_parse<FooForceModel>();
    return builder;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# Instructions to setup the development environment

## Get the source code

* Create an RSA key. For example, under MinGW, use ``ssh-keygen``. Please note
  that changing terminal often implies a different location of the configuration
  files which may lead to SSH not seeing the public RSA key.
* Send the public key (``.pub`` extension) to the system administrator
* The system administrator adds the key to ``/sir7/home/people/git_simulateur/.ssh/authorized_keys``
* Test the setup is correct by running ``ssh git_simulateur@sir7`` from a MinGW
  terminal. If a prompt appears, setup is correct. If a password is requested,
  the RSA key was not found.
* You can then run ``git clone ssh://git_simulateur@130.66.124.6/home/git_projets/simulator.git``
  which will retrieve the source code.

## Instructions for Eclipse
This is how to setup the build environment, in seven easy steps.

1. Create a new project from Eclipse
    ![Right-click - New Project](create_eclipse_project.png)
2. Select C++ Project
    ![Select C++ Project](create_eclipse_project_2.png)
3. Don't use default location
    ![Don't use default location](create_eclipse_project_3_1.png)
4. Choose "Empty Project"
    ![Choose Empty project](create_eclipse_project_3_2.png)
5. Choose MinGW GCC toolchain
    ![MinGW GCC Toolchain](create_eclipse_project_3_3.png)
6. Select build location
    ![Change project properties](create_eclipse_project_4.png)
    ![Check we're using external builder](create_eclipse_project_4_1.png)
    ![Change build location to ${workspace}/build](create_eclipse_project_4_2.png)
7. Create build targets
    ![New target](create_eclipse_project_5.png)
    ![Target parameters](create_eclipse_project_6.png)

## Building

Below are the typical commands to run to compile the simulator:

* ``cd simulator``
* ``mkdir build``
* ``cd build``
* ``cmake -G "MSYS Makefiles" ../code``
* ``make``
* ``make doc``
* ``make install``
* ``make test``
* ``make package``

# Process

The methodology used in this project is a variant of the Extreme Programming
methodology.

## Artefacts

Progress is made visible to the developers, users & product owner by a physical
kanban board & an HTML planning tool, homemade by CEC & which will be visible on
the project's Jenkins page.

### Kanban board

A kanban board. It contains three columns (todo, started & done) & post-its
of various sizes & colors with the following conventions:

- User stories (functional requirements): big blue post-it
- Dev stories (non-functional requirements): big orange post-it
- Interface tasks: small green post-it
- Kernel tasks: small purple post-it
- Physical models tasks: small yellow post-it
- Dev tasks: small pink post-it

### Planning tool

Burndown & burnup charts are automatically generated using the data stored in
two YAML files stored in the ``planning`` subdirectory.

### User stories

As stated on [Wikipedia](https://en.wikipedia.org/wiki/User_story), *a user
story is one or more sentences in the everyday or business language of the end
user or user of a system that captures what a user does or needs to do as part
of his or her job function.* User stories are entered using [Bugzilla](http://sir6/bugzilla/buglist.cgi?bug_status=UNCONFIRMED&bug_status=NEW&bug_status=OPEN&bug_status=ANALYSED&bug_status=ASSIGNED&bug_status=IMPLEMENTED&columnlist=priority%2Cassigned_to%2Cbug_status%2Cshort_desc%2Ccomponent%2Cdeadline&content=&product=Simulateur&query_format=specific&query_based_on=).
They are written in the following form:

**As a** *role*, **I want** *functionality* **so that** *benefit*

* *role*: type of user. This impacts the expected interface & the level of detail
in user interactions.
* *functionality*: actual feature being implemented.
* *benefit*: marketable value of the feature (how the customer will earn or save
money using the feature).

User stories should follow the **INVEST** guidelines:

|Letter  |  Meaning                      | Description
| :------|:------------------------------|:----------------------------------------------------------------------------------------------------------------------|
|I       | Independent                   | The user story should be self-contained, in a way that there is no inherent dependency on another user story.     |
|N       | Negotiable                    | User stories, up until they are part of an iteration, can always be changed and rewritten.                        |
|V       | Valuable                      | A user story must deliver value to the end user.                                                                  |
|E       | Estimable                     | You must always be able to estimate the size of a user story.                                                     |
|S       | Sized appropriately or Small  | User stories should not be so big as to become impossible to plan/task/prioritize with a certain level of         |certainty.
|T       | Testable                      | The user story or its related description must provide the necessary information to make test development possible.|
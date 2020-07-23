# xdyn

[![License](https://img.shields.io/badge/License-EPL%202.0-blue.svg)](https://opensource.org/licenses/EPL-2.0)
[![Build Status](https://travis-ci.org/sirehna/xdyn.svg?branch=master)](https://travis-ci.org/sirehna/xdyn)
[![codecov](https://codecov.io/gh/sirehna/xdyn/branch/master/graph/badge.svg)](https://codecov.io/gh/sirehna/xdyn)
[![](https://images.microbadger.com/badges/image/sirehna/xdyn.svg)](https://microbadger.com/images/sirehna/xdyn "xdyn layers")

xdyn is a lightweight ship simulator modelling the dynamic behaviour of a ship at sea, with its actuators, including some non-linear aspects of that behaviour and featuring a customizable maneuvring model.
It simulates the mechanical behaviour of a solid body in a fluid environment by
solving Newton's second law of motion, taking hydrodynamic forces into account.

It was developed by SIREHNA through both self-funded projects and various collaborative projects, including the IRT Jules Verne's ["Bassin Numérique"
project](https://www.irt-jules-verne.fr/wp-content/uploads/bassin-numerique.pdf).

(c) 2014-2015, [IRT Jules Verne](https://www.irt-jules-verne.fr/), [SIREHNA](http://www.sirehna.com/), [Naval Group](https://www.naval-group.com/en/), [Bureau Veritas](https://www.bureauveritas.fr/), [Hydrocean](https://marine-offshore.bureauveritas.com/bvsolutions), [STX France](http://chantiers-atlantique.com/en/), [LHEEA](https://lheea.ec-nantes.fr/) for the initial version.

(c) 2015-2018 [SIREHNA](http://www.sirehna.com/) & [Naval Group](https://www.naval-group.com/en/) for all subsequent versions.

**Disclaimer**: [Documentation](https://sirehna.github.io/xdyn)
was written for a French project, with
French participants, therefore it is in French. It will be translated
eventually. Also, please note that it is still a work-in-progress and, as such,
can be incomplete. Despite our best efforts, inaccuracies may remain. However,
the documentation will continue to evolve as new developments on xdyn are
on-going.

## Getting Started

The easiest way to run xdyn is to use [Docker](https://www.docker.com/):

~~~~~~~{.bash}
docker run sirehna/xdyn
~~~~~~~

This does not require installing or downloading anything except Docker itself.

Pre-built binaries of xdyn are also available:

- [for Debian 9](https://github.com/sirehna/xdyn/releases/latest/download/xdyn_binary_debian9_amd64.deb)
- [for Debian 10](https://github.com/sirehna/xdyn/releases/latest/download/xdyn_binary_debian10_amd64.deb)
- [for Windows](https://github.com/sirehna/xdyn/releases/latest/download/xdyn_binary_windows_64_posixthread.zip)

There are many other ways of using xdyn, all of which are described
in [the documentation](https://sirehna.github.io/xdyn).

The **models** implemented inside xdyn are described in detail from
[section 11](https://sirehna.github.io/xdyn/#modélisation-des-efforts-de-diffraction-et-de-radiation)
to
[section 13](https://sirehna.github.io/xdyn/#efforts-commandés)
of the documentation.
You can also learn how to use xdyn using the
[tutorials](https://sirehna.github.io/xdyn/#tutoriels).

## Building xdyn from source

This section describes what you need to do if you wish to compile xdyn
yourself.
These instructions will get you a copy of the project up and running on your
local machine for development and testing purposes. See deployment for notes on
how to deploy the project on a live system.

### Prerequisites

To build xdyn, the easiest is to use [Docker](https://www.docker.com/).

Depending on your platform, please refer to:

- [Get Docker CE for CentOS](https://docs.docker.com/install/linux/docker-ce/centos/)
- [Get Docker CE for Debian](https://docs.docker.com/install/linux/docker-ce/debian/)
- [Get Docker CE for Fedora](https://docs.docker.com/install/linux/docker-ce/fedora/)
- [Get Docker CE for Mac](https://docs.docker.com/docker-for-mac/install/)
- [Install Docker for Windows](https://docs.docker.com/docker-for-windows/install/)

You also need [make](https://en.wikipedia.org/wiki/Make_(software)).

### Installing

Once Docker is installed, use:

~~~~~~~{.bash}
make
~~~~~~~

to build both the Windows & the Debian versions.

If you only want one of the two versions, you can use:

~~~~~~~{.bash}
make debian
~~~~~~~

or

~~~~~~~{.bash}
make windows
~~~~~~~

which performs a cross-compilation for Windows using GCC.

The binaries can then be found in `build_windows` or `build_debian`,
respectively.

If you wish to build only part of the project, use:

~~~~~~~{.bash}
./ninja_windows.sh package
~~~~~~~

for example, to build the Windows package (ZIP file containing the xdyn executable)

or

~~~~~~~{.bash}
./ninja_debian.sh run_all_tests
~~~~~~~

to rebuild the Debian tests.

Once the build has finished, you can run the tests.

## Running the tests

To run the tests for Debian use:

~~~~~~~{.bash}
./run_all_tests_debian.sh
~~~~~~~

To run the Windows tests (using Wine in a Docker container) enter:

~~~~~~~{.bash}
./run_all_tests_windows.sh
~~~~~~~

The tests are written using Google test. These are both end-to-end tests and
unit tests. The end-to-end tests can be a bit long so you can disable them
using Google tests filters:

    ./run_all_tests_debian.sh --gtest_filter=-'*LONG*'

All arguments after the script name are passed to the GTest executable. Please
refer to [the Google Test documentation for details and other available
options](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#running-a-subset-of-the-tests).

## Running xdyn

### Running Windows or Debian xdyn

Install the xdyn executable (for debian, the command is `sudo dpkg -i xdyn.deb`), then run:

~~~~~~~{.bash}
xdyn <yaml file> [xdyn options]
~~~~~~~

All options can be found in [the documentation](https://sirehna.github.io/xdyn/#ligne-de-commande).

For example, to run the first [tutorial](https://sirehna.github.io/xdyn/#tutoriels):

~~~~~~~{.bash}
./xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1
~~~~~~~

### Running xdyn on Debian with Docker

To create a Docker image containing xdyn, run:

~~~~~~~{.bash}
make debian
make docker
~~~~~~~

To run the xdyn Docker container, use:

~~~~~~~{.bash}
docker run -it --rm -u $(id -u):$(id -g) -v $(pwd):/build -w /build/path_to_yaml_file xdyn <yaml file> [xdyn options]
~~~~~~~

- Flag `--rm` deletes the container (not the image) after exit
- Flag `-u $(id -u):$(id -g)` launches the container with the permissions
of the current user, which ensures the files generated by xdyn (if any) are
owned by the current user
- Flag `-v $(pwd):/build` mounts the current directory to the container's
`/build` directory
- Flag `-w /build/path_to_yaml_file` sets the container's working directory and
`path_to_yaml_file` should be replaced by a sub-path of the current directory

More details can be found in
[Docker's official documentation](https://docs.docker.com/engine/reference/commandline/run/).

For example, to run the first [tutorial](https://sirehna.github.io/xdyn/#tutoriels)
and display the results in the terminal, assuming we are in the project's root
directory:

~~~~~~~{.bash}
docker run -it --rm -u $(id -u):$(id -g) -v $(pwd):/build -w /build/build_debian/executables/demos xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o tsv
~~~~~~~

Results can be plotted using [Matplotlib](https://matplotlib.org/) and
[pandas](https://pandas.pydata.org/) (which can be installed using
`pip3 install matplotlib pandas`):

~~~~~~~{.bash}
docker run -it --rm -u $(id -u):$(id -g) -v $(pwd):/build -w /build/build_debian/executables/demos xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 1 -o csv | python3 postprocessing/Python/plot.py tutorial_01_plot 0 3
~~~~~~~

## Deployment

Add additional notes about how to deploy this on a live system.

## Built With

* [CMake](https://cmake.org/) - Used to compile C++ code for various platforms.
* [Make](https://www.gnu.org/software/make/) - Used for the one-step build described above.
* [GCC](http://gcc.gnu.org/) - Compiler used for both the Windows & Linux: Visual Studio is currently **not** supported.
* [Boost](https://www.boost.org/) - For command-line options, regular expressions, filesystem library.
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) - To parse the input files.
* [HDF5](https://support.hdfgroup.org/products/hdf5_tools/index.html) - To store the outputs.
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) - For matrix manipulations.
* [Pandoc](https://pandoc.org/) - To generate the documentation.
* [Pweave](http://mpastell.com/pweave/) - To generate the tutorials.
* [SSC](https://github.com/sirehna/ssc) - For websockets, units decoding, interpolations, kinematics, CSV file reading and exception handling.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to submit
issues & pull requests to xdyn.
Our code of conduct is the [Contributor Covenant](CODE_OF_CONDUCT.md) (original
version available
[here](https://www.contributor-covenant.org/version/1/4/code-of-conduct) ).

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/sirehna/xdyn/tags).

## Authors

* [Charles-Edouard CADY](https://github.com/CharlesEdouardCady)
* [Guillaume JACQUENOT](https://github.com/GuillaumeJacquenot)

See also the list of [contributors](https://github.com/sirehna/xdyn/contributors) who participated in this project.

## License

This project is licensed under the Eclipse Public License (version 2) - see the [LICENSE.md](LICENSE.md) file for details.

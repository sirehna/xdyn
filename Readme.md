To build, the easiest is to use Docker.
Once docker is installed, use:

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

which performs a cross-compilation for Windows.

The binaries can then be found in `build_windows` or `build_debian`, respectively.


Afterwards, you can just run


~~~~~~~{.bash}
./run_all_tests_debian.sh
~~~~~~~

to run the tests for Debian or

~~~~~~~{.bash}
./run_all_tests_windows.sh
~~~~~~~

to run the Windows tests.

Of course, you can filter the tests (all arguments are passed to the GTest executable):


~~~~~~~{.bash}
./run_all_tests_debian.sh --gtest_filter='Sim*'

~~~~~~~

If you wish to rebuild part of the project, use:

~~~~~~~{.bash}
./ninja_windows.sh package
~~~~~~~

for example, to build the Windows package (ZIP file containing the XDYN executable)

or

~~~~~~~{.bash}
./ninja_debian.sh run_all_tests
~~~~~~~

to rebuild the Debian tests.

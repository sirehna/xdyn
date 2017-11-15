To build, the easiest is to use Docker.
Once docker is installed, use:

~~~~~~~{.bash}
make
~~~~~~~

for the initial build.

Afterwards, you can just run


~~~~~~~{.bash}
./docker_ninja.sh
~~~~~~~

with an optional target, eg.

~~~~~~~{.bash}
./docker_ninja.sh run_all_tests
~~~~~~~


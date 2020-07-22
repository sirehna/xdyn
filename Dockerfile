# Steps to produce sirehna/xdyn docker image
#
# make debian
# cp code/build_deb10/xdyn.deb .
# docker build . -f Dockerfile --tag sirehna/xdyn
# docker run sirehna/xdyn --help
FROM debian:stable-slim
RUN apt-get update -yq && \
    apt-get install \
        --yes \
        --no-install-recommends \
        libgfortran5 \
        libquadmath0 && \
    apt-get autoclean && \
    apt-get autoremove && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /var/lib/apt/lists

ADD xdyn.deb /
RUN dpkg -r xdyn && \
    dpkg -i xdyn.deb
RUN echo "#!/bin/sh" > /usr/bin/xdyn_cli.sh && \
    echo "if [ \$# -eq 0 ];" >> /usr/bin/xdyn_cli.sh && \
    echo "then" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"XDyn docker help\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"- Get help\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"docker run sirehna/xdyn --help\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"- Run XDyn with input files located in pwd\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"docker run --rm -u \\\`id -u\\\`:\\\`id -g\\\` -v \\\`pwd\\\`:/data -w /data sirehna/xdyn `/usr/bin/xdyn --help | grep USAGE | cut -f3- -d" "`\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"- Getting started\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  # Copy tutorials in local directory\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  mkdir xdyn_demos\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  cd xdyn_demos\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  docker run --rm -u \\\`id -u\\\`:\\\`id -g\\\` -v \\\`pwd\\\`:/xdyn_demos --entrypoint /bin/bash sirehna/xdyn -c \\\"cp -rf /usr/demos/* /xdyn_demos/.\\\"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  # List all tutorials\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  ls tuto*.yml\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  # Run tutorial tutorial_01_falling_ball\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"  docker run --rm -u \\\`id -u\\\`:\\\`id -g\\\` -v \\\`pwd\\\`:/xdyn_demos -w /xdyn_demos sirehna/xdyn tutorial_01_falling_ball.yml --dt 0.1 --tend 10\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"- Below is XDyn help\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    /usr/bin/xdyn --help" >> /usr/bin/xdyn_cli.sh && \
    echo "else" >> /usr/bin/xdyn_cli.sh && \
    echo "    /usr/bin/xdyn \$@" >> /usr/bin/xdyn_cli.sh && \
    echo "fi" >> /usr/bin/xdyn_cli.sh && \
    chmod a+x /usr/bin/xdyn_cli.sh




ENTRYPOINT ["/usr/bin/xdyn_cli.sh"]

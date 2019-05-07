# docker build . -f Dockerfile --tag xdyn
# docker run xdyn --help
FROM debian:stretch-slim
RUN apt-get update -yq && \
    apt-get install \
        --yes \
        --no-install-recommends \
        libgfortran3 \
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
    echo "    echo \"docker run sirehna/xdyn --help\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"- Run XDyn with input files located in pwd\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"docker run --rm -u \\\`id -u\\\`:\\\`id -g\\\` -v \\\`pwd\\\`:/data -w /data sirehna/xdyn `/usr/bin/xdyn --help | grep USAGE | cut -f3- -d" "`\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    echo \"\"" >> /usr/bin/xdyn_cli.sh && \
    echo "    /usr/bin/xdyn --help" >> /usr/bin/xdyn_cli.sh && \
    echo "else" >> /usr/bin/xdyn_cli.sh && \
    echo "    /usr/bin/xdyn \$@" >> /usr/bin/xdyn_cli.sh && \
    echo "fi" >> /usr/bin/xdyn_cli.sh && \
    chmod a+x /usr/bin/xdyn_cli.sh

ENTRYPOINT ["/usr/bin/xdyn_cli.sh"]

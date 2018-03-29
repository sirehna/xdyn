FROM python:3.6.4-slim-jessie
RUN apt-get update -yq && \
    apt-get install --yes --no-install-recommends wget python3-numpy python3-matplotlib && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* && \
    rm -rf /var/lib/apt/lists/ && \
    python3 -m pip install pweave
ENTRYPOINT ["pweave"]

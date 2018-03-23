FROM python:3.4

# Install necessary python packages
RUN pip3 install python-gitlab==1.1.0 python-dateutil

# Need all this cruft so that the generated files can be deleted by Gitlab's CI
ARG UID
ARG GID
ARG USER

# Create group & user at build time
RUN if [ $USER != root ]; then groupadd -g $GID $USER; fi \
 && if [ $USER != root ]; then useradd -m -u $UID -g $GID -s /bin/bash $USER; fi

# Add the Python script doing all the work
ADD fetch_gitlab_artifacts.py /

# At run time, use group & user created at build time to change permissions of generated artifacts
ENTRYPOINT ["python3", "fetch_gitlab_artifacts.py"]

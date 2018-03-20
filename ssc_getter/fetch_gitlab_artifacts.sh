#!/bin/sh
python3 /fetch_gitlab_artifacts.py `echo $*`
chown -R $USER artifacts.zip
chgrp -R $USER artifacts.zip

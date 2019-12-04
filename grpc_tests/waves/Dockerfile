FROM python:3.8-slim-buster
ADD requirements.txt .
RUN pip3 install -r requirements.txt
ADD wait-for-it.sh tests.py ./
ENTRYPOINT ./wait-for-it.sh xdyn:9002 -s -t 20 -- nosetests -x -v -d --nocapture --nologcapture --rednose

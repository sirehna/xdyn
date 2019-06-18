FROM python:3.6

ADD requirements.txt .
RUN pip3 install -r requirements.txt
RUN mkdir -p /usr/python
RUN chmod a+rwx /usr/python
ENV PYTHONPATH /usr/python
RUN echo "#!/bin/bash" > entrypoint.sh
RUN echo "python3 -m grpc_tools.protoc -I /proto --python_out=/usr/python --grpc_python_out=/usr/python/ force.proto" >> entrypoint.sh
RUN echo "python3 \"\$@\"" >> entrypoint.sh
RUN chmod a+x entrypoint.sh
ADD force.py /usr/python
ENTRYPOINT ["./entrypoint.sh"]

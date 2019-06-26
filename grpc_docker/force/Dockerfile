FROM python:3.7-slim-stretch

ADD requirements.txt .
RUN python -m pip install -r requirements.txt && \
    mkdir -p /usr/python && \
    chmod a+rwx /usr/python
ENV PYTHONPATH /usr/python
ADD wave_types.proto .
ADD force.proto .
RUN python -m grpc_tools.protoc -I . --python_out=/usr/python --grpc_python_out=/usr/python/ wave_types.proto && \
    python -m grpc_tools.protoc -I . --python_out=/usr/python --grpc_python_out=/usr/python/ force.proto
ADD force.py /usr/python
ENTRYPOINT ["python"]

This directory contains gRPC demos that run with xdyn.
These demos show how user can define its own models that
can be called by xdyn with gRPC.

They are based on docker-compose to create and organize
gRPC communications between services.

Each demo contains at least two services:

- one that runs xdyn
- a second one that runs the user implemented service

Directory `force00` illustrates the use of docker image
`xdyn-force-grpc:python3`.
It is presented in detail with tutorial 10.

Directory `waves00` illustrates the use of docker image
`xdyn-waves-grpc:python3`.
It is presented in detail with tutorial 09.

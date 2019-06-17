.PHONY: all

all:
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f grpc_wave_compose.yml up -t 0 --abort-on-container-exit --build

.PHONY: all

all:
	cp ../code/waves_grpc/waves.proto ../code/waves_grpc/python_server
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f grpc_wave_compose.yml up -t 0 --exit-code-from test --abort-on-container-exit --build
	rm -f ../code/waves_grpc/python_server/waves.proto

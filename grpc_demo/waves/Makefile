.PHONY: all

all:
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
		-c "cp /usr/demos/tutorial_09_gRPC_wave_model.yml /xdyn_demos/tutorial_09_gRPC_wave_model.yml" 
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f grpc_wave_compose.yml up -t 0 --exit-code-from test --abort-on-container-exit --build
	rm -f tutorial_09_gRPC_wave_model.yml

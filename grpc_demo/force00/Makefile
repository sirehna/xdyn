.PHONY: all

all:
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
		-c "cp /usr/demos/tutorial_10_gRPC_force_model.yml /xdyn_demos/tutorial_10_gRPC_force_model.yml" 
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from xdyn --abort-on-container-exit --build
	rm -f tutorial_10_gRPC_force_model.yml

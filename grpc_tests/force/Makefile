.PHONY: all cosim sim

all: cosim sim

cosim:
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_10_gRPC_force_model*.yml /xdyn_demos/"
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-cosim.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build
	rm -f tutorial_10_gRPC_force_model.yml

sim:
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_10_gRPC_force_model*.yml /xdyn_demos/"
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-sim.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build
	#rm -f tutorial_10_gRPC_force_model.yml

clean:
	rm -f tutorial_10_gRPC_force_model*.yml
	rm -rf __pycache__

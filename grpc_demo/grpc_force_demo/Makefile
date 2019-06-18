.PHONY: all

all:
	cp ../../xdyn_demos/tutorial_10_gRPC_force_model.yml .
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from xdyn --abort-on-container-exit --build
	rm -f tutorial_10_gRPC_force_model.yml

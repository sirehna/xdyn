.PHONY: all clean

all: clean
	cp ../../code/grpc/cosimulation.proto ../../code/grpc/model_exchange.proto .
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_01_falling_ball.yml /xdyn_demos/"
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build

clean:
	rm -f tutorial_10_gRPC_force_model*.yml
	rm -rf __pycache__
	rm -rf out.csv
	rm -rf *.proto

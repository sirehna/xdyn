.PHONY: all clean

all: clean model_exchange.proto
	docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_01_falling_ball.yml /xdyn_demos/"
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose rm -f
	CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose up -t 0 --exit-code-from test --abort-on-container-exit --build

model_exchange.proto: ../../code/grpc/model_exchange.proto
	cp $< $@

clean:
	rm -f tutorial_*.yml model_exchange.proto
	rm -rf __pycache__
	rm -rf out.csv
	rm -rf *.proto

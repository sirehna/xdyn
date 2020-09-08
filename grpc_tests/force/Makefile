.PHONY: all clean cosim sim input_dependencies

all: cosim sim clean

input_dependencies:
	@docker run --rm -u $(shell id -u ):$(shell id -g) -v $(shell pwd):/xdyn_demos \
        --entrypoint /bin/bash xdyn \
        -c "cp /usr/demos/tutorial_10_gRPC_force_model*.yml /xdyn_demos/"

cosim: input_dependencies
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-cosim.yml rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-cosim.yml up -t 0 --exit-code-from test --abort-on-container-exit --build

sim: input_dependencies
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-sim.yml rm -f
	@CURRENT_UID=$(shell id -u):$(shell id -g) docker-compose -f docker-compose-sim.yml up -t 0 --exit-code-from xdyn --abort-on-container-exit --build
	@echo "Checking the generated CSV file has 4 lines (including the header line)..."
	@if [ `cat out.csv | wc -l ` -eq 4 ]i; then echo "...OK!"; else echo "***error: CSV file has `cat out.csv | wc -l ` line(s)" && exit 1; fi

clean:
	rm -f tutorial_10_gRPC_force_model*.yml
	rm -rf __pycache__
	rm -rf out.csv

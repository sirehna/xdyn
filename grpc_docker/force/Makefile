.PHONY: all

all:
	cp ../../code/grpc/force.proto .
	cp ../../code/waves_grpc/wave_grpc.proto .
	cp ../../code/waves_grpc/wave_types.proto .
	docker build . -t xdyn-force-grpc:python3
	rm -f force.proto wave_grpc.proto wave_types.proto

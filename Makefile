all: spt-0.0.0-py2.py3-none-any.whl ssc.deb
	cp spt-0.0.0-py2.py3-none-any.whl ../make_docker_image
	mkdir -p build
	docker network create hos-docker-network 2> /dev/null || true
	docker build -f Dockerfile -t build-xdyn --build-arg CACHEBUST=$(shell date +%s) .
	docker run --rm -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) -v $(shell pwd)/build:/build -w /build -v $(shell pwd):/opt/share -i build-xdyn cmake -Wno-dev -G Ninja -DINSTALL_PREFIX:PATH=/opt/xdyn -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake -DHDF5_DIR:PATH=/usr/local/hdf5 -DBOOST_ROOT:PATH=/usr/local/boost_1_60_0 /opt/share/code
	docker run --rm -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) -v $(shell pwd)/build:/build -w /build -v $(shell pwd):/opt/share -i build-xdyn /bin/sh -c "cd /opt/share/doc_user/images && make -i > /dev/null 2> /dev/null && cd /build && ninja package"
	docker run --rm -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) -v $(shell pwd)/build:/build -w /build -v $(shell pwd):/opt/share -i --net=hos-docker-network build-xdyn ./run_all_tests --gtest_output=xml:run_all_tests.xml


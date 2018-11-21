all: windows debian doc

windows: fetch-ssc-windows cmake-windows package-windows
debian: fetch-ssc-debian cmake-debian package-debian

.PHONY: fetch-ssc-windows cmake-windows package-windows windows doc

fetch-ssc-windows:
	./fetch_gitlab_artifacts.sh -c d79f2271c954ee01fbc536e162667684adf20079 --project_id 42 -b windows
	rm -rf ssc_windows
	mkdir ssc_windows
	unzip ssc.zip -d ssc_windows
	rm ssc.zip

cmake-windows:
	mkdir -p build_windows
	docker run --name xdyn-cmake-windows --rm \
        -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
        -v $(shell pwd):/opt/share \
        -w /opt/share mydockcross/windows-x64 \
          /bin/bash -c "mkdir -p /opt/share/.wine && \
                        export WINEPREFIX=/opt/share/.wine && \
                        wine winecfg && \
                        mkdir -p executables && \
                        cd build_windows && \
                        cmake -Wno-dev \
                        -G Ninja \
                        -Dssc_DIR:PATH=/opt/share/ssc_windows/lib/ssc/cmake \
                        -DCMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
                        -DCPACK_GENERATOR=ZIP \
                        -DBoost_DEBUG=0 \
                        -DBOOST_ROOT:PATH=/opt/boost \
                        -DBOOST_INCLUDEDIR:PATH=/opt/boost/include \
                        -DBoost_INCLUDE_DIR:PATH=/opt/boost/include \
                        -DBOOST_LIBRARYDIR:PATH=/opt/boost/lib \
                        -DBoost_NO_SYSTEM_PATHS:BOOL=OFF \
                        -DBoost_LIBRARY_DIR_RELEASE:PATH=/opt/boost/lib \
                        -DBoost_PROGRAM_OPTIONS_LIBRARY:PATH=/opt/boost/lib/libboost_program_options-mt.a \
                        -DBoost_FILESYSTEM_LIBRARY:PATH=/opt/boost/lib/libboost_filesystem-mt.a \
                        -DBoost_SYSTEM_LIBRARY:PATH=/opt/boost/lib/libboost_system-mt.a \
                        -DBoost_REGEX_LIBRARY:PATH=/opt/boost/lib/libboost_regex-mt.a \
                        -DHDF5_DIR=/opt/HDF5_1_8_20/cmake \
                        -Dcppzmq_DIR=/opt/libzmq/share/cmake/cppzmq \
                        -DZeroMQ_DIR=/opt/libzmq/share/cmake/ZeroMQ \
                        -DProtobuf_INCLUDE_DIR=/opt/protobuf/include \
                        -DProtobuf_LIBRARY=/opt/protobuf/lib/libprotobuf.a \
                        -DProtobuf_PROTOC_EXECUTABLE=/usr/bin/protoc \
                        -DCMAKE_SYSTEM_VERSION=7 \
                        /opt/share/code"

fetch-ssc-debian:
	./fetch_gitlab_artifacts.sh -c d79f2271c954ee01fbc536e162667684adf20079 --project_id 42 -b debian

package-windows:
	./ninja_windows.sh package

cmake-debian:
	mkdir -p build_debian
	docker build -t build-xdyn-debian --build-arg CACHEBUST=$(date +%s) .
	docker run --name xdyn-cmake-debian --rm \
        -e LD_LIBRARY_PATH=/opt/ssc/lib \
        -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
        -v $(shell pwd)/build_debian:/build \
        -w /build \
        -v $(shell pwd):/opt/share \
        -i build-xdyn-debian cmake \
                -Wno-dev \
                -G Ninja \
                -DINSTALL_PREFIX:PATH=/opt/xdyn \
                -Dssc_DIR:PATH=/opt/ssc/lib/ssc/cmake \
                -DHDF5_DIR:PATH=/usr/local/hdf5 \
                -DBOOST_ROOT:PATH=/usr/local/boost_1_60_0 \
                -DProtobuf_LIBRARY=/usr/local/lib/libprotobuf.a \
                /opt/share/code

package-debian:
	./ninja_debian.sh package

doc:
	mkdir -p build_doc
	cd build_doc && cp ../Dockerfile_doc Dockerfile && cd ..
	cd build_doc && docker build -t build-xdyn-doc --build-arg CACHEBUST=$(date +%s) . && cd ..
	docker run --name xdyn-doc --rm \
        -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
        -v $(shell pwd):/opt/share \
        -w /opt/share build-xdyn-doc \
        /bin/bash -c "cd doc_user && cd images && make && cd .. && ./doc_html.sh && mv doc.html .."

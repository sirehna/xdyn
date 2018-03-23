all: windows

windows: fetch-ssc-windows cmake-windows package-windows

.PHONY: fetch-ssc-windows cmake-windows package-windows windows

fetch-ssc-windows:
	./fetch_gitlab_artifacts.sh -c e3491f5ad68a11ac0414e496871429f74aacc493 --project_id 42 -b windows
	rm -rf ssc_windows
	mkdir ssc_windows
	unzip ssc.zip -d ssc_windows
	rm ssc.zip

cmake-windows:
	mkdir -p build_windows
	docker run --name xdyn-cmake-windows --rm -v /etc/group:/etc/group:ro  -v /etc/passwd:/etc/passwd:ro -u $(shell id -u ${USER} ):$(shell id -g ${USER} ) -v $(shell pwd):/opt/share -w /opt/share mydockcross/windows-x64 \
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


package-windows:
	./ninja_windows.sh package



all: submodule windows debian debug doc all_docker_images

windows: windows_gccx_posix
debian: debian_10_release_gcc_8
debug: debian_10_debug_gcc_8

.PHONY: fetch-ssc-windows cmake-windows package-windows windows doc submodule


submodule:
	git submodule sync --recursive
	git submodule update --init --recursive

cmake-debian: BUILD_TYPE = Release
cmake-debian: BUILD_DIR = build_deb10
cmake-debian: CPACK_GENERATOR = DEB
cmake-debian: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn
cmake-debian: BOOST_ROOT = /opt/boost
cmake-debian: SSC_ROOT = /opt/ssc
cmake-debian: ci_env=
cmake-debian: cmake-debian-target

cmake-windows: BUILD_TYPE=Release
cmake-windows: BUILD_DIR=build_win
cmake-windows: CPACK_GENERATOR=ZIP
cmake-windows: DOCKER_IMAGE=sirehna/base-image-win64-gcc540-win32threads-ssc-xdyn
cmake-windows: BOOST_ROOT=/usr/src/mxe/usr/x86_64-w64-mingw32.static
cmake-windows: SSC_ROOT=/opt/ssc
cmake-windows: HDF5_DIR=/opt/HDF5_1_8_20/cmake
cmake-windows: ci_env=
cmake-windows: cmake-windows-target

debian_8_release_gcc_492: BUILD_TYPE = Release
debian_8_release_gcc_492: BUILD_DIR = build_deb8
debian_8_release_gcc_492: CPACK_GENERATOR = DEB
debian_8_release_gcc_492: DOCKER_IMAGE = sirehna/base-image-debian8-gcc492-xdyn
debian_8_release_gcc_492: BOOST_ROOT = /opt/boost
debian_8_release_gcc_492: SSC_ROOT = /opt/ssc
debian_8_release_gcc_492: ci_env=
debian_8_release_gcc_492: cmake-debian-target build-debian test-debian

debian_9_release_gcc_6: BUILD_TYPE = Release
debian_9_release_gcc_6: BUILD_DIR = build_deb9
debian_9_release_gcc_6: CPACK_GENERATOR = DEB
debian_9_release_gcc_6: DOCKER_IMAGE = sirehna/base-image-debian9-gcc6-xdyn
debian_9_release_gcc_6: BOOST_ROOT = /opt/boost
debian_9_release_gcc_6: SSC_ROOT = /opt/ssc
debian_9_release_gcc_6: ci_env=
debian_9_release_gcc_6: cmake-debian-target build-debian test-debian

debian_10_release_gcc_8: BUILD_TYPE = Release
debian_10_release_gcc_8: BUILD_DIR = build_deb10
debian_10_release_gcc_8: CPACK_GENERATOR = DEB
debian_10_release_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn
debian_10_release_gcc_8: BOOST_ROOT = /opt/boost
debian_10_release_gcc_8: SSC_ROOT = /opt/ssc
debian_10_release_gcc_8: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_10_release_gcc_8: ci_env=
debian_10_release_gcc_8: cmake-debian-target build-debian test-debian

debian_10_coverage_gcc_8: BUILD_TYPE = Coverage
debian_10_coverage_gcc_8: BUILD_DIR = build_deb10
debian_10_coverage_gcc_8: CPACK_GENERATOR = DEB
debian_10_coverage_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn
debian_10_coverage_gcc_8: BOOST_ROOT = /opt/boost
debian_10_coverage_gcc_8: SSC_ROOT = /opt/ssc
debian_10_coverage_gcc_8: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_10_coverage_gcc_8: ci_env=`bash <(curl -s https://codecov.io/env)`
debian_10_coverage_gcc_8: cmake-debian-target build-debian test-debian

debian_10_debug_gcc_8: BUILD_TYPE = Debug
debian_10_debug_gcc_8: BUILD_DIR = build_deb10_dbg
debian_10_debug_gcc_8: CPACK_GENERATOR = DEB
debian_10_debug_gcc_8: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn
debian_10_debug_gcc_8: BOOST_ROOT = /opt/boost
debian_10_debug_gcc_8: SSC_ROOT = /opt/ssc
debian_10_debug_gcc_8: ci_env=
debian_10_debug_gcc_8: cmake-debian-target build-debian

windows_gccx_posix: BUILD_TYPE=Release
windows_gccx_posix: BUILD_DIR=build_win_posix
windows_gccx_posix: CPACK_GENERATOR=ZIP
windows_gccx_posix: DOCKER_IMAGE=sirehna/base-image-win64-gccx-posixthreads-ssc-xdyn
windows_gccx_posix: BOOST_ROOT=/usr/src/mxe/usr/x86_64-w64-mingw32.static.posix
windows_gccx_posix: SSC_ROOT=/opt/ssc
windows_gccx_posix: HDF5_DIR=/opt/HDF5_1_8_20/cmake
windows_gccx_posix: ci_env=
windows_gccx_posix: cmake-windows-target build-windows test-windows


code/yaml-cpp/CMakeLists.txt: yaml-cpp-CMakeLists.txt
	docker run $(ci_env) --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	        "rm -rf /opt/share/code/yaml-cpp && \
	        cp -rf /opt/yaml_cpp /opt/share/code/yaml-cpp && \
	        cp /opt/share/yaml-cpp-CMakeLists.txt /opt/share/code/yaml-cpp/CMakeLists.txt"


cmake-windows-target: code/yaml-cpp/CMakeLists.txt
	docker pull $(DOCKER_IMAGE) || true
	docker run --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	       "cd /opt/share &&\
	        mkdir -p $(BUILD_DIR) &&\
	        cd $(BUILD_DIR) &&\
	        mkdir -p /opt/share/.wine;\
	        export WINEPREFIX=/opt/share/.wine;\
	        wine winecfg;\
	        cmake -Wno-dev\
	        -G Ninja \
	          -DTHIRD_PARTY_DIRECTORY=/opt \
	          -DBUILD_DOCUMENTATION:BOOL=False \
	          -DCPACK_GENERATOR=$(CPACK_GENERATOR) \
	          -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	          -DCMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
	          -DSSC_ROOT=$(SSC_ROOT) \
	          -DHDF5_DIR=$(HDF5_DIR) \
	          -DBoost_DEBUG=0 \
	          -DBOOST_ROOT:PATH=$(BOOST_ROOT) \
	          -DBOOST_INCLUDEDIR:PATH=$(BOOST_ROOT)/include \
	          -DBoost_INCLUDE_DIR:PATH=$(BOOST_ROOT)/include \
	          -DBOOST_LIBRARYDIR:PATH=$(BOOST_ROOT)/lib \
	          -DBoost_NO_SYSTEM_PATHS:BOOL=OFF \
	          -DBoost_LIBRARY_DIR_RELEASE:PATH=$(BOOST_ROOT)/lib \
	          -DBoost_PROGRAM_OPTIONS_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_program_options-mt.a \
	          -DBoost_FILESYSTEM_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_filesystem-mt.a \
	          -DBoost_SYSTEM_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_system-mt.a \
	          -DBoost_REGEX_LIBRARY:PATH=$(BOOST_ROOT)/lib/libboost_regex-mt.a \
	          -DCMAKE_SYSTEM_VERSION=7 \
	        /opt/share/code"

build-windows:
	docker run --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	       "cd /opt/share &&\
	        mkdir -p $(BUILD_DIR) &&\
	        cd $(BUILD_DIR) &&\
	        mkdir -p /opt/share/.wine;\
	        export WINEPREFIX=/opt/share/.wine;\
	        wine winecfg;\
	        ninja package"

test-windows:
	docker run --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	       "cd $(BUILD_DIR) &&\
	        mkdir -p /opt/share/.wine;\
	        export WINEPREFIX=/opt/share/.wine;\
	        wine winecfg;\
	        wine ./run_all_tests --gtest_filter=-*ocket*:*ot_throw_if_CSV_file_exists"


cmake-debian-target: SHELL:=/bin/bash
cmake-debian-target: code/yaml-cpp/CMakeLists.txt
	docker pull $(DOCKER_IMAGE) || true
	docker run $(ci_env) --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	        "cd /opt/share &&\
	        mkdir -p $(BUILD_DIR) &&\
	        cd $(BUILD_DIR) &&\
	        cmake -Wno-dev \
	         -G Ninja \
	         -DTHIRD_PARTY_DIRECTORY=/opt/ \
	         -DBUILD_DOCUMENTATION:BOOL=False \
	         -DCPACK_GENERATOR=$(CPACK_GENERATOR) \
	         -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	         -DCMAKE_INSTALL_PREFIX:PATH=/opt/xdyn \
	         -DSSC_ROOT=$(SSC_ROOT) \
	         -DHDF5_DIR=$(HDF5_DIR) \
	         -DBOOST_ROOT:PATH=$(BOOST_ROOT) \
	        /opt/share/code"

build-debian: SHELL:=/bin/bash
build-debian:
	docker run $(ci_env) --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	       "cd /opt/share && \
	        mkdir -p $(BUILD_DIR) && \
	        cd $(BUILD_DIR) && \
	        ninja package"

test-debian: SHELL:=/bin/bash
test-debian:
	docker run $(ci_env) --rm \
	    -u $(shell id -u ):$(shell id -g ) \
	    -v $(shell pwd):/opt/share \
	    -w /opt/share \
	    $(DOCKER_IMAGE) /bin/bash -c \
	       "cd $(BUILD_DIR) &&\
	        ./run_all_tests &&\
	        if [[ $(BUILD_TYPE) == Coverage ]];\
	        then\
	        echo Coverage;\
	        gprof run_all_tests gmon.out > gprof_res.txt 2> gprof_res.err;\
	        bash <(curl -s https://codecov.io/bash);\
	        fi"


docker:
	./ninja_debian.sh package && cp build_deb10/xdyn.deb . &&  docker build . --tag xdyn

docker_grpc_force_model:
	cd grpc_docker && make

docker_grpc_waves_model:
	cd code/waves_grpc/python_server && make CONTAINER_NAME=xdyn-waves-grpc:python3

all_docker_images: docker docker_grpc_force_model docker_grpc_waves_model
	echo "Build all docker images after having run 'make debian'"


doc: BUILD_TYPE = Release
doc: BUILD_DIR = build_deb10
doc: CPACK_GENERATOR = DEB
doc: DOCKER_IMAGE = sirehna/base-image-debian10-gcc8-xdyn
doc: BOOST_ROOT = /opt/boost
doc: SSC_ROOT = /opt/ssc
doc: ci_env=
doc: build-debian
	cd doc_user && \
	cp ../$(BUILD_DIR)/xdyn.deb . && \
	make && \
	mv doc.html ..

clean:
	rm -f xdyn.deb
	rm -rf build_*
	rm -rf yaml-cpp
	make -C doc_user clean; rm -f doc_user/xdyn.deb doc.html

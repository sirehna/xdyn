all: windows debian doc

windows: windows_gcc_54
debian: debian_9_release_gcc_82

.PHONY: fetch-ssc-windows cmake-windows package-windows windows doc

SSC_PROJECT_NUMBER=42
GITLAB_JOB_NUMBER_TO_USE_FOR_WINDOWS_SSC=32782
GITLAB_JOB_NUMBER_TO_USE_FOR_DEBIAN_SSC=32783


debian_8_release_gcc_492: BUILD_TYPE = Release
debian_8_release_gcc_492: BUILD_DIR = build_deb8
debian_8_release_gcc_492: CPACK_GENERATOR = DEB
debian_8_release_gcc_492: DOCKER_IMAGE = sirehna/base-image-debian8-gcc492-xdyn
debian_8_release_gcc_492: BOOST_ROOT = /opt/boost
debian_8_release_gcc_492: SSC_ROOT = /opt/ssc
debian_8_release_gcc_492: BUILD_DOCUMENTATION = False
debian_8_release_gcc_492: ci_env=
debian_8_release_gcc_492: build-debian

debian_9_release_gcc_6: BUILD_TYPE = Release
debian_9_release_gcc_6: BUILD_DIR = build_deb9
debian_9_release_gcc_6: CPACK_GENERATOR = DEB
debian_9_release_gcc_6: DOCKER_IMAGE = sirehna/base-image-debian9-gcc6-xdyn
debian_9_release_gcc_6: BOOST_ROOT = /opt/boost
debian_9_release_gcc_6: SSC_ROOT = /opt/ssc
debian_9_release_gcc_6: BUILD_DOCUMENTATION = True
debian_9_release_gcc_6: ci_env=
debian_9_release_gcc_6: build-debian

debian_9_coverage_gcc_6: BUILD_TYPE = Coverage
debian_9_coverage_gcc_6: BUILD_DIR = build_deb9
debian_9_coverage_gcc_6: CPACK_GENERATOR = DEB
debian_9_coverage_gcc_6: DOCKER_IMAGE = sirehna/base-image-debian9-gcc6-xdyn
debian_9_coverage_gcc_6: BOOST_ROOT = /opt/boost
debian_9_coverage_gcc_6: SSC_ROOT = /opt/ssc
debian_9_coverage_gcc_6: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_9_coverage_gcc_6: ci_env=$(shell bash <(curl -s https://codecov.io/env))
debian_9_coverage_gcc_6: build-debian

debian_9_release_gcc_82: BUILD_TYPE = Release
debian_9_release_gcc_82: BUILD_DIR = build_deb9_gcc820
debian_9_release_gcc_82: CPACK_GENERATOR = DEB
debian_9_release_gcc_82: DOCKER_IMAGE = sirehna/base-image-debian9-gcc820-xdyn
debian_9_release_gcc_82: BOOST_ROOT = /opt/boost
debian_9_release_gcc_82: SSC_ROOT = /opt/ssc
debian_9_release_gcc_82: HDF5_DIR = /usr/local/hdf5/share/cmake
debian_9_release_gcc_82: ci_env=
debian_9_release_gcc_82: build-debian

windows_gcc_54: BUILD_TYPE=Release
windows_gcc_54: BUILD_DIR=build_win
windows_gcc_54: CPACK_GENERATOR=ZIP
windows_gcc_54: DOCKER_IMAGE=sirehna/base-image-win64-gcc540-win32threads-ssc-xdyn
windows_gcc_54: BOOST_ROOT=/usr/src/mxe/usr/x86_64-w64-mingw32.static
windows_gcc_54: SSC_ROOT=/opt/ssc
windows_gcc_54: HDF5_DIR=/opt/HDF5_1_8_20/cmake
windows_gcc_54: ci_env=
windows_gcc_54: build-windows

build-windows:
		docker run $(ci_env) --rm -u $(shell id -u ):$(shell id -g ) -v $(shell pwd):/opt/share -w /opt/share $(DOCKER_IMAGE) /bin/bash -c \
           "rm -rf /opt/share/code/yaml-cpp &&\
            cp -rf /opt/yaml_cpp /opt/share/code/yaml-cpp &&\
            cp /opt/share/yaml-cpp-CMakeLists.txt /opt/share/code/yaml-cpp/CMakeLists.txt &&\
            rm -rf /opt/share/code/eigen3-hdf5 &&\
            cp -rf /opt/eigen3-hdf5 /opt/share/code/eigen3-hdf5 &&\
            rm -rf /opt/share/code/gcovr &&\
            rm -rf /opt/share/code/google-test &&\
            cp -rf /opt/googletest /opt/share/code/google-test &&\
            rm -rf /opt/share/code/eigen &&\
            cp -rf /opt/eigen /opt/share/code/eigen &&\
            cd /opt/share/code &&\
            mkdir -p $(BUILD_DIR) &&\
            cd $(BUILD_DIR) &&\
            mkdir -p /opt/share/.wine;\
            export WINEPREFIX=/opt/share/.wine;\
            wine winecfg;\
            cmake -Wno-dev\
            -G Ninja \
              -DBUILD_DOCUMENTATION:BOOL=$(BUILD_DOCUMENTATION) \
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
              -Dcppzmq_DIR=/opt/libzmq/share/cmake/cppzmq \
              -DZeroMQ_DIR=/opt/libzmq/share/cmake/ZeroMQ \
              -DProtobuf_USE_STATIC_LIBS:BOOL=True \
              -DProtobuf_INCLUDE_DIR=/opt/protobuf/include \
              -DProtobuf_LIBRARY=/opt/protobuf/lib/libprotobuf.a \
              -DProtobuf_PROTOC_EXECUTABLE:PATH=/usr/bin/protoc \
              -DCMAKE_SYSTEM_VERSION=7 \
           /opt/share/code &&\
            ninja package \
            &&\
            wine ./run_all_tests --gtest_filter=-*ocket*:HOSTest*:*ot_throw_if_CSV_file_exists;\
            if [[ $(BUILD_TYPE) == Coverage ]];\
            then\
            echo Coverage;\
            gprof run_all_tests gmon.out > gprof_res.txt 2> gprof_res.err;\
            bash <(curl -s https://codecov.io/bash);\
            fi"

build-debian:
		docker run $(ci_env) --rm -u $(shell id -u ):$(shell id -g ) -v $(shell pwd):/opt/share -w /opt/share $(DOCKER_IMAGE) /bin/bash -c \
           "rm -rf /opt/share/code/yaml-cpp &&\
            cp -rf /opt/yaml_cpp /opt/share/code/yaml-cpp &&\
            cp /opt/share/yaml-cpp-CMakeLists.txt /opt/share/code/yaml-cpp/CMakeLists.txt &&\
            rm -rf /opt/share/code/eigen3-hdf5 &&\
            cp -rf /opt/eigen3-hdf5 /opt/share/code/eigen3-hdf5 &&\
            rm -rf /opt/share/code/gcovr &&\
            rm -rf /opt/share/code/google-test &&\
            cp -rf /opt/googletest /opt/share/code/google-test &&\
            rm -rf /opt/share/code/eigen &&\
            cp -rf /opt/eigen /opt/share/code/eigen &&\
            cd /opt/share/code &&\
            mkdir -p $(BUILD_DIR) &&\
            cd $(BUILD_DIR) &&\
            cmake -Wno-dev \
           -G Ninja \
           -DBUILD_DOCUMENTATION:BOOL=$(BUILD_DOCUMENTATION)\
           -DCPACK_GENERATOR=$(CPACK_GENERATOR)\
           -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)\
           -DCMAKE_INSTALL_PREFIX:PATH=/opt/xdyn\
           -DSSC_ROOT=$(SSC_ROOT)\
           -DHDF5_DIR=$(HDF5_DIR)\
           -DBOOST_ROOT:PATH=$(BOOST_ROOT)\
           -DBOOST_INCLUDEDIR:PATH=$(BOOST_ROOT)/include\
           -DBoost_INCLUDE_DIR:PATH=$(BOOST_ROOT)/include\
           -DBOOST_LIBRARYDIR:PATH=$(BOOST_ROOT)/lib\
           -DBoost_NO_SYSTEM_PATHS:BOOL=OFF\
           -DBoost_LIBRARY_DIR_RELEASE:PATH=$(BOOST_ROOT)/lib\
           /opt/share/code &&\
            ninja package \
            &&\
            ./run_all_tests --gtest_filter=-HOSTest*;\
            if [[ $(BUILD_TYPE) == Coverage ]];\
            then\
            echo Coverage;\
            gprof run_all_tests gmon.out > gprof_res.txt 2> gprof_res.err;\
            bash <(curl -s https://codecov.io/bash);\
            fi"

doc:
	mkdir -p build_doc
	cd build_doc && cp ../Dockerfile_doc Dockerfile && cd ..
	cd build_doc && docker build -t build-xdyn-doc --build-arg CACHEBUST=$(date +%s) . && cd ..
	docker run --name xdyn-doc --rm \
		-u $(shell id -u ${USER} ):$(shell id -g ${USER} ) \
		-v $(shell pwd):/opt/share \
		-w /opt/share build-xdyn-doc \
		/bin/bash -c "cd doc_user && cd images && make && cd .. && ./doc_html.sh && mv doc.html .."

doc_pweave:
	cd doc_user && \
	cp ../xdyn.deb . && \
	make && \
	mv doc.html ..

#FROM scratch
#FROM debian:8
#FROM gliderlabs/alpine:3.4
#FROM frolvlad/alpine-glibc
#FROM debian:jessie-slim
FROM bitnami/minideb:jessie
#ENV LD_LIBRARY_PATH=/
ENV LD_LIBRARY_PATH=/usr/bin
#ENV PATH=$PATH:/
#RUN apk update && apk add util-linux pciutils usbutils coreutils binutils findutils grep
#ADD xdyn.deb ./
#RUN dpkg -i xdyn.deb

ADD usr/lib/libgfortran.so.3.0.0 /usr/lib/libgfortran.so.3
ADD usr/lib/libquadmath.so.0.0.0 /usr/lib/libquadmath.so.0
ADD usr/lib/libstdc++.so.6.0.20 /usr/lib/libstdc++.so.6
ADD usr/lib/libm-2.19.so /usr/lib/libm.so.6
ADD usr/lib/libgcc_s.so.1 /usr/lib/libgcc_s.so.1
ADD usr/lib/libc-2.19.so /usr/lib/libc.so.6
ADD usr/lib/libpthread-2.19.so /usr/lib/libpthread.so.0
ADD usr/lib/libdl-2.19.so /usr/liblibdl.so.2
ADD usr/bin/libssc_csv_file_reader_shared.so /usr/bin/
ADD usr/bin/libssc_data_source_shared.so /usr/bin/
ADD usr/bin/libssc_decode_unit_shared.so /usr/bin/
ADD usr/bin/libssc_exception_handling_shared.so /usr/bin/
ADD usr/bin/libssc_integrate_shared.so /usr/bin/
ADD usr/bin/libssc_interpolation_shared.so /usr/bin/
ADD usr/bin/libssc_kinematics_shared.so /usr/bin/
ADD usr/bin/libssc_numeric_shared.so /usr/bin/
ADD usr/bin/libssc_text_file_reader_shared.so /usr/bin/
ADD usr/bin/libssc_websocket_shared.so /usr/bin/
ADD usr/bin/libssc_yaml_parser_shared.so /usr/bin/
ADD usr/bin/xdyn* /usr/bin/
ADD usr/lib/libx-dyn.so /usr/lib/
ADD usr/lib/ld-2.19.so /usr/lib/ld-linux-x86-64.so.2
#ADD usr/lib/ld-2.19.so /usr/lib/ld-linux-x86-64.so.2
#
#libx-dyn.so => /libx-dyn.so (0x00007f9794213000)
#
#
#
#libssc_kinematics_shared.so => /libssc_kinematics_shared.so (0x00007f9793e73000)
#libssc_data_source_shared.so => /libssc_data_source_shared.so (0x00007f9793b7f000)
#libssc_numeric_shared.so => /libssc_numeric_shared.so (0x00007f979397c000)
#libssc_yaml_parser_shared.so => /libssc_yaml_parser_shared.so (0x00007f9793701000)
#libssc_text_file_reader_shared.so => /libssc_text_file_reader_shared.so (0x00007f97934fb000)
#libssc_decode_unit_shared.so => /libssc_decode_unit_shared.so (0x00007f97932c1000)
#libssc_csv_file_reader_shared.so => /libssc_csv_file_reader_shared.so (0x00007f97930a5000)
#libssc_interpolation_shared.so => /libssc_interpolation_shared.so (0x00007f9792e36000)
#libssc_integrate_shared.so => /libssc_integrate_shared.so (0x00007f9792bf1000)
#libssc_websocket_shared.so => /libssc_websocket_shared.so (0x00007f9792583000)
#libssc_exception_handling_shared.so => /libssc_exception_handling_shared.so (0x00007f9792381000)
#libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f9792076000)
#libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f9791d75000)
#libgcc_s.so.1 => /libgcc_s.so.1 (0x00007f9791b5f000)
#libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f97917b4000)
#libgfortran.so.3 => /libgfortran.so.3 (0x00007f9791496000)
#libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f9791279000)
#libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f9791075000)
#libquadmath.so.0
#ADD usr/lib/lib* /
#ADD usr/lib/libgfortran.so.3.0.0 /libgfortran.so.3
#ADD usr/lib/libquadmath.so.0.0.0 /libquadmath.so.0
#ADD usr/bin/libssc_* usr/bin/xdyn* /
#RUN ln -s libgfortran.so.3.0.0 libgfortran.so.3
#RUN ln -s libquadmath.so.0.0.0 libquadmath.so.0

#ADD usr/bin/libssc_csv_file_reader_shared.so \
#    usr/bin/libssc_data_source_shared.so \
#    usr/bin/libssc_decode_unit_shared.so \
#    usr/bin/libssc_exception_handling_shared.so \
#    usr/bin/libssc_integrate_shared.so \
#    usr/bin/libssc_interpolation_shared.so \
#    usr/bin/libssc_kinematics_shared.so \
#    usr/bin/libssc_numeric_shared.so \
#    usr/bin/libssc_text_file_reader_shared.so \
#    usr/bin/libssc_websocket_shared.so \
#    usr/bin/libssc_yaml_parser_shared.so \
#    /opt/ssc/lib/
#
#ADD usr/bin/xdyn \
#    /usr/bin/
#
#ADD usr/lib/libx-dyn.so \
#    /usr/lib/

#CMD ["ldd", "xdyn"]
#CMD ["xdyn"]

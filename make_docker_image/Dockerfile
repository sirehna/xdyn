FROM bitnami/minideb:jessie
ENV LD_LIBRARY_PATH=/usr/bin

RUN apt-get update -yq \
 && apt-get install python3-h5py python3-yaml python3-jinja2 python3-pip python3-matplotlib python3-pandas python3-xlrd pandoc -y \
 && apt-get clean \
 && rm -rf /tmp/* /var/tmp/* \
 && rm -rf /var/lib/apt/lists/

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
ADD spt-0.0.0-py2.py3-none-any.whl .

RUN pip3 install spt-0.0.0-py2.py3-none-any.whl
RUN sed -i 's/^backend .*/backend : Agg/' `python3 -c'import matplotlib;print(matplotlib.matplotlib_fname())'`

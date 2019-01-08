# -*- coding: utf-8 -*-
import pandas as pd
import os
import yaml
import h5py
import functools


def csv(path_csv, original_path=os.getcwd()):
    return pd.read_csv(os.path.join(original_path, path_csv), sep=',')


def h5(path_hdf5, original_path=os.getcwd(), **data_source):
    df = pd.DataFrame()
    abs_path_hdf5 = os.path.join(original_path, path_hdf5)
    if not os.path.exists(abs_path_hdf5):
        raise IOError("File '" + abs_path_hdf5 + "' was not found")
    f = h5py.File(abs_path_hdf5, 'r')
    for key, path_value in data_source.items():
        if path_value not in f:
            raise Exception('Requested key "' + path_value + '" was not found in HDF5 file : "' + abs_path_hdf5 + '"')
        df[key] = f[path_value]
    f.close()
    return df


def to_yaml(yaml_data, working_directory=os.getcwd(), filename="auto_generated.yml"):
    with open(os.path.join(working_directory, filename), 'w') as f:
        f.write(yaml.dump(yaml_data))
    return filename


def read_excel(path_xls, original_path=os.getcwd(), **kwargs):
    return pd.read_excel(os.path.join(original_path, path_xls), **kwargs)


def api(params):
    wd = params["working_directory"]
    orig_path = params["original_path"]
    return {"csv": functools.partial(csv, original_path=orig_path),
            "h5": functools.partial(h5, original_path=orig_path),
            "to_yaml": functools.partial(to_yaml, working_directory=wd),
            "xls": functools.partial(read_excel, original_path=orig_path)}

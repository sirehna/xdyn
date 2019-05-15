# -*- coding: utf-8 -*-
import yaml
import functools
import re
import os


def load_yaml(path_yaml, original_path):
    # https://github.com/yaml/pyyaml/wiki/PyYAML-yaml.load(input)-Deprecation
    # Loader=yaml.BaseLoader
    # Loader=yaml.SafeLoader
    # Loader=yaml.FullLoader
    # Loader=yaml.UnsafeLoader
    with open(os.path.join(original_path, path_yaml), 'r') as f:
        content = f.read()
    return yaml.load(content, Loader=yaml.SafeLoader)


def extract_yaml(yaml_data, dict_path):
    keys_list = [elt if re.search("[^0-9]", elt) else int(elt) for elt in dict_path.split('/')]
    return functools.reduce(lambda x, y: x[y], keys_list, yaml_data)


def show(yaml_data, dict_path=None):
    key = ""
    if dict_path:
        key = dict_path.split('/')[-1]
        dict_yaml = extract_yaml(yaml_data, dict_path)
        res = ' ' * 4 + yaml.dump(dict_yaml).replace('\n','\n    ').rstrip()
        res = "\n~~~~~~~~ {{.yaml}}\n{}:\n{}\n~~~~~~~~".format(key, res)
    else:
        dict_yaml = yaml_data
        res = "\n~~~~~~~~ {{.yaml}}\n{}\n~~~~~~~~".format(yaml.dump(dict_yaml))
    return res


def api(params):
    # wd = params["working_directory"]
    orig_path = params["original_path"]
    return {"load": functools.partial(load_yaml, original_path=orig_path),
            "show": show}

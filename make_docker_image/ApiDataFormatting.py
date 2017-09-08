# -*- coding: utf-8 -*-
import yaml
import functools
import re
import os


def load_yaml(path_yaml, original_path):
    with open(os.path.join(original_path, path_yaml), 'r') as f:
        content = f.read()
    return yaml.load(content)


def expect(condition, description):
   if (condition):
     return "\n<span style=\"font-family:monospace;color:white;background:green\">{} (true)</span>".format(description)
   else:
     return "\n<span style=\"font-family:monospace;color:white;background:red\">!!! {} (false)</span>".format(description)


def extract_yaml(yaml_data, dict_path):

    keys_list = [elt if re.search("[^0-9]", elt) else int(elt) for elt in dict_path.split('/')]
    return functools.reduce(lambda x, y: x[y], keys_list, yaml_data)


def is_int(string):
    try:
        int(string)
        return True
    except ValueError:
        return False

def show(yaml_data, dict_path=None):
    key = ""
    if dict_path:
        key = dict_path.split('/')[-1]
        if is_int(key):
            dict_yaml = [extract_yaml(yaml_data, dict_path)]
        else:
            dict_yaml = {key: extract_yaml(yaml_data, dict_path)}
    else:
        dict_yaml = yaml_data
    return "\n\n~~~~~~~~ {{.yaml}}\n {} \n~~~~~~~~".format(yaml.dump(dict_yaml))


def api(params):
    # wd = params["working_directory"]
    orig_path = params["original_path"]
    return { "load": functools.partial(load_yaml, original_path=orig_path)
           , "show": show
           , "expect": expect
            }

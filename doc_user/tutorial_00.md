# Tutoriels

```python echo=False, results = 'hidden'
import sys
import os

print(os.getcwd())
print(os.listdir(os.getcwd()))
print(sys.path)
print(sys.path.append(os.path.join(os.getcwd())))
from report.ApiJinjaCommands import execCmd
from report.ApiDataFormatting import load_yaml as fun_load_yaml
from report.ApiDataFormatting import show as show_yaml
from report.ApiDataJinja import csv
from report.ApiGraphJinja import prepare_plot_data


def exec(command_line, working_directory='tuto_execution', verbose=False):
    print(execCmd(command_line, working_directory=working_directory, verbose=verbose))

def load_yaml(filename):
    return fun_load_yaml(os.path.join('/usr','demos',filename),'r')


def print_yaml(yaml_data, dict_path=None):
    print(show_yaml(yaml_data, dict_path).strip())


def print_yaml_file(filename):
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}')
    print(open(os.path.join('/usr','demos',filename),'r').read())
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
```


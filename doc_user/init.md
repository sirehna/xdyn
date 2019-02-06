```python echo=False, results='hidden', name='load-prerequisite-python-modules'
import sys
import os

import warnings
import matplotlib

warnings.filterwarnings("ignore", category=matplotlib.MatplotlibDeprecationWarning)

print(os.getcwd())
print(os.listdir(os.getcwd()))
print(sys.path)
print(sys.path.append(os.path.join(os.getcwd())))

from report.ApiJinjaCommands import execCmd
from report.ApiDataFormatting import load_yaml as fun_load_yaml
from report.ApiDataFormatting import show as show_yaml
from report.ApiDataJinja import csv as fun_load_csv
from report.ApiGraphJinja import prepare_plot_data
from report.ApiGraphJinja import cartesian_graph
from report.ApiGraphJinja import create_layout as fun_create_layout


# All files for tutorials have been copied to `tuto_execution`
working_directory = os.path.join(os.getcwd(), 'tuto_execution')


def execCommand(command_line, working_directory=working_directory, verbose=False, echo_output=False):
    print(execCmd(command_line, working_directory=working_directory, verbose=verbose, echo_output=echo_output))


def load_yaml(filename):
    return fun_load_yaml(os.path.join(working_directory, filename), 'r')


def print_yaml(yaml_data, dict_path=None):
    print(show_yaml(yaml_data, dict_path).strip())


def print_yaml_file(filename):
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ {.yaml}')
    print(open(os.path.join(working_directory, filename), 'r').read())
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')


def csv(filename):
    return fun_load_csv(os.path.join(working_directory, filename), 'r')


def create_layout(graph, title=''):
    print(' ')
    print(fun_create_layout(graphs=[(graph, (0, 0))], title=title))
    print(' ')
```

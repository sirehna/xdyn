# -*- coding: utf-8 -*-
import subprocess
import functools


def execCmd(command_line, working_directory, echo_output=False, verbose=False):
    if verbose:
        print("Executing: " + command_line)
        print("in directory: " + working_directory)
    process = subprocess.Popen(command_line, cwd=working_directory, shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output, err = process.communicate()
    output = output.decode()
    err = err.decode()
    result = "\n~~~~~~~~~~~~~{{.bash}}\n{}\n~~~~~~~~~~~~~".format(command_line)
    if echo_output:
        result += '\n    ' + '\n    '.join(output.split('\n'))
    return result


def api(params):
    wd = params["working_directory"]
    verbose = params["verbose"]
    return {"exec": functools.partial(execCmd, working_directory=wd, verbose=verbose)}

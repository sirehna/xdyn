# -*- coding: utf-8 -*-
import subprocess
import functools


def execCmd(command_line, working_directory, verbose):
    if verbose:
        print("Executing: " + command_line)
        print("in directory: " + working_directory)
    process = subprocess.Popen(command_line, cwd=working_directory, shell=True)
    process.wait()
    return "\n~~~~~~~~~~~~~{{.bash}}\n{}\n~~~~~~~~~~~~~".format(command_line)


def api(params):
    wd = params["working_directory"]
    verbose = params["verbose"]
    return {"exec": functools.partial(execCmd, working_directory=wd, verbose=verbose)}

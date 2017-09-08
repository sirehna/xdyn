# -*- coding: utf-8 -*-
import subprocess
import functools


def execCmd(command_line, working_directory, verbose):
    if verbose:
        print("Executing: " + command_line)
        print("in directory: " + working_directory)
    output = subprocess.Popen(command_line, cwd=working_directory, shell=True, stdout=subprocess.PIPE ).communicate()[0]
    #process = subprocess.Popen(command_line, cwd=working_directory, shell=True)
    #process.wait()
    if (output):
        return "\n~~~~~~~~~~~~~{{.bash}}\n{}\n~~~~~~~~~~~~~\n\n~~~~~~~\n{}\n~~~~~~~\n".format(command_line,output.decode("utf-8"))
    return "\n~~~~~~~~~~~~~{{.bash}}\n{}\n~~~~~~~~~~~~~".format(command_line)

def api(params):
    wd = params["working_directory"]
    verbose = params["verbose"]
    return {"exec": functools.partial(execCmd, working_directory=wd, verbose=verbose)}

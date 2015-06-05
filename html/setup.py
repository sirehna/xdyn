#!/usr/bin/env python

import sys
from cx_Freeze import setup, Executable

if sys.version_info.major<3:
    sys.exit("Python server should be run with at least Python 3.4")

if sys.version_info.minor<4:
    sys.exit("Python server should be run with at least Python 3.4")

setup(
    name = "SimulatorServer",
    version = "0",
    description = "This program creates a server to run simulator",
    executables = [Executable("server.py")],
)

#!/bin/sh
xdyn anthineas_amortissementRoulis.yml controlled_forces_commands.yml --dt 0.2 --tend 200 -o extinctionRoulis.h5
xdyn anthineas_amortissementTangage.yml controlled_forces_commands.yml --dt 0.2 --tend 40 -o extinctionTangage.h5

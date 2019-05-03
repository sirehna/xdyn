#!/bin/sh
xdyn test_ship_amortissementRoulis.yml controlled_forces_commands.yml --dt 0.2 --tend 200 -o extinctionRoulis.h5
xdyn test_ship_amortissementTangage.yml controlled_forces_commands.yml --dt 0.2 --tend 40 -o extinctionTangage.h5

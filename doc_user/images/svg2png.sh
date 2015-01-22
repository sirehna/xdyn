#!/bin/sh
name=`basename $1 .svg`
inkscape -z -e ${name}.png -w 1024 -h 1024 ${name}.svg

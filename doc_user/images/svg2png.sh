#!/bin/sh
name=`basename $1 .svg`
inkscape -z -e ${name}.png ${name}.svg -d 75

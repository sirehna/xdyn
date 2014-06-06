#!/bin/sh
if [ $# -eq 1 ]
then
    f=$1
else
    f=user_guide_fr
fi
pandoc ${f}.md -s --highlight-style pygments -o ${f}.html

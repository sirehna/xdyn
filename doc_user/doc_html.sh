#!/bin/sh
if [ $# -eq 1 ]
then
    f=$1
else
    f=user_guide_fr
fi
pandoc -s --mathml ${f}.md --highlight-style pygments -o ${f}.html -c stylesheet.css

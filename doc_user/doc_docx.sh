#!/bin/sh
if [ $# -eq 1 ]
then
    f=$1
else
    f=user_guide_fr
fi
sed 's/svg/png/g' ${f}.md | pandoc -s --latexmathml --highlight-style pygments -o ${f}.docx

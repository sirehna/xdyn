#!/bin/sh
if [ $# -eq 1 ]
then
    f=$1
else
    f=user_guide_fr
fi
pandoc -s --highlight-style pygments -o ${f}Ori.tex ${f}.md
sed 's/svg/png/g' ${f}Ori.tex > ${f}.tex
pdflatex ${f}.tex
rm ${f}.tex ${f}Ori.tex ${f}.out ${f}.aux ${f}.log
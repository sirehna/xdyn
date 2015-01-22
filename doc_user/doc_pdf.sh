#!/bin/sh
cat introduction.md interfaces.md solver.md reperes_et_conventions.md modeles_environnementaux.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.md
sed -i 's/svg/png/g' concatenated_doc.md
pandoc -s --highlight-style pygments -o concatenated_doc.tex concatenated_doc.md
pdflatex concatenated_doc.tex
mv concatenated_doc.pdf simulator_doc.pdf
rm *.tex
rm concatenated_doc.md

#!/bin/sh
cat introduction.md interfaces.md solver.md reperes_et_conventions.md modeles_environnementaux.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.md
cat concatenated_doc.md | sed -e 's/svg/png/g' > concatenated_doc2.md
pandoc -s --highlight-style pygments -o concatenated_doc.tex concatenated_doc2.md
pdflatex concatenated_doc.tex
mv concatenated_doc.pdf doc.pdf
rm *.tex
rm concatenated_doc.md

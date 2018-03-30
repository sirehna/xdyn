#!/bin/sh
cat introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.pmd
cd images
python3 spectrum.py
cd ..
pweave -f markdown -k python3 --cache-directory=cache concatenated_doc.pmd -o concatenated_doc.md
pandoc --title-prefix=XDYN --standalone --self-contained -t html \
       --toc --toc-depth=3 --mathml --highlight-style pygments \
       -f markdown -V lang=fr concatenated_doc.md --css stylesheet.css \
       -o doc.html
rm concatenated_doc.pmd concatenated_doc.md

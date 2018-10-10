#!/bin/sh
# Script to be run from docker container to generate doc with tutorials
cat metadata.yaml introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.pmd
cd images
python3 spectrum.py
cd ..
# Copy tuto directory to allow write operations for docker run
rm -rf tuto_execution
cp -rf /usr/demos/. tuto_execution
pweave -f markdown -k python3 --cache-directory=cache concatenated_doc.pmd -o concatenated_doc.md
python3 pandoc_layout.py concatenated_doc.md -o concatenated_doc_pandoc.md
pandoc --title-prefix=XDYN \
       -t html \
       --standalone --self-contained \
       --toc --toc-depth=3 \
       --mathml \
       --highlight-style pygments \
       --filter pandoc-eqnos \
       --filter pandoc-fignos \
       --number-sections \
       -f markdown -V lang=fr \
       --css stylesheet.css \
       concatenated_doc_pandoc.md \
       -o doc.html
rm concatenated_doc.pmd concatenated_doc.md concatenated_doc_pandoc.md
rm -rf tuto_execution

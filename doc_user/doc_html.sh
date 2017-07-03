#!/bin/sh

#!/bin/sh
cat introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.md
pandoc -s --toc --mathml -f markdown concatenated_doc.md -t html --highlight-style pygments -o doc.html -c stylesheet.css
rm -f concatenated_doc*.md


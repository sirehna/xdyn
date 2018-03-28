#!/bin/sh
if [ $# -eq 1 ]
then
    f=$1
else
    f=user_guide_fr
fi
cat introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.md
cat concatenated_doc.md | sed -e 's/svg/png/g' > concatenated_doc2.md
pandoc -s --latexmathml --highlight-style pygments -t docx -o ${f}.docx concatenated_doc2.md
rm -f concatenated_doc.md concatenated_doc2.md

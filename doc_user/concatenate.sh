#!/bin/bash
rm -f concatenated_doc.pmd
for f in metadata.yaml init.md introduction.md interfaces_utilisateur.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts_commandes_et_non_commandes.md tutorial_00.md tutorial_01.md tutorial_02.md tutorial_03.md tutorial_06.md ; do (cat $f; echo "")  >> concatenated_doc.pmd ; done

.PHONY: all clean build images

all: build doc.html

concatenated_doc.pmd: \
    metadata.yaml \
    init.md introduction.md \
    interfaces_utilisateur.md \
    reperes_et_conventions.md \
    modeles_environnementaux.md \
    calcul_de_GM.md \
    courbes_de_GZ.md \
    solver.md diffraction_radiation.md \
    modeles_efforts_commandes_et_non_commandes.md \
    tutorial_00.md \
    tutorial_01.md \
    tutorial_02.md \
    tutorial_03.md \
    tutorial_06.md \
    tutorial_09.md \
    tutorial_10.md
	rm -f concatenated_doc.pmd
	for f in $^ ; do (cat $$f; echo "")  >> concatenated_doc.pmd ; done

xdyn-pweave := docker run -t \
               -v $(shell pwd):/work \
               -w /work \
               -u $(shell id -u):$(shell id -g) \
               --entrypoint /usr/bin/xdyn-weave \
               xweave
xdyn-pandoc := docker run -t \
               -v $(shell pwd):/work \
               -w /work \
               -u $(shell id -u):$(shell id -g) \
               --entrypoint /usr/bin/pandoc \
               xweave
xdyn-python3 := docker run -t \
               -v $(shell pwd):/work \
               -w /work \
               -u $(shell id -u):$(shell id -g) \
               --entrypoint /usr/local/bin/python3 \
               xweave

images: images/spectrum.py
	cd images && \
	docker run --rm -t \
	    -u $(shell id -u):$(shell id -g) \
	    -v $(shell pwd)/images:/work_images:rw \
	    -w /work_images \
	    --entrypoint /usr/local/bin/python3 \
	    xweave \
	    spectrum.py && \
	cd ..

build:
	docker build --tag xweave .

concatenated_doc.md: concatenated_doc.pmd
	$(xdyn-pweave) -f markdown -k python3 concatenated_doc.pmd -o concatenated_doc.md

concatenated_doc_pandoc.md: concatenated_doc.md pandoc_layout.py
	$(xdyn-python3) pandoc_layout.py concatenated_doc.md -o concatenated_doc_pandoc.md

doc.html: concatenated_doc_pandoc.md images
	$(xdyn-pandoc) --title-prefix=XDYN \
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

clean:
	rm -f \
	      concatenated_doc.pmd \
	      concatenated_doc.md \
	      concatenated_doc_pandoc.md  \
	      doc.html \
	      pict*.svg
	rm -rf tuto_execution

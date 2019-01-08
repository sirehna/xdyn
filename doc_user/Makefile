.PHONY: all clean build images

all: doc.html

concatenated_doc.pmd: metadata.yaml introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md
	cat metadata.yaml introduction.md interfaces.md reperes_et_conventions.md modeles_environnementaux.md calcul_de_GM.md courbes_de_GZ.md solver.md diffraction_radiation.md modeles_efforts.md tutorial_*.md > concatenated_doc.pmd

xdyn-pweave := docker run -t -v $(shell pwd):/work -w /work -u $(shell id -u):$(shell id -g) xdyn-pweave
xdyn-pandoc := docker run -t -v $(shell pwd):/work -w /work -u $(shell id -u):$(shell id -g) xdyn-pandoc
xdyn-python3 := docker run -t -v $(shell pwd):/work -w /work -u $(shell id -u):$(shell id -g) xdyn-python3

images:
	cd images && docker run -t -v $(shell pwd)/images:/work -w /work -u $(shell id -u):$(shell id -g) xdyn-python3 spectrum.py && cd ..

build:
	docker build --build-arg NAME=/usr/local/bin/pweave . --tag xdyn-pweave
	docker build --build-arg NAME=/usr/bin/pandoc . --tag xdyn-pandoc
	docker build --build-arg NAME=/usr/local/bin/python3 . --tag xdyn-python3


concatenated_doc.md: concatenated_doc.pmd build
	$(xdyn-pweave) -f markdown -k python3 concatenated_doc.pmd -o concatenated_doc.md

concatenated_doc_pandoc.md: concatenated_doc.md
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
	rm -f concatenated_doc.pmd concatenated_doc.md concatenated_doc_pandoc.md doc.html

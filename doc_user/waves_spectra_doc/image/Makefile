IMAGES = \
    spectrumBretschneider\
    spectrumGaussian\
    spectrumITTC\
    spectrumJonswap\
    spectrumOchi\
    spectrumOneraTILV10\
    spectrumPiersonMoskowitz\
    spectrumVignatBovis\
    spectrumComparison1\
    spectrumComparison2\
    Monochromatique\
    Bichromatique

IMAGES_SVG:=$(addsuffix .svg,$(IMAGES))
IMAGES_EPS:=$(addsuffix .eps,$(IMAGES))
IMAGES_PNG:=$(addsuffix .png,$(IMAGES))

ALL_IMAGES:=$(IMAGES_SVG) $(IMAGES_EPS) $(IMAGES_PNG)

all:$(ALL_IMAGES)
help:
	@echo "This Makefile is used to generate the images of the different wave spectra"
	@echo "... $(MAKE)"
	@echo "... $(MAKE) help"
	@echo "... $(MAKE) clean"

PYTHON:=python3

%.svg:spectrum.py
	$(PYTHON) $< -i $@

%.eps:spectrum.py
	$(PYTHON) $< -i $@

%.png:spectrum.py
	$(PYTHON) $< -i $@

clean:
	rm -f $(ALL_IMAGES)
.PHONY:clean

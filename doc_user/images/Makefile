# This makefile generates for each svg image the corresponding png image

PNGS := $(patsubst %.svg,%.png,$(wildcard *.svg))

all : $(PNGS) spectrumBretschneider.svg spectrumGaussian.svg spectrumITTC.svg spectrumJonswap.svg spectrumOchi.svg spectrumOneraTILV10.svg spectrumPiersonMoskowitz.svg spectrumVignatBovis.svg spectrumComparison1.svg spectrumComparison2.svg Monochromatique.svg Bichromatique.svg

spectrumBretschneider.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumGaussian.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumITTC.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumJonswap.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumOchi.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumOneraTILV10.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumPiersonMoskowitz.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumVignatBovis.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumComparison1.svg: spectrum.py
	python3 spectrum.py -i $<

spectrumComparison2.svg: spectrum.py
	python3 spectrum.py -i $<

Monochromatique.svg: spectrum.py
	python3 spectrum.py -i $<

Bichromatique.svg: spectrum.py
	python3 spectrum.py -i $<

%.png: %.svg
	./svg2png.sh $<

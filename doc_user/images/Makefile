# This makefile generates for each svg image the corresponding png image

PNGS := $(patsubst %.svg,%.png,$(wildcard *.svg))

SPECTRA := spectrumBretschneider.svg spectrumGaussian.svg spectrumITTC.svg spectrumJonswap.svg spectrumOchi.svg spectrumOneraTILV10.svg spectrumPiersonMoskowitz.svg spectrumVignatBovis.svg spectrumComparison1.svg spectrumComparison2.svg waveMonochromatique.svg waveBichromatique.svg

SPECTRA_PNGS := $(patsubst %.svg,%.png,$(SPECTRA))

all : $(PNGS) $(SPECTRA)
#all : $(SPECTRA)

spectrumBretschneider.svg: spectrum.py
	python3 spectrum.py

%.png: %.svg
	./svg2png.sh $<

clean:
	rm -rf $(SPECTRA) $(PNGS) $(SPECTRA_PNGS)

# This makefile generates for each svg image the corresponding png image

PNGS := $(patsubst %.svg,%.png,$(wildcard *.svg))

all : $(PNGS)

%.png: %.svg
	./svg2png.sh $<

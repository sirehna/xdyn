SUBDIRSDOC = image
.PHONY: $(SUBDIRSDOC)

all:doc

doc:Doxyfile spectrum.txt subdirsDoc
	( cat Doxyfile ; echo "PROJECT_NUMBER=\"Version 0.`svnversion ..`\"" ) | doxygen -
	# doxygen

subdirsDoc:
	for dir in $(SUBDIRSDOC); do \
		$(MAKE) -C $$dir; \
	done
.PHONY: subdirsDoc
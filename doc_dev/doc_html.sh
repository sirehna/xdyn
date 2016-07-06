#!/bin/sh
pandoc -s --toc --mathml -f markdown doc_dev.md -t html --highlight-style pygments -o doc_dev.html -c stylesheet.css

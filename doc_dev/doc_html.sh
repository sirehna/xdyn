#!/bin/sh
pandoc -s --toc --mathml -f markdown livrable_doc_dev.md -t html --highlight-style pygments -o livrable_doc_dev.html -c stylesheet.css

SPEC=configuration-structure

DRAFT=riscv-$(SPEC)-draft
RELEASE=riscv-$(SPEC)-release

all:	draft

draft:	$(DRAFT).pdf

release:	$(RELEASE).pdf

%.pdf: %.adoc
	asciidoctor-pdf \
	-a toc \
	-a compress \
	-a pdf-style=docs-resources/themes/riscv-pdf.yml \
	-a pdf-fontsdir=docs-resources/fonts \
	-o $@ $<

test:
	./rvcs.py test examples/*.jer

pylint:
	pylint *.py

clean:
	rm -f $(DRAFT).pdf $(RELEASE).pdf

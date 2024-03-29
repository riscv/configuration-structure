SPEC=unified-discovery

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

clean:
	rm -f $(DRAFT).pdf $(RELEASE).pdf

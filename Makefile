SPEC=configuration-structure

DRAFT=riscv-$(SPEC)-draft
RELEASE=riscv-$(SPEC)-release

all:	draft

draft:	$(DRAFT).pdf

release:	$(RELEASE).pdf

%.pdf: %.adoc
	asciidoctor-pdf $<

test:
	./asn1tools/rvcs.py test examples/*.jer

pylint:
	pylint asn1tools/*.py

clean:
	rm -f $(DRAFT).pdf $(RELEASE).pdf

all:: subject.pdf

subject_content.tex: subject_content.rst
	pandoc -o subject_content.tex subject_content.rst

apidoc.tex useapi.tex: ../prologin2019.yml
	stechec2-generator apidoc ../prologin2019.yml .

subject.pdf:: subject.tex subject_content.tex useapi.tex apidoc.tex prologin2019.pdf
	xelatex subject.tex

clean:
	rm -f *.toc
	rm -f *.log
	rm -f *.out
	rm -f *.idx
	rm -f *.aux
	rm -f *~

distclean: clean
	rm -f apidoc.tex
	rm -f useapi.tex
	rm -f subject_content.tex
	rm -f subject.pdf

.PHONY: all subject.pdf

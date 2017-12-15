#!/bin/sh

#(
#
#for i in *.gp ; do
#	i=${i%.gp}
#	o=$i
#	c=common/common.gp
#	if [ "$1"  -o  ! -f $o.pdf  -o  $o.pdf -ot $i.gp -o  $o.pdf -ot $c ] ; then
#		echo $i >&2
#		gnuplot $i.gp || exit 1
#		epstopdf $o.eps || exit 1
#		rm $o.eps
#	fi || exit 1
#done || exit 1
#)
#
rm top.aux top.bbl 
pdflatex top 
bibtex top 
perl -i -p -e 's|^(\\begin{thebibliography}{.*})$|$1\n|' top.bbl 
pdflatex top 
pdflatex top 

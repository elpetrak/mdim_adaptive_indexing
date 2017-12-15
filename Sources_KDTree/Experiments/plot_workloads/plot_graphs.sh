#!/bin/bash
sourcespath=../../../Sources_KDTree
paperpath=../../../Paper_KDTree
currentpath=`pwd`

die() {
        echo -e "Fail:  $*" > /dev/stderr
        exit 1
}

plot_queries() {
	nl ${currentpath}/output/out_$1 > queries

	echo -e "set title \"$4\"" >plot_queries.plt
	echo -e "set xlabel \"Dimension 1\"" >>plot_queries.plt
	if [ $4 == "randomDrandomS" ]
	then echo -e "set ylabel \"Dimension 2\" offset 0.5" >>plot_queries.plt
	fi
	echo -e "set xrange [0:${2}]\nset yrange [0:${2}]" >>plot_queries.plt
	echo -e "unset xtics"  >>plot_queries.plt
	echo -e "unset ytics" >>plot_queries.plt
	echo -e "unset key" >>plot_queries.plt
	for (( q=1; q<=${3}; q++ ))
	do
		x=`awk NR==${q} queries | awk '{print $2}'`
		mx=`awk NR==${q} queries | awk '{print $3}'`
		y=`awk NR==${q} queries | awk '{print $4}'`
		my=`awk NR==${q} queries | awk '{print $5}'`	
	  	echo -e "set object $q rect front from $x,$y to ${mx},${my} fc rgb \"white\"" >>plot_queries.plt
	done

	echo -e "plot x lc rgb \"white\"" >>plot_queries.plt
	echo -e "set term postscript eps size 1.2,1.8 color blacktext \"Helvetica\" 12" >>plot_queries.plt
	echo -e "set output 'queries.eps'\n replot" >>plot_queries.plt

	gnuplot plot_queries.plt
	ps2pdf queries.eps
	rm *.eps 
	mv queries.pdf figures/queries_${4}.pdf
	rm queries
	rm plot_queries.plt
}

if [[ ! -d "${sourcespath}" ]] ; then
        die "${sourcespath} is not a valid path."
fi

if [[ ! -d "${currentpath}/output" ]] ; then
        die "${currentpath}/output is not a valid path."
fi

mkdir figures

if [[ ! -d "${currentpath}/figures" ]] ; then
        die "${currentpath}/figures is not a valid path."
fi

if [[ ! -d "${paperpath}/Figures/queries" ]] ; then
        die "${paperpath}/Figures/queries is not a valid path."
fi

for dimensions in 2
do
	for workload in randomDrandomS randomDsameS randomDrandomS randomDSBsameS skewedDsameS sequentialDsameS periodicalDsameS zoominDdifferentS 
	do
		for queries in 100
		do
			for selectivity in 0.001
			do
				for tuples in 10000
				do
					echo ${dimensions} ${workload} ${tuples} ${selectivity} ${queries}
					plot_queries ${dimensions}_${workload}_${tuples}_${selectivity}_${queries} ${tuples} ${queries} ${workload}
				done
			done
		done
	done
done

if [[ -d "${currentpath}/figures" ]] ; then
        rm ${paperpath}/Figures/queries/*
	mv figures/* ${paperpath}/Figures/queries/.
	rm -rf figures
fi

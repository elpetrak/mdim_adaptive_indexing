sourcespath=../../../Sources_KDTree
paperpath=../../../Paper_KDTree
currentpath=`pwd`

die() {
        echo -e "Fail:  $*" > /dev/stderr
        exit 1
}

plot_workloads() {
	echo -e "set title \" #dimensions=$1 \"" >plot_graph.plt
	echo -e "set xlabel \"Selectivity(%)\"" >>plot_graph.plt
	if [ $1 == "2" ]
	then echo -e "set ylabel \"Total Response Time (msec)\" offset 0.5" >>plot_graph.plt
	fi
	echo -e "set logscale y \n " >>plot_graph.plt
	echo -e "set yrange [100000:*]\n" >>plot_graph.plt
	echo -e "set style data lines" >>plot_graph.plt
	echo -e "set xtics (\"0.1\" 0.001, \"1\" 0.01, \"10\" 0.1, \"20\" 0.2, \"30\" 0.3, \"40\" 0.4, \"50\" 0.5, \"60\" 0.6, \"60\" 0.6, \"70\" 0.7, \"80\" 0.8, \"90\" 0.9) font \"Helvetica,10\"" >>plot_graph.plt
	echo -e "set key nobox font \"Helvetica,10\" at graph 0.99 , 0.45" >>plot_graph.plt
	echo -e "plot \"<awk '{x=\$2; print \$1,x}' $4\" with linespoints lt 1 lc rgb \"\#DC143C\" lw 2 pt 4 ps 1 title \"scan\",\\" >>plot_graph.plt
	echo -e "\"<awk '{x=\$3; print \$1,x}' $4\" with linespoints lt 4 lc rgb \"\#4169E1\" lw 2 pt 6 ps 1 title \"swcrack\",\\" >>plot_graph.plt
	echo -e "\"<awk '{x=\$4; print \$1,x}' $4\" with linespoints lt 4 lc rgb \"\#32CD32\" lw 2 pt 8 ps 1 title \"kdcrack\"" >>plot_graph.plt
	echo -e "set term postscript eps size 1.5,1.8  color blacktext \"Helvetica\" 12" >>plot_graph.plt
	echo -e "set output '${5}.eps'\n replot" >>plot_graph.plt
	gnuplot plot_graph.plt
	ps2pdf ${5}.eps
	mv *.pdf figures/.
	rm *.eps
	rm *.plt
}

if [[ ! -d "${sourcespath}" ]] ; then
        die "${sourcespath} is not a valid path."
fi

if [[ ! -d "${currentpath}/output" ]] ; then
        die "${currentpath}/output is not a valid path."
fi

mkdir figures
mkdir data

if [[ ! -d "${currentpath}/data" ]] ; then
        die "${currentpath}/data is not a valid path."
fi

if [[ ! -d "${currentpath}/figures" ]] ; then
        die "${currentpath}/data is not a valid path."
fi


#CREATE DATA FILES used in plots
for dimensions in 2 3 4 5 6
do
	for workload in randomDsameS 
	do
		for queries in 1000
		do
			for selectivity in 0.001 0.01 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 
			do
				for tuples in 1000000 
				do
					echo ${dimensions} ${workload} ${tuples} ${selectivity} ${queries}
					total_time_scan=`grep "Query" output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries} | awk '{x=x+$3; print x}' | tail -n1`
					total_time_crack=`grep "Query" output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries} | awk '{x=x+$4; print x}' | tail -n1`
					total_time_kdcrack=`grep "Query" output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries} | awk '{x=x+$5; print x}' | tail -n1`
					echo ${selectivity} ${total_time_scan} ${total_time_crack} ${total_time_kdcrack} >> data/data_${dimensions}_${workload}_${queries}_${tuples}
				done
			done
		done
	done
done

#PLOT DATA
for dimensions in 2 3 4 5 6
do
	for queries in 1000 
	do
		for tuples in 1000000 
		do
			plot_workloads ${dimensions} ${queries} ${tuples} data/data_${dimensions}_randomDsameS_${queries}_${tuples} ${dimensions}_${queries}_${tuples}
		done
	done	
done

if [[ -d "${currentpath}/data" ]] ; then
	rm -rf data
fi

if [[ -d "${currentpath}/figures" ]] ; then
	mv figures/* ${paperpath}/Figures/vary_selectivity/.
	rm -rf figures
fi

#!/bin/bash
sourcespath=../../../Sources_KDTree
currentpath=`pwd`

die() {
        echo -e "Fail:  $*" > /dev/stderr
        exit 1
}

if [[ ! -d "${sourcespath}" ]] ; then
        die "${sourcespath} is not a valid path."
fi

if [[ ! -d "${currentpath}/output" ]] ; then
        mkdir output || die "cannot create output directory in ${currentpath}."
fi

pushd ${sourcespath} || die "No Sources directory."
make clean
make
popd


for dimensions in 2 3 4 5 6
do
	for workload in randomDsameS randomDSBsameS skewedDsameS 
	do
		for queries in 1000
		do
			for selectivity in 0.001 #0.01 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
			do
				for tuples in 1000000
				do
					echo ${dimensions} ${workload} ${tuples} ${selectivity} ${queries}
					./${sourcespath}/bin/sideways ${workload} ${tuples} ${selectivity} ${queries} ${dimensions} > ${currentpath}/output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries}
				done
			done
		done
	done

	for workload in randomDrandomS sequentialDsameS periodicalDsameS zoominDdifferentS 
	do
		for queries in 1000
		do
			for selectivity in 0.001
			do
				for tuples in 1000000
				do
					echo ${dimensions} ${workload} ${tuples} ${selectivity} ${queries}
					./${sourcespath}/bin/sideways ${workload} ${tuples} ${selectivity} ${queries} ${dimensions} > ${currentpath}/output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries}
				done
			done
		done
	done	
done


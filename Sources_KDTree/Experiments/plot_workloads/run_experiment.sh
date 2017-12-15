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
					./${sourcespath}/bin/printqueries ${workload} ${tuples} ${selectivity} ${queries} ${dimensions} > ${currentpath}/output/out_${dimensions}_${workload}_${tuples}_${selectivity}_${queries}
				done
			done
		done
	done
done


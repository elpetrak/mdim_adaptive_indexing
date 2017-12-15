#!/bin/bash
sourcespath=../../../Sources_KDTree
paperpath=../../../Paper_KDTree
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

if [[ ! -d "${paperpath}" ]] ; then
        die "${paperpath} is not a valid path."
fi

pushd ${sourcespath} || die "No Sources directory."
make clean
make
popd


for dimensions in 2
do
	for workload in randomDrandomS randomDsameS randomDrandomS randomDSBsameS skewedDsameS sequentialDsameS periodicalDsameS zoominDdifferentS 
	do
		for queries in 20
		do
			for selectivity in 0.001
			do
				for tuples in 10000
				do
					echo ${dimensions} ${workload} ${tuples} ${selectivity} ${queries}
					./${sourcespath}/bin/printkdtree ${workload} ${tuples} ${selectivity} ${queries} ${dimensions} 
					dot -Tpdf treenodes.dot -o kdtree_${workload}.pdf
					mv kdtree_${workload}.pdf output/.
					rm treenodes.dot 
				done
			done
		done
	done
done

if [[ -d "${currentpath}/output" ]] ; then
        rm ${paperpath}/Figures/kdtree/*
        mv output/* ${paperpath}/Figures/kdtree/.
        rm output/*
fi


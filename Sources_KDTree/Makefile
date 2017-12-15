CC=gcc
CFLAGS=-std=gnu99 -Wall -O3 #-g
LDFLAGS=-lm #-lpapi
EXPVAR=-DPRINTQUERIES -DSCANFEW #-DMETRICS 
all: sideways printqueries printkdtree

sideways: outputdir
	$(CC) $(CFLAGS) -o bin/sideways Implementations/queries.c Implementations/avltree.c Implementations/kdtree.c Implementations/scan.c Implementations/sideways_cracking.c Implementations/main.c $(LDFLAGS)  

printqueries: outputdir
	$(CC) $(CFLAGS) -o bin/printqueries -DPRINTQUERIES  Implementations/queries.c Implementations/printqueries.c $(LDFLAGS)

printkdtree: outputdir
	$(CC) $(CFLAGS) -o bin/printkdtree Implementations/createdot.c Implementations/kdtree.c  Implementations/queries.c Implementations/printkdtree.c $(LDFLAGS)  

#add -DPRINTQUERIES in EXPVAR to print the query predicates
#add -DDEBUG in EXPVAR to enable debugging
#add -DSCANFEW in EXPVAR to scan only 10 queries instead of the entire workload
#add #-DMETRICS in EXPVAR to measure other metrics than time

outputdir:
	mkdir -v bin 2>/dev/null || true

clean:
	rm -rf bin

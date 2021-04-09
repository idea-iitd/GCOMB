
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 200,100

set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set xlabel "#Seeds"
set ylabel "#Snapshots (R)"
set ytics 0,50,300
set xtics 0,40,200

plot 'pmc_nethept_ic' using 1:2 title 'PMC' lw 11.5 ps 3.5 pt 14 lt 1 lc 1 with linespoint,\
	'sg_nethept_ic' using 1:2 title 'Static Greedy' lw 11.5 ps 3.5 pt 4 lt 1 lc 2 with linespoint

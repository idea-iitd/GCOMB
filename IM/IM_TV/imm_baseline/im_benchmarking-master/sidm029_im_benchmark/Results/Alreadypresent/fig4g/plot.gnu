
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 205,90

set key font "Helvetica,45,bold"
set xlabel font "Helvetica,48,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,38,bold"
set ytics font "Helvetica,38,bold"
set title font "Helvetica,42,bold"


set xlabel "#Seeds (k)"
set ylabel "#Snapshots (R)"
set ytics 0,50,300
set xtics 0,40,200

plot 'pmc_hepph_wc' using 1:2 title 'PMC' lw 12.5 ps 4.5 pt 14 lt 1 lc 1 with linespoint,\
'sg_hepph_wc' using 1:2 title 'Static Greedy' lw 12.5 ps 4.5 pt 4 lt 1 lc 2 with linespoint
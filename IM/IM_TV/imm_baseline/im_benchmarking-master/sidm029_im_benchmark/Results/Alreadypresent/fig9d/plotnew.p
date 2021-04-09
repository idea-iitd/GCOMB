
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key bottom

set key font "Helvetica,45,bold"
set xlabel font "Helvetica,48,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,38,bold"
set ytics font "Helvetica,38,bold"
set title font "Helvetica,42,bold"


set xlabel "#Seeds (k)"
set ylabel "Spread"
set xtics 0,40,200

plot 'imm.txt' using 1:2 title 'IMM' lw 7.5 ps 4.5 pt 14 lt 1 lc 1 with linespoint,\
	'celf1000.txt' using 1:2 title 'CELF, 1K' lw 7.5 ps 4.5 pt 4 lt 1 lc 2 with linespoint,\
	'celf10000.txt' using 1:2 title 'CELF, 10K' lw 7.5 ps 4.5 pt 8 lt 1 lc 4 with linespoint, \
	'celf20000.txt' using 1:2 title 'CELF, 20K' lw 7.5 ps 4.5 pt 6 lt 1 lc 3 with linespoint,\
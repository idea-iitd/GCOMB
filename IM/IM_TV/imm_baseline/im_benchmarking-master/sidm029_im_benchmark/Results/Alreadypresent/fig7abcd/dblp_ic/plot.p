
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key out horizontal top

set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set xlabel "Seeds"
set ylabel "Time"
set xrange [0:200]

set xtics 0,25,200
set logscale y
plot    'dblp_IC_0.05_time_imm.txt' using 1:2 title 'IMM' lw 5 ps 2.3 lt 1 lc 4 with linespoint, \
     'dblp_ic_200_time.txt' using 1:2 title 'PMC' lw 5 ps 2.3 lt 1 lc 5 with linespoint, \
	 'dblp_ic_1000_EASY' using 1:($2*1000) title 'EaSyIM' lw 5 ps 2.3 lt 1 lc 6 with linespoint
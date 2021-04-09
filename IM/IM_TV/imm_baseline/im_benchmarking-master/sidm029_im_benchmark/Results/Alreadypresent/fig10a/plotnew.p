
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1
set bmargin 4


set key top left

set key font "Helvetica,48,bold"
set xlabel font "Helvetica,48,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,42,bold"
set ytics font "Helvetica,42,bold"
set title font "Helvetica,42,bold"


set xlabel "#Seeds (k)"
set ylabel "Time (in min)"
#set xrange [1.05:0.1]
set xtics 0,40,200

plot 'ldag.txt' using 1:3 title 'LDAG' lw 7.5 ps 4.5 pt 6 lt 1 lc 1 with linespoint, \
    'simpath.txt' using 1:3 title 'SIMPATH' lw 7.5 ps 4.5 pt 10 lt 1 lc 2 with linespoint

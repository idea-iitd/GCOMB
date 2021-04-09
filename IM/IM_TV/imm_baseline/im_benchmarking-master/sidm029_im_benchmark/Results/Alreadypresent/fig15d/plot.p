
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 160,20
set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set xlabel "#Seeds"
set ylabel "#MC Simulations"
set xtics 0,40,200
set ytics 0,10,50


plot    'dblp_easyim_ic' using 1:2 title 'EaSyIM' lw 6.5 ps 3.5 pt 6 lt 1 lc 3 with linespoint
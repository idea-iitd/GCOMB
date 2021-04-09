
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 195,5000
set key font "Helvetica,45,bold"
set xlabel font "Helvetica,48,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,38,bold"
set ytics font "Helvetica,38,bold"
set title font "Helvetica,42,bold"

set xlabel "#Seeds (k)"
set ylabel "#MC Simulations (r)"
set xtics 0,40,200
set logscale y

plot 'easyim.txt' using 1:2 title 'EaSyIM' lw 7.5 ps 4.5 pt 6 lt 1 lc 3 with linespoint

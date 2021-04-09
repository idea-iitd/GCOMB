
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 125,200

set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"


set xlabel "#Simulations (r)"
set ylabel "Spread"
set xtics 0,4000,20000

plot '200.txt' using 1:($2+200):3 title 'NetHept (IC)' lw 7.5 ps 4.5 pt 14 lt 1 lc 1  with yerrorbars, \
'200.txt' using 1:($2+200) title '' lw 7.5 ps 4.5 pt 14 lt 1 lc 1  with linespoint

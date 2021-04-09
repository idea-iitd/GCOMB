
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1
set bmargin 4


set key bottom

set key font "Helvetica,45,bold"
set xlabel font "Helvetica,45,bold"
set ylabel font "Helvetica,45,bold"
set xtics font "Helvetica,38,bold"
set ytics font "Helvetica,38,bold"
set title font "Helvetica,42,bold"


set xlabel "#Seeds (k)"
set ylabel "Spread"
set xtics 0,50,200


plot 'incorrect_hepph_wc copy.txt' using 1:2  title 'Incorrect' lw 6.5 ps 3.5 pt 6 lt 1 lc 1 with linespoint, \
'corrected_hepph_wc.txt' using 1:2 title 'Corrected' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint
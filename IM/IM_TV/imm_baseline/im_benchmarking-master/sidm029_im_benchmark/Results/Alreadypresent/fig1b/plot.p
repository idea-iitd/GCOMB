
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 195, 150

set key spacing 1.2
set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set xlabel "Seeds (k)"
set ylabel "Time (s)"
set xrange [0:200]
set logscale y
set yrange [10:28000]

set xtics 0,25,200


plot 'imm.txt' using 1:2 title 'IMM' lw 5 ps 2.3 pt 14 lt 1 lc 1 with linespoint, \
     'easyim.txt' using 1:(100*$2) title 'EaSyIM' lw 5 ps 2.3 pt 6 lt 1 lc 2 with linespoint
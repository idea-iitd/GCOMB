
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key at 200,1.4
set key font "Helvetica,45,bold"
set xlabel font "Helvetica,48,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,38,bold"
set ytics font "Helvetica,38,bold"
set title font "Helvetica,42,bold"

set xlabel "#Seeds"
set ylabel "#Scoring Rounds"
set ytics 0,1,10
set xtics 0,40,200

plot 'imrank_youtube_wc' using 1:2 title 'l=1' lw 7.5 ps 4.5 pt 14 lt 1 lc 1 with linespoint, \
     'imrank2_youtube_wc' using 1:2 title 'l=2' lw 7.5 ps 4.5 pt 4 lt 1 lc 2 with linespoint
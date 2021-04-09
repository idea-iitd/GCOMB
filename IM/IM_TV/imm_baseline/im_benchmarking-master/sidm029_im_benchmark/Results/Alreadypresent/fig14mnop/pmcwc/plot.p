
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

set xlabel "#Snapshots (R)"
set ylabel "Spread"
set xrange [0:300]

set xtics 0,50,300

plot 'stat_hepph_wc_200_rother1' using 1:2 title 'k = 1' lw 5 ps 2.3 pt 14 lt 1 lc 1 with linespoint, \
     'stat_hepph_wc_200_rother50' using 1:2 title 'k = 50' lw 5 ps 2.3 lt 1 lc 6 with linespoint, \
     'stat_hepph_wc_200_rother100' using 1:2 title 'k = 100' lw 5 ps 2.3 lt 1 lc 8 with linespoint, \
     'stat_hepph_wc_200_rother150' using 1:2 title 'k = 150' lw 5 ps 2.3 lt 1 lc 10 with linespoint, \
'stat_hepph_wc_200_rother200' using 1:2 title 'k = 200' lw 5 ps 2.3 lt 1 lc 12 with linespoint
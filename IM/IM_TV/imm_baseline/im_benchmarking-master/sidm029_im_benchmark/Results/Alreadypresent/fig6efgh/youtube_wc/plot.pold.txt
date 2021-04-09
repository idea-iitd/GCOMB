
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
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

plot 'stat_IMM_youtube_WC_0.1.txt' using 1:2 title 'IMM' lw 5 ps 2.3 lt 1 lc 3 with linespoint, \
     'stat_TIM_youtube_WC_0.15.txt' using 1:2 title 'TIM' lw 5 ps 2.3 lt 1 lc 4 with linespoint, \
     'stat_youtube_wc_200_r250.txt' using 1:2 title 'PMC' lw 5 ps 2.3 lt 1 lc 5 with linespoint, \
	 'stat_youtube_IRIE_WC.txt' using 1:2 title 'IRIE' lw 5 ps 2.3 lt 1 lc 6 with linespoint, \
     'stat_youtube_imrank_num4_l1.txt' using 1:2 title 'imrank' lw 5 ps 2.3 lt 1 lc 7 with linespoint, \
     'stat_youtube_imrank_num4_l2.txt' using 1:2 title 'imrank l=2' lw 5 ps 2.3 lt 1 lc 8 with linespoint, \
     'stat_youtube_IRIE_WC.txt' using 1:2 title 'EaSyIM' lw 5 ps 2.3 lt 1 lc 8 with linespoint
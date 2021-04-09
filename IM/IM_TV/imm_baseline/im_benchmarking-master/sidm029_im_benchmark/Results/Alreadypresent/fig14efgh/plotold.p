
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set bmargin 7

#set key samplen 6
#set key spacing 15
#set key default
#set key at 2.5,1
#set key out horizontal top left
set size 4.8,1.3


set key out horizontal top

set key font "Helvetica,46,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,36,bold"
set ytics font "Helvetica,36,bold"
set title font "Helvetica,42,bold"



set multiplot layout 1,4
unset key
set size 1.1,1.2
set origin 0.05,0
set xlabel "#MC Simulations"
set ylabel "Spread"
set xrange [0:10000]

set xtics 0,2000,10000

set label "(a) CELF++ (WC)" at 5000,-920 center font "Helvetica,60,bold"


plot 'celf++wc/stat_200_celf++_other1' using 1:2 title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint,\
'celf++wc/stat_200_celf++_other50' using 1:2 title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint,\
'celf++wc/stat_200_celf++_other100' using 1:2 title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint,\
'celf++wc/stat_200_celf++_other150' using 1:2 title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint,\
'celf++wc/stat_200_celf++_other200' using 1:2 title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 1.5,1.26
set xlabel "#MC Simulations"
set ylabel "Spread"
set xrange [0:10000]

set xtics 0,2000,10000

set label "(b) CELF++ (LT)" at 5000,-1500 center font "Helvetica,60,bold"

plot 'lt/stat_200_celf++_other1' using 1:2 title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'lt/stat_200_celf++_other50' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'lt/stat_200_celf++_other100' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'lt/stat_200_celf++_other150' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'lt/stat_200_celf++_other200' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
#unset key
set key at screen 2.55,1.26
set size 1.1,1.2
set origin 2.45,0
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(c) TIM^+ (IC)" at 0.5,3800 center font "Helvetica,60,bold"


plot 'timic/stat_TIM_hepph_IC_other1' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'timic/stat_TIM_hepph_IC_other50' using 1:2 title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'timic/stat_TIM_hepph_IC_other100' using 1:2 title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'timic/stat_TIM_hepph_IC_other150' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'timic/stat_TIM_hepph_IC_other200' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label

set key at screen 3.6,1.26
set size 1.1,1.2
set origin 3.65,0
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(d) TIM^+ (WC)" at 0.5,-900 center font "Helvetica,60,bold"


plot 'timwc/stat_TIM_hepph_WC_other1' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'timwc/stat_TIM_hepph_WC_other50' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'timwc/stat_TIM_hepph_WC_other100' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'timwc/stat_TIM_hepph_WC_other150' using 1:2 title 'k=150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'timwc/stat_TIM_hepph_WC_other200' using 1:2 title 'k=200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint
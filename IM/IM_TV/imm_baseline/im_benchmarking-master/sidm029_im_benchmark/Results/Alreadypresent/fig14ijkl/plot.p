
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
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(i) TIM^+ (LT)" at 0.5,-1650 center font "Helvetica,60,bold"


plot 'timlt/stat_TIM_hepph_LT_other1' using 1:($2+1) title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint,\
'timlt/stat_TIM_hepph_LT_other50' using 1:($2+50) title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint,\
'timlt/stat_TIM_hepph_LT_other100' using 1:($2+100) title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint,\
'timlt/stat_TIM_hepph_LT_other150' using 1:($2+150) title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint,\
'timlt/stat_TIM_hepph_LT_other200' using 1:($2+200) title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 1.5,1.26
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(j) IMM (IC)" at 0.5,3730 center font "Helvetica,60,bold"

plot 'ic/stat_IMM_hepph_IC_other1' using 1:($2+1) title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'ic/stat_IMM_hepph_IC_other50' using 1:($2+50) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'ic/stat_IMM_hepph_IC_other100' using 1:($2+100) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'ic/stat_IMM_hepph_IC_other150' using 1:($2+150) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'ic/stat_IMM_hepph_IC_other200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
#unset key
set key at screen 2.55,1.26
set size 1.1,1.2
set origin 2.45,0
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(k) IMM (WC)" at 0.5,-940 center font "Helvetica,60,bold"


plot 'wc/stat_IMM_hepph_WC_other1' using 1:($2+1) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'wc/stat_IMM_hepph_WC_other50' using 1:($2+50) title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'wc/stat_IMM_hepph_WC_other100' using 1:($2+100) title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'wc/stat_IMM_hepph_wC_other150' using 1:($2+150) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'wc/stat_IMM_hepph_WC_other200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label

set key at screen 3.6,1.26
set size 1.1,1.2
set origin 3.65,0
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(l) IMM (LT)" at 0.5,-1650 center font "Helvetica,60,bold"


plot 'lt/stat_IMM_hepph_LT_other1' using 1:($2+1) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'lt/stat_IMM_hepph_LT_other50' using 1:($2+50) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'lt/stat_IMM_hepph_LT_other100' using 1:($2+100) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'lt/stat_IMM_hepph_LT_other150' using 1:($2+150) title 'k=150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'lt/stat_IMM_hepph_LT_other200' using 1:($2+200) title 'k=200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

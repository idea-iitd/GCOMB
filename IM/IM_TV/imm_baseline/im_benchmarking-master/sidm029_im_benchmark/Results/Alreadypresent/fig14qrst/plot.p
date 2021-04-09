
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
#set xrange [0:10000]

set xtics 0,200,1000

set label "(q) EaSyIM (IC)" at 500,3740 center font "Helvetica,60,bold"



plot 'easyic/stat_st_hepph_ic_other1' using 1:($2+1) title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'easyic/stat_st_hepph_ic_other50' using 1:($2+50) title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'easyic/stat_st_hepph_ic_other100' using 1:($2+100) title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'easyic/stat_st_hepph_ic_other150' using 1:($2+150) title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'easyic/stat_st_hepph_ic_other200' using 1:($2+200) title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 1.5,1.26
set xlabel "#MC Simulations"
set ylabel "Spread"
#set xrange [0:10000]

set xtics 0,200,1000

set label "(r) EaSyIM (WC)" at 500,-980 center font "Helvetica,60,bold"


plot 'wc/stat_st_hepph_wc_other1' using 1:($2+1) title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'wc/stat_st_hepph_wc_other50' using 1:($2+50) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'wc/stat_st_hepph_wc_other100' using 1:($2+100) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'wc/stat_st_hepph_wc_other150' using 1:($2+150) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'wc/stat_st_hepph_wc_other200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
#unset key
set key at screen 2.55,1.26
set size 1.1,1.2
set origin 2.45,0
set xlabel "#MC Simulations"
set ylabel "Spread"
set xrange [0:1000]

set xtics 0,200,1000

set label "(s) EaSyIM (LT)" at 500,-1430 center font "Helvetica,60,bold"


plot 'lt/stat_st_hepph_lt_other1' using 1:($2+1) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'lt/stat_st_hepph_lt_other50' using 1:($2+50) title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'lt/stat_st_hepph_lt_other100' using 1:($2+100) title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'lt/stat_st_hepph_lt_other150' using 1:($2+150) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'lt/stat_st_hepph_lt_other200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label

set key at screen 3.6,1.26
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Scoring Rounds"
set ylabel "Spread"
set xrange [1:10]

set xtics 0,2,10

set label "(t) IMRank (IC) (l=1)" at 5,3790 center font "Helvetica,60,bold"


plot 'imrankic/stat_hepph_imrank_numother1' using 1:($2+1) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'imrankic/stat_hepph_imrank_numother50' using 1:($2+50) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'imrankic/stat_hepph_imrank_numother100' using 1:($2+100) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'imrankic/stat_hepph_imrank_numother150' using 1:($2+150) title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'imrankic/stat_hepph_imrank_numother200' using 1:($2+200) title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

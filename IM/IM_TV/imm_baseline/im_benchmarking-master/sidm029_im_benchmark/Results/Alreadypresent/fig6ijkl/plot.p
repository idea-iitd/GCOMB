
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set bmargin 7

#set key samplen 6
#set key spacing 15
#set key default
#set key at 2.5,1
#set key out horizontal top left
set size 4.8,1.33


set key out horizontal top

set key font "Helvetica,46,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,36,bold"
set ytics font "Helvetica,36,bold"
set title font "Helvetica,42,bold"



set multiplot layout 1,4
set key at screen 1.7,1.29
set size 1.1,1.2
set origin 0.05,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(i) Nethept (LT)" at 100,-1000 center font "Helvetica,60,bold"


plot 'netheptlt/stat_200_celf_10000.txt' using 1:2 title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'netheptlt/stat_200_celf++_10000.txt' using 1:2 title 'CELF++' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'netheptlt/stat_IMM_nethept_LT_0.1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'netheptlt/stat_TIM_nethept_LT_0.35.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4  with linespoint, \
'netheptlt/stat_200_ldag_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'netheptlt/stat_200_simpath_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 44 lt 1 lc '#9ACD32' with linespoint, \
'netheptlt/stat_st_nethept_lt_25.txt' using 1:2 title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint
#'netheptic/stat_nethept_imrank_num10_l1.txt' using 1:2 title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint, \

unset label

set size 1.1,1.2
set origin 1.25,0
set key at screen 2.65,1.29
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(j) HepPh (LT)" at 100,-1800 center font "Helvetica,60,bold"

plot 'hepphlt/stat_200_celf_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepphlt/stat_200_celf++_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepphlt/stat_IMM_hepph_LT_0.1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepphlt/stat_TIM_hepph_LT_0.35.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepphlt/stat_200_ldag_10000.txt' using 1:2 title 'LDAG' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'hepphlt/stat_200_simpath_10000.txt' using 1:2 title 'SIMPATH' lw 6.5 ps 3.5 pt 44 lt 1 lc '#9ACD32' with linespoint, \
'hepphlt/stat_st_hepph_lt_25.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint
#'hepphlt/stat_hepph_imrank_num10_l1.txt' using 1:2 title '' lw 6.5 ps 3.5 lt 1 lc 8 with linespoint, \

unset label

#unset key
set key at screen 3.45,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(k) DBLP (LT)" at 100,-6300 center font "Helvetica,60,bold"


plot 'dblplt/stat_IMM_dblp_LT_0.1.txt' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
    'dblplt/stat_TIM_dblp_LT_0.35.txt' using 1:2 title 'TIM^+' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
    'dblplt/stat_200_ldag_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
    'dblplt/stat_st_dblp_lt_25.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9  with linespoint
#'dblpic/stat_dblp_imrank_num10_l1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 8 with linespoint, \

unset label

set key at screen 4,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(l) Youtube (LT)" at 100,-115000 center font "Helvetica,60,bold"


plot    'youtubelt/stat_IMM_youtube_LT_0.1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'youtubelt/stat_TIM_youtube_LT_0.35.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4  with linespoint, \
    'youtubelt/stat_200_ldag_10000.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
    'youtubelt/stat_st_youtube_lt_25.txt' using 1:2 title 'EaSyIM' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint
#'youtubeic/stat_youtube_imrank_num10_l1.txt' using 1:2 title 'IMRank (l=1)' lw 6.5 ps 3.5 pt 6 lt 1 lc 8 with linespoint, \
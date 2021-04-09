
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



set multiplot layout 1,5
set key at screen 1.25,1.29
set size 1.1,1.2
set origin 0.05,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(a) Nethept (IC)" at 100,375 center font "Helvetica,60,bold"


plot 'netheptic/stat_200_celf_10000.txt' using 1:2 title 'CELF' lw 10 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'netheptic/stat_200_celf++_7500.txt' using 1:2 title 'CELF++' lw 10 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'netheptic/stat_IMM_nethept_IC_0.05.txt' using 1:2 title '' lw 10 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'netheptic/stat_TIM_nethept_IC_0.05.txt' using 1:2 title '' lw 10 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'netheptic/stat_nethept_ic_200_r200.txt' using 1:2 title '' lw 10 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'netheptic/stat_nethept_ic_200_r250.txt' using 1:2 title '' lw 10 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'netheptic/stat_nethept_IRIE_IC.txt' using 1:2 title '' lw 10 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'netheptic/stat_nethept_imrank_num10_l2.txt' using 1:2 title '' lw 10 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'netheptic/stat_st_nethept_ic_50.txt' using 1:2 title '' lw 10 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'netheptic/stat_nethept_imrank_num10_l1.txt' using 1:2 title '' lw 10 ps 3.5 lt 1 lc 7 with linespoint

unset label

set size 1.1,1.2
set origin 1.25,0
set key at screen 2.05,1.29
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(b) HepPh (IC)" at 100,3790 center font "Helvetica,60,bold"

plot 'hepphic/stat_200_celf_10000.txt' using 1:2 title '' lw 10 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepphic/stat_200_celf++_7500.txt' using 1:2 title '' lw 10 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepphic/stat_IMM_hepph_IC_0.05.txt' using 1:2 title 'IMM' lw 10 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepphic/stat_TIM_hepph_IC_0.05.txt' using 1:2 title 'TIM^+' lw 10 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepphic/stat_hepph_ic_200_r200.txt' using 1:2 title '' lw 10 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'hepphic/stat_hepph_ic_200_r250.txt' using 1:2 title '' lw 10 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'hepphic/stat_hepph_IRIE_IC.txt' using 1:2 title '' lw 10 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'hepphic/stat_hepph_imrank_num10_l2.txt' using 1:2 title '' lw 10 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'hepphic/stat_st_hepph_ic_50.txt' using 1:2 title '' lw 10 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'hepphic/stat_hepph_imrank_num10_l1.txt' using 1:2 title '' lw 10 ps 3.5 lt 1 lc 7 with linespoint
unset label
#unset key
set key at screen 2.95,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

set label "(c) DBLP (IC)" at 100,47175 center font "Helvetica,60,bold"


plot    'dblpic/stat_dblp_ic_200_r200.txt' using 1:2 title 'PMC' lw 10 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'dblpic/stat_dblp_ic_200_r250.txt' using 1:2 title '' lw 10 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'dblpic/stat_dblp_IRIE_IC.txt' using 1:2 title '' lw 10 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'dblpic/stat_dblp_imrank_num10_l2.txt' using 1:2 title '' lw 10 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'dblpic/stat_st_dblp_ic_50.txt' using 1:2 title '' lw 10 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'dblpic/stat_dblp_imrank_num10_l1.txt' using 1:2 title 'IMRank1' lw 10 ps 3.5 lt 1 lc 7 with linespoint
unset label

set key at screen 3.8,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(d) Youtube (IC)" at 100,169200 center font "Helvetica,60,bold"


plot    'youtubeic/stat_youtube_ic_200_r200.txt' using 1:2 title '' lw 10 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtubeic/stat_youtube_ic_200_r75.txt' using 1:2 title 'SG' lw 10 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtubeic/stat_youtube_IRIE_IC.txt' using 1:2 title '' lw 10 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtubeic/stat_youtube_imrank_num10_l2.txt' using 1:2 title 'IMRank2' lw 10 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtubeic/stat_st_youtube_ic_50.txt' using 1:2 title '' lw 10 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'youtubeic/stat_youtube_imrank_num10_l1.txt' using 1:2 title '' lw 10 ps 3.5 lt 1 lc 7 with linespoint



unset label
set key at screen 4.7,1.29
set size 1.1,1.2
set origin 4.85,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set logscale y


plot    'youtubeic/stat_youtube_ic_200_r200.txt' using 1:2 title '' lw 10 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtubeic/stat_youtube_ic_200_r75.txt' using 1:2 title '' lw 10 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtubeic/stat_youtube_IRIE_IC.txt' using 1:2 title 'IRIE' lw 10 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtubeic/stat_youtube_imrank_num10_l2.txt' using 1:2 title '' lw 10 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtubeic/stat_st_youtube_ic_50.txt' using 1:2 title 'EaSyIM' lw 10 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'youtubeic/stat_youtube_imrank_num10_l1.txt' using 1:2 title '' lw 10 ps 3.5 lt 1 lc 7 with linespoint

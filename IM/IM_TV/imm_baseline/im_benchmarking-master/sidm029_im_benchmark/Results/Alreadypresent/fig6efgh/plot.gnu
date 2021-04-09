
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
set xlabel "Seeds"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(e) Nethept (WC)" at 100,-800 center font "Helvetica,60,bold"



plot 'nethept_wc/stat_200_celf_10000.txt' using 1:2 title 'CELF' lw 11 ps 3.5 pt 8 lt 1 lc 'coral'  with linespoint, \
'nethept_wc/stat_200_celf++_7500.txt' using 1:2 title 'CELF++' lw 11 ps 3.5  pt 10 lt 1 lc 2 with linespoint, \
'nethept_wc/stat_IMM_nethept_WC_0.1.txt' using 1:2 title '' lw 11 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_wc/stat_TIM_nethept_WC_0.15.txt' using 1:2 title '' lw 11 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_wc/stat_nethept_wc_200_r250sg.txt' using 1:2 title '' lw 11 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_wc/stat_nethept_wc_200_r250.txt' using 1:2 title '' lw 11 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_wc/stat_nethept_IRIE_WC.txt' using 1:2 title '' lw 11 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_wc/stat_nethept_imrank_num10_l1.txt' using 1:2 title '' lw 11 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_wc/stat_nethept_imrank_num10_l2.txt' using 1:2 title '' lw 11 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_wc/stat_st_nethept_wc_50.txt' using 1:2 title '' lw 11 pt 5 ps 3.5 lt 1 lc 9 with linespoint

unset label
set size 1.1,1.2
set origin 1.25,0
set key at screen 2.05,1.29

set xlabel "Seeds"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(f) HepPh (WC)" at 100,-1000 center font "Helvetica,60,bold"

plot 'hepph_wc/stat_200_celf_10000.txt' using 1:2 title '' lw 11 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepph_wc/stat_200_celf++_7500.txt' using 1:2 title '' lw 11 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepph_wc/stat_IMM_hepph_WC_0.1.txt' using 1:2 title 'IMM' lw 11 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepph_wc/stat_TIM_hepph_WC_0.15.txt' using 1:2 title 'TIM^+' lw 11 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepph_wc/stat_hepph_wc_200_r250.txt' using 1:2 title '' lw 11 ps 3.5 pt 14 lt 1 lc 5  with linespoint, \
'hepph_wc/stat_hepph_wc_200_r250sg.txt' using 1:2 title '' lw 11 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'hepph_wc/stat_hepph_IRIE_WC.txt' using 1:2 title '' lw 11 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'hepph_wc/stat_hepph_imrank_num10_l1.txt' using 1:2 title '' lw 11 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'hepph_wc/stat_hepph_imrank_num10_l2.txt' using 1:2 title '' lw 11 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'hepph_wc/stat_st_hepph_wc_50.txt' using 1:2 title '' lw 11 ps 3.5 pt 5 lt 1 lc 9  with linespoint


unset label
set key at screen 2.95,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "Seeds"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(g) DBLP (WC)" at 100,-4500 center font "Helvetica,60,bold"


plot 'dblp_wc/stat_IMM_dblp_WC_0.1.txt' using 1:2 title '' lw 11 ps 3.5 pt 12 lt 1 lc 3 with linespoint,\
'dblp_wc/stat_TIM_dblp_WC_0.15.txt' using 1:2 title '' lw 11 ps 3.5 pt 4 lt 1 lc 4 with linespoint,\
'dblp_wc/stat_dblp_wc_200_r250.txt' using 1:2 title 'PMC' lw 11 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'dblp_wc/stat_dblp_IRIE_WC.txt' using 1:2 title '' lw 11 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'dblp_wc/stat_dblp_wc_200_r250sg.txt' using 1:2 title '' lw 11 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'dblp_wc/stat_dblp_imrank_num10_l1.txt' using 1:2 title 'IMRank1' lw 11 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'dblp_wc/stat_dblp_imrank_num10_l2.txt' using 1:2 title '' lw 11 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'dblp_wc/stat_st_dblp_wc_50.txt' using 1:2 title '' lw 11 ps 3.5 pt 5 lt 1 lc 9 with linespoint


unset label

set key at screen 3.8,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "Seeds"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,25,200

set label "(h) Youtube (WC)" at 100,-80000 center font "Helvetica,60,bold"


plot 'youtube_wc/stat_IMM_youtube_WC_0.1.txt' using 1:2 title '' lw 11 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'youtube_wc/stat_TIM_youtube_WC_0.15.txt' using 1:2 title '' lw 11 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'youtube_wc/stat_youtube_wc_200_r250.txt' using 1:2 title '' lw 11 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_wc/stat_youtube_wc_200_r300.txt' using 1:2 title 'SG' lw 11 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtube_wc/stat_youtube_IRIE_WC.txt' using 1:2 title '' lw 11 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_wc/stat_youtube_imrank_num10_l1.txt' using 1:2 title '' lw 11 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'youtube_wc/stat_youtube_imrank_num10_l2.txt' using 1:2 title 'IMRank2' lw 11 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtube_wc/stat_st_youtube_wc_50.txt' using 1:2 title '' lw 11 ps 3.5 pt 5 lt 1 lc 9 with linespoint





unset label
set key at screen 4.7,1.29
set size 1.1,1.2
set origin 4.85,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200


plot 'youtube_wc/stat_IMM_youtube_WC_0.1.txt' using 1:2 title '' lw 11 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'youtube_wc/stat_TIM_youtube_WC_0.15.txt' using 1:2 title '' lw 11 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'youtube_wc/stat_youtube_wc_200_r250.txt' using 1:2 title '' lw 11 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_wc/stat_youtube_IRIE_WC.txt' using 1:2 title 'IRIE' lw 11 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_wc/stat_youtube_imrank_num10_l1.txt' using 1:2 title '' lw 11 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'youtube_wc/stat_youtube_imrank_num10_l2.txt' using 1:2 title '' lw 11 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtube_wc/stat_st_youtube_wc_50.txt' using 1:2 title 'EaSyIM' lw 11 ps 3.5 pt 5 lt 1 lc 9 with linespoint
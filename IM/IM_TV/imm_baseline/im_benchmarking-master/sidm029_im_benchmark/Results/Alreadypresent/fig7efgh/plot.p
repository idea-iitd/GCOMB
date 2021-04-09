
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set bmargin 7

#set key samplen 2
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



set multiplot layout 1,5
set key at screen 1.25,1.29
set size 1.1,1.2
set origin 0.05,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set label "(e) Nethept (WC)" at 100,0.000003 center font "Helvetica,60,bold"


set logscale y
plot 'nethept_wc/celf_200_10000_54.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_wc/celf++_200_7500_41.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF++' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_wc/nethept_WC_0.1_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_wc/nethept_WC_0.15_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_wc/nethept_wc_r250_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_wc/nethept_wc_r300_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_wc/nethept_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_wc/nethept_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_wc/nethept_irie_wc.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_wc/nethept_wc_time_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint

unset label
set size 1.1,1.2
set origin 1.25,0
set key at screen 2.05,1.29
set xlabel "#Seeds (k)"
set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(f) HepPh (WC)" at 100,0.000002 center font "Helvetica,60,bold"

plot 'hepph_wc/celf_200_10000_52.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepph_wc/celf++_200_7500_46.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepph_wc/hepph_WC_0.1_time_imm.txt' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepph_wc/hepph_WC_0.15_time.txt' using 1:2 title 'TIM^+' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepph_wc/hepph_wc_r250_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'hepph_wc/hepph_wc_r300_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'hepph_wc/hepph_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'hepph_wc/hepph_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'hepph_wc/hepph_irie_wc.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'hepph_wc/hepph_wc_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint


unset label
set key at screen 2.95,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(g) DBLP (WC)" at 100,0.003 center font "Helvetica,60,bold"

plot    'dblp_wc/dblp_WC_0.1_time_imm.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'dblp_wc/dblp_WC_0.15_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'dblp_wc/dblp_wc_r250_time.txt' using 1:2 title 'PMC' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'dblp_wc/dblp_wc_r300_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'dblp_wc/dblp_imrank_num10_l=1.txt' using 1:($2*0.613) title 'IMRank1' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'dblp_wc/dblp_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'dblp_wc/dblp_irie_wc.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'dblp_wc/dblp_wc_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint

unset label

set key at screen 3.8,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(h) Youtube (WC)" at 100,0.0015 center font "Helvetica,60,bold"

plot    'youtube_wc/youtube_WC_0.1_time_time_imm.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'youtube_wc/youtube_WC_0.15_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'youtube_wc/youtube_wc_r250_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_wc/youtube_wc_r300_time.txt' using 1:2 title 'SG' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtube_wc/youtube_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'youtube_wc/youtube_imrank_num10_l=2.txt' using 1:($2*0.613) title 'IMRank2' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtube_wc/youtube_irie_wc.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_wc/youtube_wc_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint

unset label
set key at screen 4.7,1.29
set size 1.1,1.2
set origin 4.85,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set logscale y
plot 'nethept_wc/celf_200_10000_54.txt_time.txt' using 1:($2*60) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_wc/celf++_200_7500_41.txt_time.txt' using 1:($2*60) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_wc/nethept_WC_0.1_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_wc/nethept_WC_0.15_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_wc/nethept_wc_r250_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_wc/nethept_wc_r300_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_wc/nethept_imrank_num10_l=1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_wc/nethept_imrank_num10_l=2.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_wc/nethept_irie_wc.txt' using 1:2 title 'IRIE' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_wc/nethept_wc_time_50_EASY' using 1:($2*50) title 'EaSyIM' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint

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

set label "(a) Nethept (IC)" at 100,0.000002 center font "Helvetica,60,bold"


set logscale y
plot 'nethept_ic/celf_200_10000_10.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_ic/celf++_200_7500_10.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF++' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_ic/nethept_IC_0.05_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_ic/nethept_IC_0.05_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_ic/nethept_ic_r200_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_ic/nethept_ic_r75_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_ic/nethept_irie_ic.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_ic/nethept_ic_time_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint

unset label
set size 1.1,1.2
set origin 1.25,0
set key at screen 2.05,1.29
set xlabel "#Seeds (k)"
set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(b) HepPh (IC)" at 100,0.000001 center font "Helvetica,60,bold"

plot 'hepph_ic/celf_200_10000_54.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepph_ic/celf++_200_7500_53.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepph_ic/hepph_IC_0.05_time_imm.txt' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepph_ic/hepph_IC_0.05_time.txt' using 1:2 title 'TIM^+' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepph_ic/hepph_ic_r200_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'hepph_ic/hepph_ic_r75_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'hepph_ic/hepph_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'hepph_ic/hepph_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'hepph_ic/hepph_irie_ic.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'hepph_ic/hepph_ic_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint


unset label
set key at screen 2.95,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(c) DBLP (IC)" at 100,0.003 center font "Helvetica,60,bold"

plot    'dblp_ic/dblp_IC_0.05_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'dblp_ic/dblp_ic_r200_time.txt' using 1:2 title 'PMC' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'dblp_ic/dblp_ic_r75_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'dblp_ic/dblp_imrank_num10_l=1.txt' using 1:($2*0.613) title 'IMRank1' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'dblp_ic/dblp_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'dblp_ic/dblp_irie_ic.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'dblp_ic/dblp_ic_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint

unset label

set key at screen 3.8,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(d) Youtube (IC)" at 100,0.003 center font "Helvetica,60,bold"

plot    'youtube_ic/youtube_ic_r200_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_ic/youtube_ic_r75_time.txt' using 1:($2*0.8701637468) title 'SG' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtube_ic/youtube_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'youtube_ic/youtube_imrank_num10_l=2.txt' using 1:($2*0.613) title 'IMRank2' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'youtube_ic/youtube_irie_ic.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_ic/youtube_ic_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint

unset label
set key at screen 4.7,1.29
set size 1.1,1.2
set origin 4.85,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set logscale y
plot 'nethept_ic/celf_200_10000_10.txt_time.txt' using 1:($2*60) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_ic/celf++_200_7500_10.txt_time.txt' using 1:($2*60) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_ic/nethept_IC_0.05_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_ic/nethept_IC_0.05_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_ic/nethept_ic_r200_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_ic/nethept_ic_r75_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=1.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=2.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_ic/nethept_irie_ic.txt' using 1:2 title 'IRIE' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_ic/nethept_ic_time_50_EASY' using 1:($2*50) title 'EaSyIM' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint
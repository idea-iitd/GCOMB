
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
set ylabel "Memory (MB)"
set xrange [0:200]

set xtics 0,25,200
set logscale y
set label "(a) Nethept (IC)" at 100,0.03 center font "Helvetica,60,bold"


plot 'nethept_ic/celf_200_10000_10.txt_memory.txt' using 1:2 title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_ic/celf++_200_7500_10.txt_memory.txt' using 1:2 title 'CELF++' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_ic/nethept_IC_0.05_memory_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3  with linespoint, \
'nethept_ic/nethept_IC_0.05_memory_tim.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_ic/nethept_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_ic/static_greedy_75.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_ic/nethept_irie_ic_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_ic/nethept_ic_memory' using 1:2 title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint, \
'nethept_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint
#'netheptic/stat_nethept_imrank_num10_l1.txt' using 1:2 title '' lw 6.5 ps 3.5 lt 1 lc 8 with linespoint, \



unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 2.05,1.29

set xlabel "#Seeds (k)"
set ylabel "Memory (MB)"
set xrange [0:200]

set xtics 0,25,200
set logscale y

set label "(b) HepPh (IC)" at 100,0.02 center font "Helvetica,60,bold"




plot 'hepph_ic/celf_200_10000_54.txt_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'hepph_ic/celf++_200_7500_53.txt_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepph_ic/hepph_IC_0.05_memory.txt' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 12 lt 1 lc 3  with linespoint, \
'hepph_ic/hepph_IC_0.05_memory_tim.txt' using 1:2 title 'TIM^+' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepph_ic/hepph_ic_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'hepph_ic/static_greedy_75.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'hepph_ic/hepph_irie_ic_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'hepph_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 8 with linespoint, \
'hepph_ic/hepph_ic_memory' using 1:2 title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint, \
'hepph_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint
#'hepph_ic/stat_hepph_imrank_num10_l1.txt' using 1:2 title '' lw 6.5 ps 3.5 lt 1 lc 8 with linespoint, \


unset label
#unset key
set key at screen 2.95,1.29
set size 1.1,1.2
set origin 2.45,0

set xlabel "#Seeds (k)"
set ylabel "Memory (MB)"
set xrange [0:200]

set xtics 0,25,200
set logscale y

set label "(c) DBLP (IC)" at 100,0.3 center font "Helvetica,60,bold"



plot    'dblp_ic/dblp_IC_0.05_memory_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'dblp_ic/dblp_IC_0.05_memory_tim.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'dblp_ic/dblp_ic_200_memory.txt' using 1:2 title 'PMC' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'dblp_ic/static_greedy_75.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'dblp_ic/dblp_irie_ic_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'dblp_ic/imrank.txt' using 1:2 title 'IMRank1' lw 6.5 ps 3.5 lt 1 lc 7 with linespoint, \
'dblp_ic/dblp_ic_memory' using 1:2 title ''lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint, \
'dblp_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint

unset label

unset label
#unset key
set key at screen 3.8,1.29
set size 1.1,1.2
set origin 3.65,0

set xlabel "#Seeds (k)"
set ylabel "Memory (MB)"
set xrange [0:200]

set xtics 0,25,200
set logscale y

set label "(d) Youtube (IC)" at 100,25 center font "Helvetica,60,bold"


plot    'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_ic/static_greedy_75.txt' using 1:2 title 'SG' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtube_ic/youtube_irie_ic_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_ic/imrank.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 8 with linespoint, \
'youtube_ic/youtube_ic_memory' using 1:2 title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint, \
'youtube_ic/imrank.txt' using 1:2 title 'IMRank2' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint
#'youtube_ic/stat_youtube_imrank_num10_l1.txt' using 1:2 title 'IMRank (l=1)' lw 6.5 ps 3.5 pt 6 lt 1 lc 8 with linespoint, \



unset label
set key at screen 4.7,1.29
set size 1.1,1.2
set origin 4.85,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set logscale y


plot    'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title 'IRIE' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 8 with linespoint, \
'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title 'EaSyIM' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'youtube_ic/youtube_wc_200_memory.txt' using 1:2 title '' lw 6.5 ps 3.5 lt 1 lc 7 with linespoint

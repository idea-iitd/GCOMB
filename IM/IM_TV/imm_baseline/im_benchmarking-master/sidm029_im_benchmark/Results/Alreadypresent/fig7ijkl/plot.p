
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

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200

set label "(i) Nethept (LT)" at 100,0.0015 center font "Helvetica,60,bold"


set logscale y
plot 'nethept_lt/celf_200_10000_55.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_lt/celf++200_10000_46.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF++' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'nethept_lt/nethept_LT_0.1_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_lt/nethept_LT_0.35_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_lt/nethept_ldag_output.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'nethept_lt/nethept_simpath_200.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 44 lt 1 lc '#9ACD32' with linespoint, \
'nethept_lt/nethept_lt_time_25' using 1:($2*25*0.8701637468) title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint

unset label
set size 1.1,1.2
set origin 1.25,0
set key at screen 2.65,1.29
set xlabel "#Seeds (k)"
set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(j) HepPh (LT)" at 100,0.0003 center font "Helvetica,60,bold"

plot 'hepph_lt/celf_200_10000_52.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'hepph_lt/celf++_200_10000_51.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint, \
'hepph_lt/hepph_LT_0.1_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'hepph_lt/hepph_LT_0.35_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'hepph_lt/hepph_ldag_output.txt_time.txt' using 1:($2*60*0.8701637468) title 'LDAG' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'hepph_lt/hepph_simpath_200.txt' using 1:($2*60*0.8701637468) title 'SIMPATH' lw 6.5 ps 3.5 pt 44 lt 1 lc '#9ACD32' with linespoint, \
'hepph_lt/hepph_lt_25_EASY' using 1:($2*25*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint


unset label
set key at screen 3.45,1.29
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(k) DBLP (LT)" at 100,0.06 center font "Helvetica,60,bold"

plot 'dblp_lt/dblp_LT_0.1_time_imm.txt' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'dblp_lt/dblp_LT_0.35_time.txt' using 1:2 title 'TIM^+' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'dblp_lt/dblp_ldag_output.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'dblp_lt/dblp_lt_25_EASY' using 1:($2*25*0.8701637468) title '' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint

unset label

set key at screen 4,1.29
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200
set logscale y

set label "(l) Youtube (LT)" at 100,0.015 center font "Helvetica,60,bold"

plot 'youtube_lt/youtube_LT_0.1_time_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'youtube_lt/youtube_LT_0.35_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'youtube_lt/youtube_ldag_output.txt_time.txt' using 1:($2*60*0.8701637468) title '' lw 6.5 ps 3.5 pt 28 lt 1 lc '#2F4F4F' with linespoint, \
'youtube_lt/youtube_lt_25_EASY' using 1:($2*25*0.8701637468) title 'EaSyIM' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint
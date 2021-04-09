
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



set size 1.1,1.2
set xlabel "#Seeds (k)"

set ylabel "Time (s)"
set xrange [0:200]

set xtics 0,50,200



set logscale y
plot 'nethept_ic/celf_200_10000_10.txt_time.txt' using 1:($2*60*0.8701637468) title 'CELF' lw 6.5 ps 3.5 pt 8 lt 1 lc 'coral' with linespoint, \
'nethept_ic/nethept_IC_0.05_time_imm.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'nethept_ic/nethept_IC_0.05_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'nethept_ic/nethept_ic_r200_time.txt' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'nethept_ic/nethept_ic_r75_time.txt' using 1:($2*0.8701637468) title '' lw 6.5 ps 3.5 pt 6 lt 1 lc 6 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=1.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 1 lt 1 lc 7 with linespoint, \
'nethept_ic/nethept_imrank_num10_l=2.txt' using 1:($2*0.613) title '' lw 6.5 ps 3.5 pt 2 lt 1 lc 8 with linespoint, \
'nethept_ic/nethept_irie_ic.txt' using 1:($2*0.497) title '' lw 6.5 ps 3.5 pt 3 lt 1 lc 'brown' with linespoint, \
'nethept_ic/nethept_ic_time_50_EASY' using 1:($2*50*0.8701637468) title '' lw 6.5 pt 5 ps 3.5 lt 1 lc 9 with linespoint

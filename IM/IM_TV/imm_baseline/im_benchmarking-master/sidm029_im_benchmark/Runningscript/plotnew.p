
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



set size 1.1,1.2
set origin 0.05,0
set xlabel "Epsilon  ({/Symbol e})"
set ylabel "Spread"
set xrange [1:0]

set xtics 0,0.2,1

set label "(i) TIM^+ (LT)" at 0.5,-1650 center font "Helvetica,60,bold"


plot 'output.txt' using 1:($2) title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint,\
'output.txt' using 1:3 title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint,\
'output.txt' using 1:($4) title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint,\
'output.txt' using 1:($5) title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint,\
'output.txt' using 1:($6) title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint



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
set xlabel "#Seeds (k)"
set ylabel "Memory"
set xrange [0:200]

set xtics 0,50,200

set logscale y

plot 'immtime.txt' using 1:4 title 'imm' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'timtime.txt' using 1:4 title 'tim' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'ldagtime.txt' using 1:4 title 'ldag' lw 6.5 ps 3.5 pt 5 lt 1 lc 9 with linespoint,\
'simpathtime.txt' using 1:4 title 'simpath' lw 6.5 ps 3.5 lt 1 lc 7 with linespoint,\
'easyimtime.txt' using 1:4 title 'easyim' lw 6.5 ps 3.5 lt 1 lc 10 with linespoint



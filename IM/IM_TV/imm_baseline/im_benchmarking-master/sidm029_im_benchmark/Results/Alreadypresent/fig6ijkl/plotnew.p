
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


set key out# horizontal top

set key font "Helvetica,46,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,36,bold"
set ytics font "Helvetica,36,bold"
set title font "Helvetica,42,bold"



#set key at screen 1.25,1.29
set size 1.1,1.2
set origin 0.05,0
set xlabel "#Seeds (k)"
set ylabel "Spread"
set xrange [0:200]

set xtics 0,50,200

#set label "(a) Nethept (IC)" at 100,335 center font "Helvetica,60,bold"


plot 'imm.txt' using 1:2 title 'imm' lw 6.5 ps 3.5 pt 12 lt 1 lc 3 with linespoint, \
'tim.txt' using 1:2 title 'tim' lw 6.5 ps 3.5 pt 4 lt 1 lc 4 with linespoint, \
'pmc.txt' using 1:2 title 'pmc' lw 6.5 ps 3.5 pt 14 lt 1 lc 5 with linespoint, \
'easyim.txt' using 1:2 title 'easyim' lw 6.5 ps 3.5 lt 1 lc 6 with linespoint,\
'simpath.txt' using 1:2 title 'simpath' lw 6.5 ps 3.5 lt 1 lc 7 with linespoint,\
'ldag.txt' using 1:2 title 'ldag' lw 6.5 ps 3.5 lt 1 lc 8 with linespoint



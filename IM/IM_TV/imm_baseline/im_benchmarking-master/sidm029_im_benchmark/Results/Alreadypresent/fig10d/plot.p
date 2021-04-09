
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1
set bmargin 4


#set key bottom left
set key at 0.51, 13600
#set key at 0.51, 15000
set key samplen 2
set key font "Helvetica,42,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"


set xlabel "Epsilon ({ /Symbol e})"
set ylabel "Spread"

set xtics 0,0.2,1

plot 'imm/dblp_WC_other200' using 1:2 title 'IMM' lw 6.5 ps 4.5 pt 8 lt 1 lc 3 with linespoint, \
'imm/stat_IMM_dblp_WC_other200' using 1:($2+200)  title 'IMM ({ /Symbol s})' lw 7.5 ps 4.5 pt 4 lt 1 lc 4 with linespoint, \
'dblp_WC_other200' using 1:($2+200)   title 'TIM' lw 6.5 ps 3.5 pt 6 lt 1 lc 1 with linespoint, \
'stat_TIM_dblp_WC_other200' using 1:($2+200) title 'TIM ({ /Symbol s})' lw 6.5 ps 3.5 pt 10 lt 1 lc 2 with linespoint

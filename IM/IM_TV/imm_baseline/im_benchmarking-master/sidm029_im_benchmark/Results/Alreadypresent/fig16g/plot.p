
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set view equal xy
set size 1,1.1


set key

set key font "Helvetica,32,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,34,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set xlabel "#Seeds"
set ylabel "epsilon ({/Symbol e})"
set ytics 0,0.1,1
set xtics 0,40,200

plot 'nethept_imm_ic' using 1:2 title 'IMM' lw 6.5 ps 3.5 pt 14 lt 1 lc 1 with linespoint, \
'tim_nethept_ic' using 1:2 title 'TIM^{+}' lw 6.5 ps 3.5 pt 4 lt 1 lc 2 with linespoint
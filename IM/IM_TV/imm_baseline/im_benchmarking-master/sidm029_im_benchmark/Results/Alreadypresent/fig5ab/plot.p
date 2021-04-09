
reset

set term postscript eps dashed color enhanced 24


set output "output.eps"

set bmargin 7

#set key samplen 6
#set key spacing 15
#set key default
#set key at 2.5,1
#set key out horizontal top left
set size 2.4,1.3


set key out horizontal top

set key font "Helvetica,46,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,42,bold"
set xtics font "Helvetica,36,bold"
set ytics font "Helvetica,36,bold"
set title font "Helvetica,42,bold"



set multiplot layout 1,3
unset key
set size 1.1,1.2
set origin 0.05,0
set key at screen 1,1.26
set xlabel "#Scoring Rounds"
set ylabel "Spread"
set xrange [1:10]

set xtics 0,2,10

set label "(a) IMRank (IC) (l=1)" at 5.5,3790 center font "Helvetica,60,bold"

plot 'icl1/stat_hepph_imrank_numother1' using 1:($2+1) title 'k=1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'icl1/stat_hepph_imrank_numother50' using 1:($2+50) title 'k=50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'icl1/stat_hepph_imrank_numother100' using 1:($2+100) title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'icl1/stat_hepph_imrank_numother150' using 1:($2+150) title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'icl1/stat_hepph_imrank_numother200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 1.85,1.26
set xlabel "#Scoring Rounds"
set ylabel "Spread"
set xrange [1:10]

set xtics 0,2,10

set label "(b) IMRank (IC) (l=2)" at 5.5,3748 center font "Helvetica,60,bold"

plot 'icl2/stat_hepph_imrank_numother1' using 1:($2+1) title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'icl2/stat_hepph_imrank_numother50' using 1:($2+50) title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'icl2/stat_hepph_imrank_numother100' using 1:($2+100) title 'k=100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'icl2/stat_hepph_imrank_numother150' using 1:($2+150) title 'k=150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'icl2/stat_hepph_imrank_numother200' using 1:($2+200) title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
unset key
set size 1.1,1.2
set origin 2.45,0

set key at screen 2.25,1.26
set xlabel "#Scoring Rounds"
set ylabel "Spread"
set xrange [1:10]

set xtics 0,2,10

plot 'icl2/stat_hepph_imrank_numother1' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'icl2/stat_hepph_imrank_numother50' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'icl2/stat_hepph_imrank_numother100' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'icl2/stat_hepph_imrank_numother150' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'icl2/stat_hepph_imrank_numother200' using 1:2 title 'k=200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

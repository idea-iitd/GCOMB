
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



set multiplot layout 1,4
unset key
set size 1.1,1.2
set origin 0.05,0

set xlabel "#Snapshots (R)"
set ylabel "Spread"
set xrange [0:300]

set xtics 0,50,300

set label "(m) PMC (IC)" at 150,3115 center font "Helvetica,60,bold"


plot 'pmcic/stat_hepph_ic_200_rother1' using 1:2 title 'k = 1' lw 6.5 ps 3.5 pt 14 lt 1 lc 1 with linespoint, \
'pmcic/stat_hepph_ic_200_rother50' using 1:2 title 'k = 50' lw 6.5 ps 3.5 lt 1 lc 6 with linespoint, \
'pmcic/stat_hepph_ic_200_rother100' using 1:2 title 'k = 100' lw 6.5 ps 3.5 lt 1 lc 8 with linespoint, \
'pmcic/stat_hepph_ic_200_rother150' using 1:2 title 'k = 150' lw 6.5 ps 3.5 lt 1 lc 10 with linespoint, \
'pmcic/stat_hepph_ic_200_rother200' using 1:2 title 'k = 200' lw 6.5 ps 3.5 lt 1 lc 12 with linespoint


unset label
unset key
set size 1.1,1.2
set origin 1.25,0

set key at screen 1.5,1.26
set xlabel "#Snapshots (R)"
set ylabel "Spread"
set xrange [0:300]

set xtics 0,50,300

set label "(n) PMC (WC)" at 150,-930 center font "Helvetica,60,bold"



plot 'pmcwc/stat_hepph_wc_200_rother1' using 1:2 title 'k = 1' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'pmcwc/stat_hepph_wc_200_rother50' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'pmcwc/stat_hepph_wc_200_rother100' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'pmcwc/stat_hepph_wc_200_rother150' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'pmcwc/stat_hepph_wc_200_rother200' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label
#unset key
set key at screen 2.55,1.26
set size 1.1,1.2
set origin 2.45,0
set xlabel "#Snapshots (R)"
set ylabel "Spread"
set xrange [0:300]

set xtics 0,50,300

set label "(o) SG (IC)" at 150,3785 center font "Helvetica,60,bold"


plot 'ic/stat_hepph_ic_200_rother1' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'ic/stat_hepph_ic_200_rother50' using 1:2 title 'k = 50' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'ic/stat_hepph_ic_200_rother100' using 1:2 title 'k = 100' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'ic/stat_hepph_ic_200_rother150' using 1:2 title '' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'ic/stat_hepph_ic_200_rother200' using 1:2 title '' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

unset label

set key at screen 3.6,1.26
set size 1.1,1.2
set origin 3.65,0
set xlabel "#Snapshots (R)"
set ylabel "Spread"
set xrange [0:300]

set xtics 0,50,300

set label "(p) SG (WC)" at 150,-930 center font "Helvetica,60,bold"

plot 'wc/stat_hepph_wc_200_rother1' using 1:2 title '' lw 6.5 ps 3.5 pt 8 lt 1 lc 1 with linespoint, \
'wc/stat_hepph_wc_200_rother50' using 1:2 title '' lw 6.5 ps 3.5 pt 10 lt 1 lc 6 with linespoint, \
'wc/stat_hepph_wc_200_rother100' using 1:2 title '' lw 6.5 ps 3.5 pt 12 lt 1 lc 8 with linespoint, \
'wc/stat_hepph_wc_200_rother150' using 1:2 title 'k = 150' lw 6.5 ps 3.5 pt 4 lt 1 lc 10 with linespoint, \
'wc/stat_hepph_wc_200_rother200' using 1:2 title 'k = 200' lw 6.5 ps 3.5 pt 14 lt 1 lc 12 with linespoint

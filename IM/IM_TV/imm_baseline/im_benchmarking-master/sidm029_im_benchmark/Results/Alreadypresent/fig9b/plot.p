set term postscript eps enhanced 24
#set term epslatex 18
set view equal xy
set size 1,1.1
set output 'output.eps'

set boxwidth 0.9 absolute
set style fill   solid 1.00 border lt -1
set key inside right top nobox
set style histogram clustered gap 2 title textcolor lt -1
set datafile missing '-'
set style data histograms
#set xtics border in scale 0,0 nomirror rotate by -45
set xtics border in scale 0,0 nomirror rotate by 0
set xtics  norangelimit
set xtics   ()

#set xtics nomirror offset 0,graph 0.02
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autofreq 
#set title "Query Time Comparison"

set key spacing 1.3
set key font "Helvetica,33,bold"
set xlabel font "Helvetica,42,bold"
set ylabel font "Helvetica,48,bold"
set xtics font "Helvetica,39,bold"
set ytics font "Helvetica,34,bold"
set title font "Helvetica,42,bold"

set ylabel "Running Time (in min)" 

set yrange [ 110 : 190 ] noreverse nowriteback
set ytics 120,10,180

plot 'data.txt' using ($2):xtic(1) ti col lw 4 lc 1 lt 1, '' u ($3) ti col lw 4 lc 2 lt 1

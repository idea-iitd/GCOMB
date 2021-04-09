#!/bin/bash
execfilesimpath="../Executables/simpath"
execfileldag="../Executables/ldag"
pathtograph="../Dataset/dblp/dblp_lt_parallel.txt"
ldagmodel="LT"
simpathmodel="LTNew"
outputsimpath="../Results/freshRuns/algorithmOutput/fig10b/simpath"
outputldag="../Results/freshRuns/algorithmOutput/fig10b/ldag"
mkdir -p ../Results/freshRuns/algorithmOutput/fig10b
mkdir -p ../Results/freshRuns/processed/fig10b
mkdir -p $outputsimpath
mkdir -p $outputldag

ldagPhase=15
simpathPhase=17

mc=10000
#mc=1 #This is a test value, use the above value for actual runs!

echo "Running SIMPATH" 
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph" -outdir "$outputsimpath
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph -outdir $outputsimpath
cp $outputsimpath"/stats_LTNew_10000.txt" ../Results/freshRuns/processed/fig10b/simpath.txt

echo "Running LDAG" 
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph" -outdir "$outputldag
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph -outdir $outputldag
cp $outputldag"/stats_LT.txt" ../Results/freshRuns/processed/fig10b/ldag.txt

cp ../Results/Alreadypresent/fig10b/plotnew.p ../Results/freshRuns/processed/fig10b/
cd ../Results/freshRuns/processed/fig10b/
gnuplot plotnew.p
epspdf output.eps


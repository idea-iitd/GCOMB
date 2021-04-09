#!/bin/bash
execfilesimpath="../Executables/simpath"
execfileldag="../Executables/ldag"
pathtograph_netheptP="../Dataset/nethept/nethept_lt_parallel.txt"
pathtograph_nethept="../Dataset/nethept/graph_lt.inf"
pathtograph_hepph="../Dataset/hepph/graph_lt.inf"
pathtograph_dblp="../Dataset/dblp/graph_lt.inf"
pathtograph_dblpP="../Dataset/dblp/dblp_lt_parallel.txt"
ldagmodel="LT"
simpathmodel="LTNew"
outputsimpath_nethept="../Results/freshRuns/algorithmOutput/table4/simpath_nethept"
outputldag_nethept="../Results/freshRuns/algorithmOutput/table4/ldag_nethept"

outputsimpath_netheptP="../Results/freshRuns/algorithmOutput/table4/simpath_netheptP"
outputldag_netheptP="../Results/freshRuns/algorithmOutput/table4/ldag_netheptP"

outputsimpath_hepph="../Results/freshRuns/algorithmOutput/table4/simpath_hepph"
outputldag_hepph="../Results/freshRuns/algorithmOutput/table4/ldag_hepph"

outputsimpath_dblp="../Results/freshRuns/algorithmOutput/table4/simpath_dblp"
outputldag_dblp="../Results/freshRuns/algorithmOutput/table4/ldag_dblp"

outputsimpath_dblpP="../Results/freshRuns/algorithmOutput/table4/simpath_dblpP"
outputldag_dblpP="../Results/freshRuns/algorithmOutput/table4/ldag_dblpP"
mkdir -p ../Results/freshRuns/algorithmOutput/table4
mkdir -p $outputsimpath_nethept
mkdir -p $outputsimpath_netheptP
mkdir -p $outputsimpath_hepph
mkdir -p $outputsimpath_dblp
mkdir -p $outputsimpath_dblpP
mkdir -p $outputldag_nethept
mkdir -p $outputldag_netheptP
mkdir -p $outputldag_hepph
mkdir -p $outputldag_dblp
mkdir -p $outputldag_dblpP

ldagPhase=15
simpathPhase=17

mc=10000
#mc=1 #This is a test value, use the above value for actual runs!

echo "Running SIMPATH" 
echo "Nethept"
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph_nethept" -outdir "$outputsimpath_nethept
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph_nethept -outdir $outputsimpath_nethept

echo "NetheptP"
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph_netheptP" -outdir "$outputsimpath_netheptP
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph_netheptP -outdir $outputsimpath_netheptP

echo "hepph"
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph_hepph" -outdir "$outputsimpath_hepph
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph_hepph -outdir $outputsimpath_hepph


echo "DBLP-P"
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph_dblpP" -outdir "$outputsimpath_dblpP
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph_dblpP -outdir $outputsimpath_dblpP

echo "Running LDAG" 
echo "Nethept"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_nethept" -outdir "$outputldag_nethept
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_nethept -outdir $outputldag_nethept

echo "Nethept-P"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_netheptP" -outdir "$outputldag_netheptP
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_netheptP -outdir $outputldag_netheptP

echo "hepph"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_hepph" -outdir "$outputldag_hepph
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_hepph -outdir $outputldag_hepph

echo "DBLP"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_dblp" -outdir "$outputldag_dblp
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_dblp -outdir $outputldag_dblp

echo "DBLP-P"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_dblpP" -outdir "$outputldag_dblpP
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_dblpP -outdir $outputldag_dblpP


echo "SIMPATH DBLP"
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph_dblp" -outdir "$outputsimpath_dblp
$execfilesimpath -propModel $simpathmodel -mcruns $mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph_dblp -outdir $outputsimpath_dblp

mkdir -p ../Results/freshRuns/processed/table4/
python processtable4.py  > ../Results/freshRuns/processed/table4/table4.txt

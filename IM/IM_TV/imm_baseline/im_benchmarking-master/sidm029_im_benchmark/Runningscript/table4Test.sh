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

echo "Running LDAG" 
echo "Nethept"
echo $execfileldag" -propModel "$ldagmodel" -mcruns "$mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph_nethept" -outdir "$outputldag_nethept
$execfileldag -propModel $ldagmodel -mcruns $mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph_nethept -outdir $outputldag_nethept



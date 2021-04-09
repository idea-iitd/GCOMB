#!/bin/bash
mkdir -p ../Executables
cd ../Codes

echo "Compiling tim_format.cpp for formatting the downloaded datasets"
g++ -std=c++0x -o timFormat tim_format.cpp 
cp timFormat ../Executables/

echo "Compiliing Celf and Celf++"
cd CelfCelf++
make clean
make
cp InfluenceModels ../../Executables/celf
cp InfluenceModels ../../Executables/celfpp
cd ../CelfCelf++_RandomSeed
make clean
make
cp InfluenceModels ../../Executables/celfRandom
cp InfluenceModels ../../Executables/celfppRandom
echo "Compiling EaSyIM"
cd ../EasyIM
cp easyimIC ../../Executables/
cp easyimWC ../../Executables/
cp easyimLT ../../Executables/

echo "Compiling IMM"
cd ../IMM
make clean
make
cp imm_discrete ../../Executables/imm

echo "compiling PMC"
cd ../pruned-monte-carlo
make clean
make
cp pmc ../../Executables/

echo "compiling SIMPATH"
cd ../SIMPATH
make clean
make
cp InfluenceModels ../../Executables/simpath
cp InfluenceModels ../../Executables/ldag

echo "compiling TIM"
cd ../TIM
make clean
make
cp tim ../../Executables/tim

echo "compiling flow executable"
cd ../flow
cp icFlow ../../Executables
cp wcFlow ../../Executables
cp ltFlow ../../Executables

cd ../flow_seeds_input
cp icFlowSeeds ../../Executables
cp wcFlowSeeds ../../Executables
cp ltFlowSeeds ../../Executables

cd ../../Runningscript

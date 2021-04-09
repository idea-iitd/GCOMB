#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph=$1
pathtoseeds=$2
datasetname=$3
seedIndex=$4
cores=$5
model="IC"
execFile="./icFlowSeeds"
prefix="IMM_"$datasetname"_"$model
num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=0.1
declare -a seeds=(1 1 2 5 10 25 50 75 100 125 150 175 200);
for eps in $num
do 
  for mc in 1000 2500 5000 7500 10000 15000 20000
  do
  	rm -r $pathtoseeds$prefix"_"$eps"_"$mc
  	mkdir -p $pathtoseeds$prefix"_"$eps"_"$mc
	for i in $(seq 1 1 $seedIndex)
	#i=$seedIndex
	do
		seedfile=$pathtoseeds$prefix"_"$eps"_"${seeds[i]}".txt"
		echo $prefix"_"$eps"_"${seeds[i]}".txt" 
    		python experiments.py $execFile $pathtograph $seedfile ${seeds[i]} -1 $mc 1 $pathtoseeds$prefix"_"$eps"_"$mc $cores
	done
  done
done

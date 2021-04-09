for loop in `seq 1 10`;
	do
	for seed in 1 2 5 10 25 50 75 100 125 150 175 200;
        	do
                	echo $loop $seed
			g++ -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w -DIMRank_LOOP=$loop -DSET_SIZE=$seed
			{ time ./mymain -imrank ../orkut/data_imrank/youtubeSG.txt > script_results/"memory_"$loop"_"$seed"_1.txt" ;} 2>> script_results/"time_imrank_"$loop"_"$seed"_1.txt"
		done 
	done

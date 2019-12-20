result_ordered.tsv: result.tsv
	sort -k4n -k1n result.tsv > result_ordered.tsv

result.tsv: branchsimulator_oknotok.cpp config.txt trace.txt
	g++ -o branchsimulator.out branchsimulator_oknotok.cpp
	./branchsimulator.out config.txt trace.txt 2>&1 | tee result.tsv

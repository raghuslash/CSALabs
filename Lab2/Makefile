result_ordered.tsv: result.tsv
	sort -k6 -k4n result.tsv > result_ordered.tsv

result.tsv: branchsimulator_oknotok.cpp
	g++ -o branchsimulator_oknotok branchsimulator_oknotok.cpp
	./branchsimulator_oknotok config.txt trace.txt > result.tsv
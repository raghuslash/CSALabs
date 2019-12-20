#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <bitset>
#include <sstream> 

//OKNOTOK

using namespace std;

uint bhr;

vector<bitset<2>> pCol(pow(2,20)*pow(2,8), 0x03);

void updateBhr(bool taken, uint k){
		bhr = (bhr << 1) & (uint)(pow (2, k) -1);
		if (taken)
			bhr = bhr | 0x01;
	}

bool predict(unsigned long pcm){
	pcm+=bhr*pow(2,20);
	return pCol.at(pcm).to_ulong() & 0b01;
}

void updateTable(unsigned long pcm, bool taken){
	pcm+=bhr*pow(2,20);
	switch (pCol.at(pcm).to_ulong()){
		case 3: if (!taken)
					pCol.at(pcm) = 0x01;
				break;
		case 1: if (taken)
					pCol.at(pcm) = 0x03;
				else
					pCol.at(pcm) = 0X00;
				break;
		case 0: if (taken)
					pCol.at(pcm) = 0x02;
				break;
		case 2: if (taken)
					pCol.at(pcm) = 0x03;
				else
					pCol.at(pcm) = 0x00;

	}
	cout<<"Taken is "<< taken <<". Updated to "<<pCol.at(pcm);
}

int main (int argc, char** argv) {
	ifstream config;
	config.open(argv[1]);

	int m, k;
	config >> m >> k;
	uint nrows=pow(2,m);
	bhr=pow(2,k)-1;
 	
	config.close();

	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open(argv[2]);
	unsigned long pc; bool taken;
	bool prediction;

	trace >> std::hex >> pc >> taken;
	uint count=1;
	uint mispred=0;
	while (!trace.eof()){
		prediction = predict(pc%nrows);
		out << prediction;
		cout<<count<<'\t';
		cout<< "Predicted:\t" << prediction << '\t';
		cout<< pc <<":\t"<< taken << '\t';
		cout<< "BHR:" << bhr <<'\t';
		updateTable(pc%nrows, taken);
		updateBhr(taken, k);
		trace >> std::hex >> pc >> taken;
		cout<<endl;
		if (taken != prediction)
			mispred++;
		count++;
	};

	cout<<"Misprediction rate is "<<(float)mispred/count*100<<"."<<endl;

	trace.close();	
	out.close();
}
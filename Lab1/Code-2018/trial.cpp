#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;

int main(){
	// bitset <32> a=0b01111111111111111111111111111110;
	bitset <32> b=0xFFFF0000;
	cout<<(int)(b.to_ulong());
	return 0;
}
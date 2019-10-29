#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;

int main(){
	bitset <32> a=0x01111111111111111111111111111110;
	bitset <32> b=0x11111111111111111111111111111111;
	cout<<a.to_ulong()+b.to_ulong();
	return 0;
}
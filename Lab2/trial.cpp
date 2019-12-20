#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include <sstream> 
#include<math.h>
using namespace std;
#define MemSize 1000 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.



struct branch{
	bitset <32> instAddr;
	bitset <1> taken;
};

// class INSMem
// {
// 	public:
//         bitset<32> Instruction;
//         INSMem()
//         {       
// 			IMem.resize(MemSize); 
//          ifstream imem;
// 			string line;
// 			int i=0;
// 			imem.open("imem.txt");
// 			if (imem.is_open())
// 			{
// 				while (getline(imem,line))
// 				{      
// 					IMem[i] = bitset<8>(line);
// 					i++;
// 				}                    
// 			}
//             else cout<<"Unable to open file";
// 			imem.close();                     
// 		}
                  
// 		bitset<32> readInstr(bitset<32> ReadAddress) 
// 		{    
// 			string insmem;
// 			insmem.append(IMem[ReadAddress.to_ulong()].to_string());
// 			insmem.append(IMem[ReadAddress.to_ulong()+1].to_string());
// 			insmem.append(IMem[ReadAddress.to_ulong()+2].to_string());
// 			insmem.append(IMem[ReadAddress.to_ulong()+3].to_string());
// 			Instruction = bitset<32>(insmem);		//read instruction memory
// 			return Instruction;     
// 		}     
      
//     private:
//         vector<bitset<8> > IMem;     
// };

int main(){


		vector<branch> t1;
		ulong addrInt;
		bitset<32> addr;
	    ifstream tracef;
	    string line;
		string word;
		int i=0;
		uint bhr = pow (2, 8) -1;
		bhr = (bhr << 1) & (uint)(pow (2, 8) -1);
		bhr = bhr | 0x01;
		cout<<bhr<<endl;

		return 0;
}